#ifndef LEDSTRIP_H
#define LEDSTRIP_H

#include <vector>
#include "Colors.h"

class LEDStrip
{
  public:
    virtual ~LEDStrip() { }
    virtual void SetContents(const std::vector<RGBColor> &pValues) = 0;
};
#endif
