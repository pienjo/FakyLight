#include "LEDSink.h"
#include "LEDStrip.h"

LEDSink::LEDSink(uint32_t iNrSideLeds, uint32_t iNrTopLeds, ILEDStrip &iTargetStrip)
  : mNrSideLeds(iNrSideLeds)
  , mNrTopLeds(iNrTopLeds)
  , mLedStrip(iTargetStrip)
{
  mLedValues.resize(GetNrLeds());
  Clear();
}

void LEDSink::SetColor(const RelativeRect &iRect, const RGBColor &Color)
{
  // LEDS run counter-clockwise; the LED at index [0] sits at the bottom right.
  
  size_t start = GetNrLeds(), stop = 0;

  const size_t topBoundaryStart = mNrSideLeds;
  const size_t leftBoundaryStart = mNrSideLeds + mNrTopLeds;

  if (iRect.mRight > 99) // Along right boundary
  {
    start = std::min(start, topBoundaryStart - (size_t) ( iRect.mBottom * (float) mNrSideLeds / 100. + .5));
    stop  = std::max(stop, topBoundaryStart - (size_t) ( iRect.mTop * (float) mNrSideLeds / 100. + .5));
  }

  if (iRect.mTop < 1) // Along top boundary
  {
    start = std::min(start, leftBoundaryStart - (size_t) ( iRect.mRight * (float) mNrTopLeds / 100. + .5)); 
    stop  = std::max(stop, leftBoundaryStart - (size_t) ( iRect.mLeft * (float) mNrTopLeds / 100. + .5));
  }

  if (iRect.mLeft  < 1 ) // Along left boundary
  {
    start = std::min(start, (size_t) ( iRect.mTop * (float) mNrSideLeds / 100. + .5) + leftBoundaryStart);
    stop  = std::max(stop,  (size_t) ( iRect.mBottom * (float) mNrSideLeds / 100. +.5) + leftBoundaryStart);
  }

  for (size_t i = start; i < stop; ++i)
  {
    mLedValues[i] = Color * mGain;
  }
}

void LEDSink::Flush()
{
  mLedStrip.SetContents(mLedValues);
  Clear();
}

uint32_t LEDSink::GetNrLeds() const
{
  return 2 * mNrSideLeds + mNrTopLeds;  
}

void LEDSink::Clear()
{
  for(uint32_t i = 0; i < GetNrLeds(); ++i)
  {
    mLedValues[i] = { 0, 0, 0};
  }
}

 void LEDSink::setGain(const RGBGain &gain)  
 {
   mGain = gain;
 }
