#include <stdio.h>
#include <unistd.h>

#include "WS2812Strip.h"


RGBColor Colors[]  = { { 0x00, 0x00, 0x00 }, // black 
		       { 0x00, 0x00, 0xff }, // Blue
		       { 0x00, 0xff, 0x00 }, // Red
		       { 0x00, 0xff, 0xff }, // magenta
		       { 0xff, 0x00, 0x00 }, // Blue
		       { 0xff, 0x00, 0xff }, // cyan
		       { 0xff, 0xff, 0x00 },// yellow
		       { 0xff, 0xff, 0xff },// white 
		     };

int main(void)
{
  WS2812Strip strip("/dev/spidev1.0");
  
  std::vector<RGBColor> data;
  data.resize(56);

  auto fill = [&data] (const RGBColor &color)
  {
    for( RGBColor &c : data)
    {
      c = color; 
    }
  };

  size_t colorIndex = 0;

  for (;;)
  {
    fill(Colors[colorIndex]);
    colorIndex = colorIndex + 1;
    if (colorIndex > (sizeof(Colors) / sizeof(Colors[0])))
      colorIndex = 0;

    for (int i = 0; i < 200; ++i)
    {
      strip.SetContents(data);
      usleep(10000);
    }
  }
}
