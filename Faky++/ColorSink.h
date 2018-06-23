#ifndef COLORSINK_H
#define COLORSINK_H
#include "Colors.h"
#include "Rect.h"

class ColorSink
{
  public:
    virtual ~ColorSink() { } 
    virtual void SetColor(const RelativeRect &iRect, const RGBColor &iColor) = 0;
    virtual void Flush() = 0;
};

#endif
