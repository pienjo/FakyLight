#ifndef IDEBUGOUTPUT_H
#define IDEBUGOUTPUT_H

#include "ColorRoutines.h"

class IDebugOutput
{
  public:
    virtual void AddDebugOutput(const std::string &pLabel, const ColorRoutines::histogramT &pHistogram) = 0;
};

#endif
