#ifndef MIXINGSTRIP_H
#define MIXINGSTRIP_H

#include "LEDStrip.h"
#include "IPin.h"

#include <thread>
#include <mutex>
#include <condition_variable>

class MixingStrip : public ILEDStrip
{
  public:
    MixingStrip(ILEDStrip &pRealStrip, IPin &pEnablePin);
    ~MixingStrip();

    void SetContents(const std::vector<RGBColor> &pValues) override;
  private:
    std::vector<RGBColor> mCurrentValues;

    std::mutex mTargetValuesMutex;
    std::vector<RGBColor> mTargetValues;
    std::chrono::steady_clock::time_point mLastTargetUpdate;

    void Mix(uint8_t pMixRatio);

    void WriteCurrentValues() const;
    
    void SetToBlack();
    
    enum deviceStatus 
    {
      MIXING,
      IDLE,
      STANDBY,
      STOPPING,
    } mDeviceStatus;
    std::mutex mDeviceStatusMutex;
    std::condition_variable mDeviceStatusCV;

    void DeviceLoop();
    std::thread mDeviceThread;
    
    IPin &mEnablePin;
    ILEDStrip &mRealStrip;
};

#endif
