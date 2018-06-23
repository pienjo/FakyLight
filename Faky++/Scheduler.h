#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "ImageSource.h"
#include "ColorSink.h"

#include <atomic>
#include <thread>

class Scheduler
{
  public:
    Scheduler(ImageSource &source, ColorSink &sink);

    void Run();
  private:
    ImageSource &mSource;
    ColorSink &mSink;

    void RetrieveLoop();
    void CalculationLoop();
    void DiagnosticsLoop();

    std::atomic<uint32_t> mNrFramesRetrieved;
};

#endif
