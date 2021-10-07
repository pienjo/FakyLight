#include "UDPStrip.h"
#include <stdexcept>

UDPStrip::UDPStrip(const char *hostname, uint16_t port)
{
  mSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (0 == mSocket)
  {
    perror("socket");
  }
  mSockAddr.sin_family = AF_INET;
  mSockAddr.sin_port = htons(port);
  if (0 == inet_aton(hostname, &mSockAddr.sin_addr))
  {
    throw std::runtime_error("Error converting UDP address");
  }
}

void UDPStrip::SetContents(const std::vector<RGBColor> &contents)
{
  std::vector<uint8_t> buf;
  buf.reserve(contents.size() * 3);
  
  for (auto it = contents.rbegin(); it != contents.rend(); ++it)
  {
    buf.push_back(it->g);
    buf.push_back(it->r);
    buf.push_back(it->b);
  }
  
  sendto(mSocket, buf.data(), buf.size(), 0, (struct sockaddr *) &mSockAddr, sizeof(mSockAddr));
}
