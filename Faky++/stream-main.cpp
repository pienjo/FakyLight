#include "RoapSource.h"
#include "ProcessingRoutines.h"
#include "UDPStrip.h"
#include "LEDSink.h"
#include "Scheduler.h"

int main(void)
{
  RoapSource source("192.168.64.5", 855905);
  UDPStrip strip("192.168.64.27", 5628);
  LEDSink sink( 28, 50, strip);
  sink.setGain(RGBGain{0.5f, 0.5f, 0.5});
  Scheduler scheduler(source, sink);

  scheduler.Run();

  printf("Uh-oh\n");
  return 0;
}
