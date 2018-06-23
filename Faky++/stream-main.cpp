#include "RoapSource.h"
#include "ProcessingRoutines.h"
#include "UDPStrip.h"
#include "LEDSink.h"
#include "Scheduler.h"

int main(void)
{
  RoapSource source("192.168.64.223", 855905);
  UDPStrip strip("192.168.64.27", 5628);
  LEDSink sink( 14, 25, strip);
  Scheduler scheduler(source, sink);

  scheduler.Run();
  return 0;
}
