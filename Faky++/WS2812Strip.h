#ifndef WS2812STRIP_H
#define WS2812STRIP_H

#include "LEDStrip.h"
#include <thread>
#include <mutex>
#include <condition_variable>

class WS2812Strip : public LEDStrip
{
  public:
    WS2812Strip();
    ~WS2812Strip();

    void SetContents(const std::vector<RGBColor> &pValues) override;
  private:
  
    void Mix(uint8_t pMixRatio);
    void WriteCurrentValues() const;
    void WriteBitstream(const std::vector<uint8_t> &pBitstream) const;

    void SetToBlack();

    void DeviceLoop();
    
    std::vector<uint8_t> mCurrentValues;

    std::mutex mTargetValuesMutex;
    std::vector<uint8_t> mTargetValues;
   
    std::chrono::steady_clock::time_point mLastTargetUpdate;

    enum deviceStatus 
    {
      MIXING,
      IDLE,
      STANDBY,
      STOPPING,
    } mDeviceStatus;
    
    std::mutex mDeviceStatusMutex;
    std::condition_variable mDeviceStatusCV;

    std::thread mDeviceThread;
    int mDeviceDescriptor;
};
#endif
