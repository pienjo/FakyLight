#ifndef IMAGEROUTINES_H
#define IMAGEROUTINES_H

#include "Image.h"
#include "Rect.h"
#include "ColorRoutines.h"

namespace ImageRoutines
{
  void SetRect(Image &ioImage, const AbsoluteRect &rect, const ColorRoutines::RGBColor &color);
  void SetRect(Image &ioImage, const RelativeRect &rect, const ColorRoutines::RGBColor &color);
  AbsoluteRect ToAbsoluteRect(const Image &iImage, const RelativeRect &iRelativeRect);
  RelativeRect ToRelativeRect(const Image &iImage, const AbsoluteRect &iAbsoluteRect);
}
#endif

