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

WS2812Strip::WS2812Strip(const std::string &pDeviceName)
  : mDeviceDescriptor(-1)
{
  mDeviceDescriptor = open(pDeviceName.c_str(), O_RDWR);
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
}

WS2812Strip::~WS2812Strip()
{
  if (mDeviceDescriptor >= 0)
  {
    close(mDeviceDescriptor);
    mDeviceDescriptor = -1;
  }
}

void WS2812Strip::SetContents(const std::vector<RGBColor> &pValues) 
{
  std::vector<uint8_t> buf;
  buf.reserve(pValues.size() * 3 * 4 + LATCHBYTES + 1);
 
  buf.push_back(0); // dummy byte, allowing SPI clock to settle

  auto encodeByte = [&buf] ( uint8_t byte) 
  {  
    for (uint32_t groupIdx = 0; groupIdx < 4; groupIdx++)
    {
      uint8_t encoded = 0x44; // bit-pattern for two zeros
      if (byte & 0x80)
	encoded |= 0x20;
      if (byte & 0x40)
	encoded |= 0x02;

      buf.push_back(encoded);
      byte <<= 2; 
    }
  };

  for (const RGBColor &c : pValues)
  {
    encodeByte(c.g);
    encodeByte(c.r);
    encodeByte(c.b);
  }
  
  for (int i = 0; i < LATCHBYTES; ++i)
    buf.push_back(0);

  WriteBitstream(buf);
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


