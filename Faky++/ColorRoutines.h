#ifndef COLORROUTINES_H
#define COLORROUTINES_H

#include <vector>
#include "Image.h"
#include "Rect.h"

namespace ColorRoutines
{
  enum 
  {
    HUE_MAX = 252
  };
  
  struct RGBColor 
  {
    uint8_t r,g,b;
  };

  struct HSVColor
  {
    uint8_t h,s,v;
  };
  
  typedef std::vector<uint32_t> histogramT;
  
  HSVColor RGBtoHSV(const RGBColor &rgbColor);
  RGBColor HSVtoRGB(const HSVColor &hsvColor);
  
  class ImageStatistics
  {
    public:
      ImageStatistics();
      
      enum {
	HueHistogramSize = (HUE_MAX+1)
      };
      histogramT mHueHistogram;
      uint32_t mHueHistogramTotal;
      histogramT mValueHistogram;
      uint8_t mAverageSaturation;
  };

  ImageStatistics GetImageStatistics(const Image &iImage, const Rect &iRect);
};

#endif
