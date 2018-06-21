#ifndef PROCESSINGROUTINES_H
#define PROCESSINGROUTINES_H

class ImageSource;
class ColorSink;

namespace ProcessingRoutines
{
  void Process(ImageSource &source, ColorSink &sink);
}
#endif
