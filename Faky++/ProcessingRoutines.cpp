#include "ProcessingRoutines.h"
#include "ColorSink.h"
#include "ImageSource.h"

#include "HistogramRoutines.h"

#define SMOOTH_KERNEL_SIZE 7
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

void ProcessingRoutines::Process(ImageSource &source, ColorSink &sink)
{
  try {
    Image sourceImage = source.FetchImage();
  
    for (const RelativeRect &r : rectsToProcess )
    {
      ColorRoutines::ImageStatistics stats = ColorRoutines::GetImageStatistics(sourceImage, r);
      
      HistogramRoutines::SmoothHueHistogram(stats, SMOOTH_KERNEL_SIZE );
      HistogramRoutines::ClearLowerValueBuckets(stats, MIN_VALUE_BUCKET );
      HSVColor dominantHSV = HistogramRoutines::GetDominantColor(stats);
      
      sink.SetColor(r, ColorRoutines::HSVtoRGB(dominantHSV));
    }

    sink.Flush();
  }
  catch ( ImageSource::timeout_error &e )
  {
    // Do nothing, swallow exception
  }
}
