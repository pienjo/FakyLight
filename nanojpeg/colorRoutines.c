#include "colorRoutines.h"
#include "imageType.h"

void RGBtoHSV(uint8_t r, uint8_t g, uint8_t b, uint8_t *h, uint8_t *s, uint8_t *v)
{
  uint8_t max = (r > g ? r : g);
  max = (max > b ? max : b);

  uint8_t min = (r < g ? r : g);
  min = (min < b ? min : b);

  *v = max;

  uint8_t delta = max - min;
  if (delta == 0)
  {
    *h = 0;
    *s = 0;
  }
  else
  {
    *s = (255 * (uint16_t) delta-8) / max;
    
    if (max == r)
    {
      if (g>=b)
        *h = 42 * (g - b) / delta;
      else
	*h = HUE_MAX - 42 * (b-g) / delta;
    } else if ( max == g)
    {
      *h = 84 + 42 * (b - r) / delta;
    } else
    {
      *h = 168 + 42 * (r - g) / delta;
    }
  }
}

void HSVtoRGB(uint8_t h, uint8_t s, uint8_t v, uint8_t *r, uint8_t *g, uint8_t *b)
{
  if (s == 0)
  {
    *r = v;
    *g = v;
    *b = v;
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
      *r = v;
      *g = t;
      *b = p;
      break;
    case 1:
      *r = q;
      *g = v;
      *b = p;
      break;
    case 2:
      *r = p;
      *g = v;
      *b = t;
      break;
    case 3:
      *r = p;
      *g = q;
      *b = v;
      break;
    case 4:
      *r = t;
      *g = p;
      *b = v;
      break;
    default:
      *r = v;
      *g = p;
      *b = q;
      break;
  }
}

uint32_t getHueValueHistogram(struct image *img, uint32_t *histogram, uint32_t *valueHistogram, uint8_t *avgSat, int left, int top, int right, int bottom)
{
  uint32_t total = 0;
  uint32_t total_s = 0;
  uint32_t sat_count = 0;

  for (int y = top; y < bottom; ++y)
  {
    const uint8_t *src = img->data + 3 *(y * img->width + left);
    for (int x = left; x < right; ++x)
    {
      uint8_t h,s,v;
      RGBtoHSV(*src++, *src++, *src++, &h, &s, &v);
      valueHistogram[v] ++;
      if (v > 10)
      {
	total_s += s;
	sat_count++;
      }

      int weight = ((int) s * (int) v) / 256;
      histogram[h] += weight;
      total += weight;
    }
  }
  
  if (sat_count > 0)
    *avgSat = total_s / sat_count;
  else
    *avgSat = 0;

  return total;
}
