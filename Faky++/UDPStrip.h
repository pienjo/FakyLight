#ifndef UDPSTRIP_H
#define UDPSTRIP_H

#include "LEDStrip.h"
#include <arpa/inet.h>
#include <sys/socket.h>

class UDPStrip : public LEDStrip
{
  public:
    UDPStrip(const char *hostname, uint16_t port);

    void SetContents(const std::vector<RGBColor> &pValues) override;
  private:
    int mSocket;
    struct sockaddr_in mSockAddr;
};

#endif
