#include "ColorRoutines.h"
#include "Image.h"
#include "ImageRoutines.h"

ColorRoutines::ImageStatistics::ImageStatistics() 
  : mHueHistogramTotal (0)
  , mAverageSaturation (0)
{
  mHueHistogram.resize(HueHistogramSize);
  mValueHistogram.resize(256);
}

HSVColor ColorRoutines::RGBtoHSV(const RGBColor &rgb)
{
  HSVColor hsv;

  uint8_t max = (rgb.r > rgb.g ? rgb.r : rgb.g);
  max = (max > rgb.b ? max : rgb.b);

  uint8_t min = (rgb.r < rgb.g ? rgb.r : rgb.g);
  min = (min < rgb.b ? min : rgb.b);

  hsv.v = max;

  uint8_t delta = max - min;
  if (delta == 0)
  {
    hsv.h = 0;
    hsv.s = 0;
  }
  else
  {
    hsv.s = (255 * (uint16_t) delta-8) / max;
    
    if (max == rgb.r)
    {
      if (rgb.g>=rgb.b)
        hsv.h = 42 * (rgb.g - rgb.b) / delta;
      else
	hsv.h = HSVColor::HUE_MAX - 42 * (rgb.b-rgb.g) / delta;
    } else if ( max == rgb.g)
    {
      hsv.h = 84 + 42 * (rgb.b - rgb.r) / delta;
    } else
    {
      hsv.h = 168 + 42 * (rgb.r - rgb.g) / delta;
    }
  }

  return hsv;
}

RGBColor ColorRoutines::HSVtoRGB( const HSVColor &hsv)
{

  if (hsv.s == 0)
  {
    return {hsv.v, hsv.v, hsv.v };
  }

  int region = hsv.h / 42;
  uint8_t remainder = (hsv.h - 42 * region) * 6;

  uint8_t p = ((uint16_t) hsv.v * (255 - hsv.s)) >> 8;
  uint8_t q = ((uint16_t) hsv.v * (255 - (((uint16_t) hsv.s * remainder) >> 8))) >> 8;
  uint8_t t = ((uint16_t) hsv.v * (255 - (((uint16_t) hsv.s * ( 255 - remainder)) >> 8))) >> 8;

  switch(region)
  {
    case 0:
      return { hsv.v,t, p };
    case 1:
      return {q, hsv.v, p };
    case 2:
      return {p,hsv.v,t };
    case 3:
      return {p,q, hsv.v};
    case 4:
      return {t, p, hsv.v};
    default:
      break;
  }
  return {hsv.v,p,q };
}

ColorRoutines::ImageStatistics ColorRoutines::GetImageStatistics(const Image &iImage, const RelativeRect &iRect)
{
  AbsoluteRect absoluteRect = ImageRoutines::ToAbsoluteRect(iImage, iRect);

  return GetImageStatistics(iImage, absoluteRect);
}

ColorRoutines::ImageStatistics ColorRoutines::GetImageStatistics(const Image &iImage, const AbsoluteRect &iRect)
{
  ImageStatistics stats;
  
  uint32_t total_s = 0;
  uint32_t sat_count = 0;
  
  const uint8_t *src = iImage.data() + iImage.Pitch() * iRect.mTop;
  const int stride = iImage.Stride();

  for (int y = iRect.mTop; y < std::min(iRect.mBottom, (int32_t) iImage.Height()); ++y, src += iImage.Pitch())
  {
    const uint8_t *src_p = src + iImage.Stride() * iRect.mLeft;
    for (int x = iRect.mLeft; x < iRect.mRight; ++x, src_p += stride)
    {
      const RGBColor rgb={src_p[0], src_p[1], src_p[2]};
      HSVColor hsv = RGBtoHSV(rgb);

      stats.mValueHistogram[hsv.v] ++;
      if (hsv.v > 10)
      {
	total_s += hsv.s;
	sat_count++;
      }

      int weight = ((int) hsv.s * (int) hsv.v) / 256;
      stats.mHueHistogram[hsv.h] += weight;
      stats.mHueHistogramTotal += weight;
    }
  }
  
  if (sat_count > 0)
    stats.mAverageSaturation = total_s / sat_count;

  return stats;
}
