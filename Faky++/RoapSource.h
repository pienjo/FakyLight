#ifndef ROAPSOURCE_H
#define ROAPSOURCE_H

#include "ImageSource.h"
#include <string>
#include <curlpp/cURLpp.hpp>

class RoapSource : public ImageSource
{
  public:
    RoapSource(const std::string &pHostName, uint32_t pAuthenticationCookie);
    ~RoapSource();

    Image FetchImage() override;

  private:
    void PerformAuthenticationRequest();
    
    
    static curlpp::Cleanup mCurlCleanup;
    std::string mHostName;
    uint32_t mAuthenticationCookie;
    
    enum : uint32_t 
    {
      REQUEST_WIDTH = 320,
      REQUEST_HEIGHT = 180
    };
};

#endif
