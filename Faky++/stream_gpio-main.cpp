#include "RoapSource.h"
#include "ProcessingRoutines.h"
#include "LEDSink.h"
#include "Scheduler.h"
#include "WS2812Strip.h"
#include "MixingStrip.h"
#include "GPIOPin.h"

int main(void)
{
  RoapSource source("192.168.64.233", 855905);
  GPIOPin enablePin("/dev/gpiochip0", 3);
  WS2812Strip strip("/dev/spidev1.0");
  MixingStrip mixer(strip, enablePin);
  LEDSink sink( 14, 25, strip);
  Scheduler scheduler(source, sink);

  scheduler.Run();

  printf("Uh-oh\n");
  return 0;
}
