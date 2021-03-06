#ifndef COLORS_H
#define COLORS_H
#include <cstdint>
#include <iostream>

class RGBColor 
{
  public:
    uint8_t r,g,b;
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
