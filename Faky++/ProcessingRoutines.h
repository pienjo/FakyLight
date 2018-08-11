#ifndef PROCESSINGROUTINES_H
#define PROCESSINGROUTINES_H

#include "ColorSink.h"
#include "IDebugOutput.h"
#include "Image.h"

namespace ProcessingRoutines
{
  void Process(const Image &source, ColorSink &sink, IDebugOutput *pOptionalDebugOutput = nullptr);
}
#endif
