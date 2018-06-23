#include "RoapSource.h"
#include "ProcessingRoutines.h"
#include "UDPStrip.h"
#include "LEDSink.h"
#include <atomic>

int main(void)
{
  RoapSource source("192.168.64.223", 855905);
  UDPStrip strip("192.168.64.27", 5628);
  LEDSink sink( 14, 25, strip);

  return 0;
}
