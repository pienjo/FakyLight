#include "Scheduler.h"
#include <thread>
#include <chrono>
#include "ProcessingRoutines.h"

using namespace std::chrono_literals;

Scheduler::Scheduler(ImageSource &source, ColorSink &sink)
  : mSource(source)
  , mSink(sink)
  , mNrFramesRetrieved(0)
  , mTimeoutTriggered(false)
{
}

void Scheduler::RetrieveLoop()
{
  printf("Retrieve: %p\n", this);
  const auto framePeriod = 50ms;
  const auto timeoutPeriod = 1s;
  
  for ( ;; )
  {
    std::chrono::steady_clock::time_point wakeupTime = std::chrono::steady_clock::now();
    Image retrievedImage;
    try 
    {
      retrievedImage = mSource.FetchImage();
      ++mNrFramesRetrieved;
      wakeupTime += framePeriod;
    } catch (ImageSource::timeout_error &)
    {
      wakeupTime += timeoutPeriod; 
      mTimeoutTriggered = true;
    }
    
    do {
      std::lock_guard<std::mutex> lock(mImageReadyMutex);
      mOutstandingImages.push_back(retrievedImage);
    } while(0); 
    mImageReadyCV.notify_one();

    std::this_thread::sleep_until(wakeupTime);
  }
}

void Scheduler::CalculationLoop()
{
  printf("Calculation: %p\n", this);
  for(;;)
  {
    Image imageToProcess;
    do {
      std::unique_lock<std::mutex> lock(mImageReadyMutex);
      mImageReadyCV.wait(lock, [this, &imageToProcess] () -> bool
	  {
	    if (!mOutstandingImages.empty())
	    {
	      imageToProcess = mOutstandingImages.front();
	      mOutstandingImages.pop_front();
	      return true;
	    }
	    else
	    {
	      return false;
	    }
	  });
    } while(0);
    mImageReadyCV.notify_one();

    if(imageToProcess.length() > 0)
      ProcessingRoutines::Process(imageToProcess, mSink);
  }
}

void Scheduler::DiagnosticsLoop()
{
  printf("Diagnostics: %p\n", this);

  for(;;)
  {
    uint32_t produced = mNrFramesRetrieved.exchange(0);

    if (produced != 0)
    {
      mTimeoutTriggered = false;
    } 
    printf("%d FPS %s\r", produced, mTimeoutTriggered?"(Timeout)":"          ");
    fflush(stdout);
    std::this_thread::sleep_for( 1s );
  }
}

void Scheduler::Run()
{
  mNrFramesRetrieved = 0;
  
  std::thread retrieveThread ( [this] { RetrieveLoop(); });

  std::thread calculationThread([this] { CalculationLoop(); } );
  std::thread diagnosticsThread([this] { DiagnosticsLoop(); } );

  retrieveThread.join();
  calculationThread.join();
  diagnosticsThread.join();
}
