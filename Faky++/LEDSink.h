#ifndef LEDSINK_H
#define LEDSINK_H

#include <vector>
#include "ColorSink.h"
#include "Colors.h"
#include "LEDStrip.h"
#include "WhiteBalanceCorrection.h"

class LEDStrip;

class LEDSink : public ColorSink, public WhitebalaceCorrection
{
  public:
    LEDSink(uint32_t iNrSideLeds, uint32_t iNrTopLeds, ILEDStrip &iTargetStrip);
  
    void SetColor(const RelativeRect &iRect, const RGBColor &Color) override;
    void Flush() override;
    void setGain(const RGBGain &gain) override;

  private:
    uint32_t GetNrLeds() const;
    void Clear();

    uint32_t mNrSideLeds, mNrTopLeds;
    RGBGain mGain;
    std::vector<RGBColor> mLedValues;
    ILEDStrip &mLedStrip;
};

#endif
