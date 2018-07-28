#include "MixingStrip.h"

using namespace std::chrono_literals;

#define MIX_TIME 1s
#define IDLE_TIME 30s
#define MIX_DELAY 20ms

MixingStrip::MixingStrip(ILEDStrip &pRealStrip, IPin &pEnablePin)
  : mEnablePin(pEnablePin)
  , mRealStrip(pRealStrip)
{
  mEnablePin.Disengage();

  mDeviceStatus = STANDBY;
  mDeviceThread = std::thread([this]{ DeviceLoop(); } );
}

MixingStrip::~MixingStrip()
{
  do {
    std::lock_guard<std::mutex> lock(mDeviceStatusMutex);
    mDeviceStatus = STOPPING;
    mDeviceStatusCV.notify_one();
  } while(0);

  mDeviceThread.join();
  mEnablePin.Disengage();
}

void MixingStrip::SetContents(const std::vector<RGBColor> &pValues)
{
  do {
    std::lock_guard<std::mutex> lock(mTargetValuesMutex);
    mTargetValues = pValues;
  } while(0);

  do {
    std::lock_guard<std::mutex> lock(mDeviceStatusMutex);
    mLastTargetUpdate = std::chrono::steady_clock::now();
    mDeviceStatus = MIXING;
    mDeviceStatusCV.notify_one();
  } while(0);
}

void MixingStrip::Mix(uint8_t pMixRatio)
{
  std::lock_guard<std::mutex> lock(mTargetValuesMutex);
  if (mCurrentValues.size() < mTargetValues.size())
    mCurrentValues.resize(mTargetValues.size());

  uint32_t ratio = (uint32_t) pMixRatio;
  uint32_t complement = (uint32_t) (256 - pMixRatio);
 
  auto doMix = [ratio, complement](uint32_t component1, uint32_t component2) -> uint8_t
  {
    uint32_t m = component1 * complement + component2 * ratio;
    return m>>8;
  };

  for (size_t i = 0; i < mTargetValues.size() ; ++i)
  {
    const RGBColor &target = mTargetValues[i];
    RGBColor &current = mCurrentValues[i];

    current.r = doMix(current.r, target.r);
    current.g = doMix(current.r, target.r);
    current.g = doMix(current.r, target.r);
  }
}

void MixingStrip::SetToBlack()
{
  std::lock_guard<std::mutex> lock(mTargetValuesMutex);

  for (RGBColor &c : mTargetValues)
    c = {0,0,0};
}

void MixingStrip::DeviceLoop()
{
  for (;;)
  {
    std::unique_lock<std::mutex> lock(mDeviceStatusMutex);
    switch(mDeviceStatus)
    {
      case MIXING:
      {
	if (std::chrono::steady_clock::now() - mLastTargetUpdate >= MIX_TIME)	
	{
	  mDeviceStatus = IDLE;
	  SetToBlack();
	}
	// else fall-through
      case IDLE:

	if (std::chrono::steady_clock::now() - mLastTargetUpdate >= IDLE_TIME)
	{
	  // Go to standby.
	  mEnablePin.Disengage();
	  mDeviceStatus = STANDBY;
	  mCurrentValues.clear();
	  break;
	}
	
	lock.unlock();
	Mix( 16 );
	mRealStrip.SetContents(mCurrentValues);

	std::this_thread::sleep_for(MIX_DELAY);
	break;
      }
      case STANDBY:
	mDeviceStatusCV.wait( lock, [this] { return mDeviceStatus != STANDBY; });
	if (mDeviceStatus != STOPPING)
	{
	  mEnablePin.Engage();
	}
	break;
      case STOPPING:
	return;
    }
  }
}
