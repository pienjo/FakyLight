#include "WS2812Strip.h"
#include <stdexcept>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
  /* Determination of transfer speed:
     
     Max pulse time for a 0-bit :  500 ns
     Min pulse time for a 1-bit :  550 ns

     Min space between successive bits: 450 ns

     To transfer using 4 bit chunks, ( 1-pulse = 1100, 0-pulse = 1000 ) the pulse time must be no lower than
       550/2 = 275 ns -> Max bitrate of 3.636 MHz. 
     Minimum pulse time = 500 ns -> min bitrate of 2 MHz. 

     Fortunately, the OrangePi's SPI module seems to have a valid bitrate at 3 MHz. This yields the folowing times:

     0-bit: Written using 1000 nybble
	    T0H = 333 ns ( within spec, < 500)
	    T0L = 999 ns ( within simplified spec: > 450 ns, and < 5000 ns
     1-bit: Written using 1100 nybble
	    T1H = 666 ns ( within spec, > 550 )
	    T1L = 666 ns ( within simplified spec: > 450 ns, and < 5000 ns

     Due to the unreliable timing of the Linux (or OrangePi's?) SPI module (it can stretch the first bit in a byte..) 
     it is more reliable to move the 1-bits to the middle of the nybble (so the space actually gets divided
     between to successive nybbles): So write as 0100 for a 0-bit, and as 0110 for a 1-bit. 

     At this speed, the minimum latch time (6000 ns) boils down to 18 bits - rounded up to 3 bytes.
  */

#define BITRATE 2000000
#define LATCHBYTES  3 

#define DEVICENAME "/dev/spidev1.0"

using namespace std::chrono_literals;

#define MIX_TIME 1s
#define IDLE_TIME 30s
#define MIX_DELAY 20ms

WS2812Strip::WS2812Strip(IPin &pEnablePin)
  : mDeviceDescriptor(-1)
  , mEnablePin(pEnablePin)
{
  mDeviceDescriptor = open(DEVICENAME, O_RDWR);
  if (mDeviceDescriptor < 0)
  {
    throw std::runtime_error("Unable to open SPI device");
  }
  
  // Setup device

  uint8_t mode = SPI_MODE_3;
  uint8_t bitsPerWord = 8;
  uint32_t speed = BITRATE;

  if (0 != ioctl(mDeviceDescriptor, SPI_IOC_WR_MODE, &mode))
    throw std::runtime_error("Unable to set write mode");
  
  if (0 != ioctl(mDeviceDescriptor, SPI_IOC_WR_BITS_PER_WORD, &bitsPerWord))
    throw std::runtime_error("Unable to set bits per word");
 
  if (0 != ioctl(mDeviceDescriptor, SPI_IOC_WR_MAX_SPEED_HZ, &speed))
    throw std::runtime_error("Unable to write speed");
  
  mEnablePin.Disengage();

  mDeviceStatus = STANDBY;
  mDeviceThread = std::thread([this]{ DeviceLoop(); } );
}

WS2812Strip::~WS2812Strip()
{
  do {
    std::lock_guard<std::mutex> lock(mDeviceStatusMutex);
    mDeviceStatus = STOPPING;
    mDeviceStatusCV.notify_one();
  } while(0);

  mDeviceThread.join();
  mEnablePin.Disengage();
  
  if (mDeviceDescriptor >= 0)
  {
    close(mDeviceDescriptor);
    mDeviceDescriptor = -1;
  }
}

void WS2812Strip::SetContents(const std::vector<RGBColor> &pValues) 
{
  std::vector<uint8_t> buf;
  buf.reserve(pValues.size() * 3);
  
  for (const RGBColor &c : pValues)
  {
    buf.push_back(c.g);
    buf.push_back(c.r);
    buf.push_back(c.b);
  }

  do {
    std::lock_guard<std::mutex> lock(mTargetValuesMutex);
    mTargetValues = std::move(buf);
  } while (0);

  do {
    std::lock_guard<std::mutex> lock(mDeviceStatusMutex);
    mLastTargetUpdate = std::chrono::steady_clock::now();
    mDeviceStatus = MIXING;
    mDeviceStatusCV.notify_one();
  } while(0);

}

void WS2812Strip::WriteCurrentValues() const
{
  // Convert to nybble. 
  std::vector<uint8_t> myBuffer;
  if (mCurrentValues.empty())
    return;
  uint32_t outputSize = mCurrentValues.size() * 4 + LATCHBYTES;
  myBuffer.resize( outputSize );

  uint8_t *output = &myBuffer.at(0);
  const uint8_t *buffer = &mCurrentValues.at(0);

  int outputIdx = 0;

  for (uint32_t byteIdx = 0; byteIdx < mCurrentValues.size(); ++byteIdx) 
  {
    uint8_t byte = buffer[byteIdx];
    for (uint32_t groupIdx = 0; groupIdx < 4; groupIdx++)
    {
      uint8_t encoded = 0x44; // bit-pattern for two zeros
      if (byte & 0x80)
	encoded |= 0x20;
      if (byte & 0x40)
	encoded |= 0x02;
      output[outputIdx++] = encoded;
      byte <<= 2; 
    }
  }

  WriteBitstream(myBuffer);
}

void WS2812Strip::WriteBitstream(const std::vector<uint8_t> &pBitstream) const
{
  struct spi_ioc_transfer tr = { 0 }; 
  tr.tx_buf = (uint64_t) (&pBitstream[0]);
  tr.len = pBitstream.size();

  if (0 > ioctl(mDeviceDescriptor, SPI_IOC_MESSAGE(1), &tr))
  {
    throw std::runtime_error("Error sending message!");
  }
}

void WS2812Strip::Mix(uint8_t pMixRatio)
{
  std::lock_guard<std::mutex> lock(mTargetValuesMutex);
  if (mCurrentValues.size() < mTargetValues.size())
    mCurrentValues.resize(mTargetValues.size());

  int complement = (int) (256 - pMixRatio);

  for (size_t i = 0; i < mTargetValues.size() ; ++i)
  {
    uint32_t m = (uint32_t) mCurrentValues[i] * complement + (uint32_t) mTargetValues[i] * (int)pMixRatio;
    mCurrentValues[i] = (uint8_t) (m >> 8);
  }
}

void WS2812Strip::SetToBlack()
{
  std::lock_guard<std::mutex> lock(mTargetValuesMutex);
  memset(&mTargetValues[0], 0, mTargetValues.size());
}

void WS2812Strip::DeviceLoop()
{
  for (;;)
  {
    std::unique_lock<std::mutex> lock(mDeviceStatusMutex);
    switch(mDeviceStatus)
    {
      case MIXING:
      {
	if (std::chrono::steady_clock::now() - mLastTargetUpdate >= MIX_TIME)	
	{
	  mDeviceStatus = IDLE;
	  SetToBlack();
	}
	// else fall-through
      case IDLE:

	if (std::chrono::steady_clock::now() - mLastTargetUpdate >= IDLE_TIME)
	{
	  // Go to standby.
	  mEnablePin.Disengage();
	  mDeviceStatus = STANDBY;
	  mCurrentValues.clear();
	  break;
	}
	
	lock.unlock();
	Mix( 16 );
	WriteCurrentValues();

	std::this_thread::sleep_for(MIX_DELAY);
	break;
      }
      case STANDBY:
	mDeviceStatusCV.wait( lock, [this] { return mDeviceStatus != STANDBY; });
	if (mDeviceStatus != STOPPING)
	{
	  mEnablePin.Engage();
	}
	break;
      case STOPPING:
	return;
    }
  }
}
