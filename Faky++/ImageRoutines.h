#ifndef IMAGEROUTINES_H
#define IMAGEROUTINES_H

#include "Image.h"
#include "Rect.h"
#include "ColorRoutines.h"

namespace ImageRoutines
{
  void SetRect(Image &ioImage, const Rect &rect, const ColorRoutines::RGBColor &color);
}
#endif

