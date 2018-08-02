#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "ImageSource.h"
#include "ColorSink.h"

#include <atomic>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <list>

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

    std::condition_variable mImageReadyCV;
    std::mutex mImageReadyMutex;

    std::list<Image> mOutstandingImages;

    std::atomic<uint32_t> mNrFramesRetrieved;
    std::atomic<bool> mTimeoutTriggered;
};

#endif
