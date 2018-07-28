#ifndef WS2812STRIP_H
#define WS2812STRIP_H

#include "LEDStrip.h"

class WS2812Strip : public ILEDStrip
{
  public:
    WS2812Strip(const std::string &pDeviceName);
    ~WS2812Strip();

    void SetContents(const std::vector<RGBColor> &pValues) override;
  private:
    void WriteBitstream(const std::vector<uint8_t> &pBitstream) const;

    int mDeviceDescriptor;

};
#endif
