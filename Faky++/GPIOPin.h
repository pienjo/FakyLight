#ifndef GPIOPIN_H
#define GPIOPIN_H
#include "IPin.h"
#include <inttypes.h>
#include <gpiod.h>

// This impleme
class GPIOPin : public IPin
{
  public:
    GPIOPin(const char *pPinDevice, uint32_t pPinNr);
    ~GPIOPin();

    void Engage() override;
    void Disengage() override;

  private:
    struct gpiod_chip *mChip;
    struct gpiod_line *mLine;
};

#endif
