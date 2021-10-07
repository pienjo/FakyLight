#ifndef WHITEBALANCECORRECTION_H__ 
#define WHITEBALANCECORRECTION_H__

struct RGBGain;

class WhitebalaceCorrection
{
public:
  virtual void setGain(const RGBGain &gain) = 0;
};

#endif