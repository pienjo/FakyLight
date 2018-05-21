#include <stdexcept>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include <inttypes.h>
#include <vector>

const char *const devicename = "/dev/spidev1.0";
class spiDevice {
  public:
    spiDevice();
    ~spiDevice();

    void WriteBuffer( const uint8_t *buffer, const uint32_t size);

  private:
    int mFileDescriptor;
  /* Determination of transfer speed:
     
     Max pulse time for a 0-bit :  500 ns
     Min pulse time for a 1-bit :  550 ns

     Min space between successive bits: 450 ns

     To transfer using 4 bit chunks, ( 1-pulse = 1100, 0-pulse = 1000 ) the pulse time must be no lower than
       550/2 = 275 ns -> Max bitrate of 3.636 MHz. 
     Minimum pulse time = 500 ns -> Max bitrate of 2 MHz. 

     Fortunately, the OrangePi's SPI module seems to have a valid bitrate at 3 MHz. This yields the folowing times:

     0-bit: Written using 1000 nybble
	    T0H = 333 ns ( within spec, < 500)
	    T0L = 999 ns ( within simplified spec: > 450 ns, and < 5000 ns
     1-bit: Written using 1100 nybble
	    T1H = 666 ns ( witih spec, > 550 )
	    T1L = 666 ns ( within simplified spec: > 450 ns, and < 5000 ns

     Due to the unreliable timing of the Linux SPI module (it can stretch the first bit in a byte..) 
     it is more reliable to move the 1-bits to the middle of the nybble (so the space actually gets divided
     between to successive nybbles): So write as 0100 for a 0-bit, and as 0110 for a 1-bit. 

     At this speed, the minimum latch time (6000 ns) boils down to 18 bits - rounded up to 3 bytes.
  */
  enum :uint32_t {
    bitRate = 2000000,
    latchBytes = 3} ;
};

spiDevice::spiDevice() :
  mFileDescriptor(-1)
{
  mFileDescriptor = open(devicename, O_RDWR);
  if (mFileDescriptor < 0)
  {
    throw std::runtime_error("Unable to open file");
  }
  
  // Setup device

  uint8_t mode = SPI_MODE_3;
  uint8_t bitsPerWord = 8;
  uint32_t speed = bitRate;

  if (0 != ioctl(mFileDescriptor, SPI_IOC_WR_MODE, &mode))
    throw std::runtime_error("Unable to set write mode");
  
  if (0 != ioctl(mFileDescriptor, SPI_IOC_WR_BITS_PER_WORD, &bitsPerWord))
    throw std::runtime_error("Unable to set bits per word");
 
  if (0 != ioctl(mFileDescriptor, SPI_IOC_WR_MAX_SPEED_HZ, &speed))
    throw std::runtime_error("Unable to write speed");

}

spiDevice::~spiDevice() 
{
  if (mFileDescriptor >= 0)
  {
    close(mFileDescriptor);
    mFileDescriptor = -1;
  }
}

void spiDevice::WriteBuffer( const uint8_t *buffer, const uint32_t size)
{
  // Convert to nybble. 
  std::vector<uint8_t> myBuffer;
  uint32_t outputSize = size * 4 + latchBytes;
  myBuffer.resize( outputSize );

  uint8_t *output = &myBuffer.at(0);
  int outputIdx = 0;

  for (uint32_t byteIdx = 0; byteIdx < size; ++byteIdx) 
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

  struct spi_ioc_transfer tr = { 0 }; 
    tr.tx_buf = (uint64_t) output;
    tr.len = outputSize;

  if (0 > ioctl(mFileDescriptor, SPI_IOC_MESSAGE(1), &tr))
  {
    perror("waa");
    throw std::runtime_error("Error sending message!");
  }

}

int main(void)
{
  int nrLeds = 68;

  uint8_t colorBuffer[nrLeds * 3]= { 0x00, 0x00, 0x00, // black 
			  0x00, 0x00, 0xff, // Blue
			  0x00, 0xff, 0x00, // Red
			  0x00, 0xff, 0xff,  // magenta
			  0xff, 0x00, 0x00, // Blue
			  0xff, 0x00, 0xff, // cyan
			  0xff, 0xff, 0x00,// yellow
			  0xff, 0xff, 0xff };// white 
  spiDevice myDevice;
  for(;;)
  {
    myDevice.WriteBuffer( colorBuffer, sizeof(colorBuffer));
    uint8_t t_g = colorBuffer[(nrLeds -1) * 3]    ,
	    t_r = colorBuffer[(nrLeds -1) * 3 + 1] ,
	    t_b = colorBuffer[(nrLeds -1) * 3 + 2] ;
    uint8_t *p = colorBuffer + sizeof(colorBuffer) - 1;
    uint8_t *q = p - 3;

    for (int i = 0; i < nrLeds ; i++)
    {
      *p-- = *q--;
      *p-- = *q--;
      *p-- = *q--;
    }
    colorBuffer[0] = t_g;
    colorBuffer[1] = t_r;
    colorBuffer[2] = t_b;
    usleep(50000);
  }

  return 0;
}
