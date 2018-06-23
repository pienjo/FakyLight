#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "ImageSource.h"
#include "ColorSink.h"

#include <atomic>

class Scheduler
{
  public:
    Scheduler(ImageSource &source, ColorSink &sink);

    void Run();
  private:
    ImageSource &mSource;
    ColorSink &mSink;
};

#endif
