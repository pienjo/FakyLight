#include "GPIOPin.h"

#include <stdexcept>

    
GPIOPin::GPIOPin(const char *pPinDevice, uint32_t pPinNr)
{
  mChip = gpiod_chip_open(pPinDevice);
  if (!mChip)
    throw std::runtime_error("Unable to open gpiod device");

  mLine = gpiod_chip_get_line(mChip, pPinNr);
  if (!mLine)
    throw std::runtime_error("Error locating gpio pin");

  if (0 != gpiod_line_request_output(mLine, "FL enable", 0))
    throw std::runtime_error("Unable to claim line for output");
}

GPIOPin::~GPIOPin()
{
  if (mLine)
  {
    gpiod_line_release(mLine);
    mLine = nullptr;
  }

  if (mChip)
  {
    gpiod_chip_close(mChip);
    mChip = nullptr;
  }
}
void GPIOPin::Engage()
{
  gpiod_line_set_value(mLine, 1);
}

void GPIOPin::Disengage()
{
  gpiod_line_set_value(mLine, 0);
}
