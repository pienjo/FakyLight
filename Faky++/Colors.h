#ifndef COLORS_H
#define COLORS_H
#include <cstdint>
#include <iostream>

struct RGBGain
{
  RGBGain(float rGain, float gGain, float bGain)
    : rGain{rGain}
    , gGain{gGain}
    , bGain{bGain}
    {
    
    }
  RGBGain() = default;
  RGBGain(const RGBGain &rhs) = default;

  float rGain {1.};
  float gGain {1.};
  float bGain {1.};
};

class RGBColor 
{
  public:
    uint8_t r,g,b;

    RGBColor operator*(const RGBGain &gains) const
    {
      RGBColor result{};
      result.r = (uint8_t) std::max( 0.f, std::min( (float)r * gains.rGain, 255.f));
      result.g = (uint8_t) std::max( 0.f, std::min( (float)g * gains.gGain, 255.f));
      result.b = (uint8_t) std::max( 0.f, std::min( (float)b * gains.bGain, 255.f));

      return result;
    }

    bool operator==(const RGBColor &other) const
    {
      return r == other.r && b == other.b && g == other.g;
    }

    bool operator!=(const RGBColor &other) const
    {
      return !(other == *this);
    }
};

class HSVColor
{
  public:
    uint8_t h,s,v;
    
    bool operator==(const HSVColor &other) const
    {
      return h == other.h && s == other.s && v == other.v;
    }

    bool operator!=(const HSVColor &other) const
    {
      return !(other == *this);
    }
    
    enum 
    {
      HUE_MAX = 251
    };
};

inline std::ostream & operator<< (::std::ostream &os, const RGBColor &d)
{
  os << "(" << (int)d.r << "," << (int)d.g << "," << (int)d.b <<")";
  return os;
}

inline std::ostream & operator<< (::std::ostream &os, const HSVColor &d)
{
  os << "(" << (int)d.v << "," << (int)d.s << "," << (int)d.v <<")";
  return os;
}

#endif
