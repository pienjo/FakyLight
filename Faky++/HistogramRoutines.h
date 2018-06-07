#ifndef HISTOGRAMROUTINES_H
#define HISTOGRAMROUTINES_H

#include "ColorRoutines.h"

namespace HistogramRoutines
{
  using ColorRoutines::ImageStatistics;

  void SmoothHueHistogram(ImageStatistics &ioStatistics, uint32_t iKernelSize);

  size_t GetMode_Hue(const ImageStatistics &iStatistics);
  size_t GetModal_Value(const ImageStatistics &iStatistics);

  
}

#endif
