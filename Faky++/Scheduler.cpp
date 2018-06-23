#include "Scheduler.h"
#include <thread>
#include <chrono>

using namespace std::chrono_literals;

Scheduler::Scheduler(ImageSource &source, ColorSink &sink)
  : mSource(source)
  , mSink(sink)
{
}

void Scheduler::RetrieveLoop()
{
  printf("Retrieve: %p\n", this);
  const auto framePeriod = 10ms;
  const auto timeoutPeriod = 1s;
  
  for ( ;; )
  {
    std::chrono::system_clock::time_point wakeupTime = std::chrono::system_clock::now();

    try 
    {
      mSource.FetchImage();
      ++mNrFramesRetrieved;
      wakeupTime += framePeriod;
    } catch (ImageSource::timeout_error &)
    {
      printf("Timeout\n");
      wakeupTime += timeoutPeriod; 
    }

    std::this_thread::sleep_until(wakeupTime);
  }
}

void Scheduler::CalculationLoop()
{

  printf("Calculation: %p\n", this);
}

void Scheduler::DiagnosticsLoop()
{
  printf("Diagnostics: %p\n", this);

  for(;;)
  {
    uint32_t produced = mNrFramesRetrieved.exchange(0);
    
    printf("%d FPS\n", produced);
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
