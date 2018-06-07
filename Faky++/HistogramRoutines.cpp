#include "HistogramRoutines.h"

using HistogramRoutines::ImageStatistics;

void HistogramRoutines::SmoothHueHistogram(ImageStatistics &ioStatistics, uint32_t iKernelSize)
{
  std::vector<uint32_t> copy = ioStatistics.mHueHistogram;

  uint32_t total = copy[0];

  for (uint32_t init = 1; init <= iKernelSize; ++init)
  {
    total += copy[init];
    total += copy[ImageStatistics::HueHistogramSize - init ];
  }
 
  for (uint32_t center = 0; center < ImageStatistics::HueHistogramSize; ++center)
  {
    ioStatistics.mHueHistogram[center] = total;
    
    int oldIndex = (center - iKernelSize + ImageStatistics::HueHistogramSize) % (ImageStatistics::HueHistogramSize); 

    uint32_t oldValue = copy[oldIndex];
    
    int newIndex = (center + iKernelSize + 1) % (ImageStatistics::HueHistogramSize); 
    uint32_t newValue = copy[ newIndex];
    total += newValue;
    total -= oldValue;
  }

  ioStatistics.mHueHistogramTotal *= (2*iKernelSize + 1);
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
