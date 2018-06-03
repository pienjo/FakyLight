#ifndef HISTOGRAMROUTINES_H
#define HISTOGRAMROUTINES_H

#include "ColorRoutines.h"

namespace HistogramRoutines
{
  using ColorRoutines::ImageStatistics;

  void SmoothHueHistogram(ImageStatistics &ioStatistics, uint32_t iKernelSize);
}

#endif
