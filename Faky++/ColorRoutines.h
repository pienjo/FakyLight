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

  typedef std::vector<uint32_t> histogramT;
  
  void RGBtoHSV(const uint8_t r, const uint8_t g, const uint8_t b, uint8_t &h, uint8_t &s, uint8_t &v);
  void HSVtoRGB(const uint8_t h, const uint8_t s, const uint8_t v, uint8_t &r, uint8_t &g, uint8_t &b);
  
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
