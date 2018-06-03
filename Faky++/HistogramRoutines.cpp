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

