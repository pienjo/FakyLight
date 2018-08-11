#include "ProcessingRoutines.h"
#include "ColorSink.h"

#include "HistogramRoutines.h"

#define MIN_VALUE_BUCKET 9
const RelativeRect rectsToProcess[] =
{
  { 75, 66.6, 100, 100 },
  { 75, 33.3, 100,  66.6 },
  { 80,  0, 100,  33.3 },
  { 60,  0,  80,  33.3 },
  { 40,  0,  60,  33.3 },
  { 20,  0,  40,  33.3 },
  {  0,  0,  20,  33.3 },
  {  0, 33.3,  25,  66.6 },
  {  0, 66.6,  25, 100 },
};

namespace
{
std::vector<uint32_t> kernel = { 70, 56, 28, 8, 1};
}

void ProcessingRoutines::Process(const Image &sourceImage, ColorSink &sink, IDebugOutput *pOptionalDebugOutput)
{
  for (const RelativeRect &r : rectsToProcess )
  {
    ColorRoutines::ImageStatistics stats = ColorRoutines::GetImageStatistics(sourceImage, r);
    if (pOptionalDebugOutput)
      pOptionalDebugOutput->AddDebugOutput("RawHue", stats.mHueHistogram); 

    HistogramRoutines::SmoothHueHistogram(stats, kernel );
    if (pOptionalDebugOutput)
      pOptionalDebugOutput->AddDebugOutput("FilteredHue", stats.mHueHistogram); 

    HistogramRoutines::ClearLowerValueBuckets(stats, MIN_VALUE_BUCKET );
    HSVColor dominantHSV = HistogramRoutines::GetDominantColor(stats);
    
    sink.SetColor(r, ColorRoutines::HSVtoRGB(dominantHSV));
  }

  sink.Flush();
}
