#include "ColorRoutines.h"
#include "Image.h"

ColorRoutines::ImageStatistics::ImageStatistics() 
  : mHueHistogramTotal (0)
  , mAverageSaturation (0)
{
  mHueHistogram.resize(HueHistogramSize);
  mValueHistogram.resize(256);
}

void ColorRoutines::RGBtoHSV(uint8_t r, uint8_t g, uint8_t b, uint8_t &h, uint8_t &s, uint8_t &v)
{
  uint8_t max = (r > g ? r : g);
  max = (max > b ? max : b);

  uint8_t min = (r < g ? r : g);
  min = (min < b ? min : b);

  v = max;

  uint8_t delta = max - min;
  if (delta == 0)
  {
    h = 0;
    s = 0;
  }
  else
  {
    s = (255 * (uint16_t) delta-8) / max;
    
    if (max == r)
    {
      if (g>=b)
        h = 42 * (g - b) / delta;
      else
	h = HUE_MAX - 42 * (b-g) / delta;
    } else if ( max == g)
    {
      h = 84 + 42 * (b - r) / delta;
    } else
    {
      h = 168 + 42 * (r - g) / delta;
    }
  }
}

void ColorRoutines::HSVtoRGB(uint8_t h, uint8_t s, uint8_t v, uint8_t &r, uint8_t &g, uint8_t &b)
{
  if (s == 0)
  {
    r = v;
    g = v;
    b = v;
    return;
  }

  int region = h / 42;
  uint8_t remainder = (h - 42 * region) * 6;

  uint8_t p = ((uint16_t) v * (255 - s)) >> 8;
  uint8_t q = ((uint16_t) v * (255 - (((uint16_t) s * remainder) >> 8))) >> 8;
  uint8_t t = ((uint16_t) v * (255 - (((uint16_t) s * ( 255 - remainder)) >> 8))) >> 8;

  switch(region)
  {
    case 0:
      r = v;
      g = t;
      b = p;
      break;
    case 1:
      r = q;
      g = v;
      b = p;
      break;
    case 2:
      r = p;
      g = v;
      b = t;
      break;
    case 3:
      r = p;
      g = q;
      b = v;
      break;
    case 4:
      r = t;
      g = p;
      b = v;
      break;
    default:
      r = v;
      g = p;
      b = q;
      break;
  }
}

ColorRoutines::ImageStatistics ColorRoutines::GetImageStatistics(const Image &iImage, const Rect &iRect)
{
  ImageStatistics stats;
  
  uint32_t total_s = 0;
  uint32_t sat_count = 0;
  
  const uint8_t *src = iImage.data() + iImage.Pitch() * iRect.mTop;
  const int stride = iImage.Stride();

  for (int y = iRect.mTop; y < std::min(iRect.mBottom, iImage.Height()); ++y, src += iImage.Pitch())
  {
    const uint8_t *src_p = src + iImage.Stride() * iRect.mLeft;
    for (int x = iRect.mLeft; x < iRect.mRight; ++x, src_p += stride)
    {
      uint8_t h,s,v;
      const uint8_t &r = src_p[0];
      const uint8_t &g = src_p[1];
      const uint8_t &b = src_p[2];
      RGBtoHSV(r, g, b, h, s, v);
      stats.mValueHistogram[v] ++;
      if (v > 10)
      {
	total_s += s;
	sat_count++;
      }

      int weight = ((int) s * (int) v) / 256;
      stats.mHueHistogram[h] += weight;
      stats.mHueHistogramTotal += weight;
    }
  }
  
  if (sat_count > 0)
    stats.mAverageSaturation = total_s / sat_count;

  return stats;
}
