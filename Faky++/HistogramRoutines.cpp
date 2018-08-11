#include "HistogramRoutines.h"

using HistogramRoutines::ImageStatistics;

void HistogramRoutines::SmoothHueHistogram(ImageStatistics &ioStatistics, const std::vector<uint32_t> &iKernel)
{
  ColorRoutines::histogramT outputHistogram;
  int kernelSize = iKernel.size();

  outputHistogram.resize(ImageStatistics::HueHistogramSize);
  
  for (size_t idx = 0; idx < ImageStatistics::HueHistogramSize; ++idx)
  {
    size_t leftIdx = idx;
    size_t rightIdx = idx;
    
    uint32_t v = ioStatistics.mHueHistogram[idx];
    
    outputHistogram[idx] += iKernel[0] * v;

    for (int k = 1; k < kernelSize; ++k)
    {
      leftIdx = (leftIdx > 0) ? leftIdx - 1 : ImageStatistics::HueHistogramSize - 1;
      rightIdx = (rightIdx < ImageStatistics::HueHistogramSize - 1) ? rightIdx + 1 : 0;
      
      outputHistogram[leftIdx] += iKernel[k] * v;
      outputHistogram[rightIdx] += iKernel[k] * v;
    }
  }
  
  uint32_t totalKernelWeight = 0;
  for (uint32_t k : iKernel)
  {
    totalKernelWeight += k*2;
  }
  
  totalKernelWeight -= iKernel[0];

  for (size_t idx = 0; idx < ImageStatistics::HueHistogramSize; ++idx)
  {
    ioStatistics.mHueHistogram[idx] = (outputHistogram[idx] + totalKernelWeight/2) / totalKernelWeight;
  }
}

size_t HistogramRoutines::GetMode_Hue(const ImageStatistics &iStatistics)
{
  uint32_t modeValue = iStatistics.mHueHistogram[0];
  size_t modeIndex = 0;
  
  for (size_t i = 1; i < ImageStatistics::HueHistogramSize; ++i)
  {
    if (iStatistics.mHueHistogram[i] > modeValue)
    {
      modeValue = iStatistics.mHueHistogram[i];
      modeIndex = i;
    }
  }

  return modeIndex;
}

size_t HistogramRoutines::GetModal_Value(const ImageStatistics &iStatistics)
{
  uint32_t total = 0;
  for (const uint32_t v : iStatistics.mValueHistogram)
    total += v;

  const uint32_t target_value = total / 2;
  
  total = 0;

  for (size_t i = 0; i < 256; ++i)
  {
    total += iStatistics.mValueHistogram[i];
    if (total >= target_value)
      return i;
  }

  return 0; // unreached
}

void HistogramRoutines::ClearLowerValueBuckets(ImageStatistics &ioStatistics, size_t nrBuckets)
{
  for (size_t i = 0; i < nrBuckets && i < 256; ++i)
  {
    ioStatistics.mValueHistogram[i] = 0;
  }
};

HSVColor HistogramRoutines::GetDominantColor(const ImageStatistics &statistics)
{
  HSVColor hsvColor = { 0, 0, 0 };
  
  // The hue is the mode of the hue histogram
  hsvColor.h = GetMode_Hue(statistics);
   
  // The magic formula: The ratio of the surface of the mode bin and the total surface of the hue histogram, multiplied
  // by the average saturation.

  if (statistics.mHueHistogramTotal > 0)
  {
    uint32_t saturation = 256 * GetHueBucketSum(statistics, hsvColor.h, 7) / statistics.mHueHistogramTotal;
    saturation = (saturation * (uint32_t) statistics.mAverageSaturation) / 114;

    hsvColor.s = (uint8_t) std::min( (uint32_t) 255, saturation);
  }
  
  uint32_t modalValue = GetModal_Value(statistics);
  
  hsvColor.v  = (uint8_t) std::min ((uint32_t) 255,  modalValue + 96); // Add offset.
  return hsvColor;
}

uint32_t HistogramRoutines::GetHueBucketSum(const ImageStatistics &iStatistics, size_t centerBucket, size_t windowSize)
{
  uint32_t returnValue = iStatistics.mHueHistogram[centerBucket];

  size_t leftIdx = centerBucket;
  size_t rightIdx = centerBucket;

  for (size_t k = 0; k < windowSize; ++k)
  {
    leftIdx = (leftIdx > 0) ? leftIdx - 1 : ImageStatistics::HueHistogramSize - 1;
    rightIdx = (rightIdx < ImageStatistics::HueHistogramSize - 1) ? rightIdx + 1 : 0;

    returnValue += iStatistics.mHueHistogram[leftIdx] + iStatistics.mHueHistogram[rightIdx];
  }

  return returnValue;
}
