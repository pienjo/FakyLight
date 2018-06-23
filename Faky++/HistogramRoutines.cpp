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

void HistogramRoutines::ClearLowerValueBuckets(ImageStatistics &ioStatistics, size_t nrBuckets)
{
  for (size_t i = 0; i < nrBuckets && i < 256; ++i)
  {
    ioStatistics.mValueHistogram[i] = 0;
  }
};

HSVColor HistogramRoutines::GetDominantColor(const ImageStatistics &statistics)
{
  HSVColor hsvColor;
  
  // The hue is the mode of the hue histogram
  hsvColor.h = GetMode_Hue(statistics);
  
  // The magic formula: The ratio of the surface of the mode bin and the total surface of the hue histogram, multiplied
  // by the average saturation.

  if (statistics.mHueHistogramTotal > 0)
  {
    uint32_t saturation = 256 * 15 * statistics.mHueHistogram[hsvColor.h] / statistics.mHueHistogramTotal;
    saturation = (saturation * (uint32_t) statistics.mAverageSaturation) / 128;

    hsvColor.s = (uint8_t) std::min( (uint32_t) 255, saturation);
  }
  
  uint32_t modalValue = GetModal_Value(statistics);
  
  hsvColor.v  = (uint8_t) std::min ((uint32_t) 255,  modalValue + 96); // Add offset.
  return hsvColor;
}
