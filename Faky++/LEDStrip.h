#ifndef LEDSTRIP_H
#define LEDSTRIP_H

#include <vector>
#include "Colors.h"

class ILEDStrip
{
  public:
    virtual ~ILEDStrip() = default;
    virtual void SetContents(const std::vector<RGBColor> &pValues) = 0;
};
#endif
