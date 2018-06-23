#ifndef RECT_H
#define RECT_H
#include <iostream>

class AbsoluteRect
{
  public:
    int32_t mLeft, mTop, mRight, mBottom;
};


class RelativeRect
{
  public:
    float mLeft, mTop, mRight, mBottom;
};

inline std::ostream& operator<< (::std::ostream &os, const AbsoluteRect &r)
{
   os << "(" << r.mLeft <<"," << r.mTop <<" - " << r.mRight << "," << r.mBottom << ")";
   return os;
}

inline std::ostream& operator<< (::std::ostream &os, const RelativeRect &r)
{
  std::streamsize originalPrecision = os.precision();
  os.precision(1);

  os << "(" << r.mLeft <<"," << r.mTop <<" - " << r.mRight << "," << r.mBottom << ")";
  
  os.precision(originalPrecision);

  return os;
}

#endif
