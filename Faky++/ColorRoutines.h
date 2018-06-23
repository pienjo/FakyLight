#ifndef COLORROUTINES_H
#define COLORROUTINES_H

#include <vector>
#include "Image.h"
#include "Rect.h"
#include "Colors.h"

namespace ColorRoutines
{
  typedef std::vector<uint32_t> histogramT;
  
  HSVColor RGBtoHSV(const RGBColor &rgbColor);
  RGBColor HSVtoRGB(const HSVColor &hsvColor);
  
  class ImageStatistics
  {
    public:
      ImageStatistics();
      
      enum {
	HueHistogramSize = (HSVColor::HUE_MAX+1)
      };
      histogramT mHueHistogram;
      uint32_t mHueHistogramTotal;
      histogramT mValueHistogram;
      uint8_t mAverageSaturation;
  };

  ImageStatistics GetImageStatistics(const Image &iImage, const AbsoluteRect &iRect);
  ImageStatistics GetImageStatistics(const Image &iImage, const RelativeRect &iRect);
};

#endif
