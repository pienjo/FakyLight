#ifndef IPIN_H
#define IPIN_H

class IPin
{
  public:
    virtual ~IPin() = default;
    virtual void Engage() = 0;
    virtual void Disengage() = 0;
};

#endif
