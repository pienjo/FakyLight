#include "ImageRoutines.h"
#include <stdio.h>
#include <algorithm>

void ImageRoutines::SetRect(Image &ioImage, const Rect &iRect, const ColorRoutines::RGBColor &iColor)
{
  uint8_t *d = ioImage.data();
  
  for (int32_t y = std::max(0, (int32_t) iRect.mTop); y < std::min(iRect.mBottom, (int32_t) ioImage.Height()); ++y)
  {
    uint8_t *d_r = d + y * ioImage.Pitch();
    for (int32_t x = std::max(0, (int32_t) iRect.mLeft); x < std::min(iRect.mRight, (int32_t) ioImage.Width()); ++x)
    {
      d_r[3*x] = iColor.r;
      d_r[3*x+1] = iColor.g;
      d_r[3*x+2] = iColor.b;
    }
  }
}
