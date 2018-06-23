#ifndef IMAGEROUTINES_H
#define IMAGEROUTINES_H

#include "Image.h"
#include "Rect.h"
#include "Colors.h"
#include <string>

namespace ImageRoutines
{
  void SetRect(Image &ioImage, const AbsoluteRect &rect, const RGBColor &color);
  void SetRect(Image &ioImage, const RelativeRect &rect, const RGBColor &color);
  AbsoluteRect ToAbsoluteRect(const Image &iImage, const RelativeRect &iRelativeRect);
  RelativeRect ToRelativeRect(const Image &iImage, const AbsoluteRect &iAbsoluteRect);

  void WritePPM(const Image &iImage, const std::string &iFilename);

  Image ImageFromJPEG(const std::string &iJpegStream);
}
#endif

