#include "RoapSource.h"
#include "ImageRoutines.h"
#include <curlpp/Options.hpp>
#include <curlpp/Infos.hpp>
#include <sstream>

RoapSource::RoapSource(const std::string &pHostName, uint32_t pAuthenticationCookie)
  : mHostName(pHostName)
  , mAuthenticationCookie(pAuthenticationCookie)
  , mFetchRequest(nullptr)
{
}

RoapSource::~RoapSource()
{
  delete mFetchRequest;
}

Image RoapSource::FetchImage() 
{
  if (mFetchRequest == nullptr)
  {
    mFetchRequest = new cURLpp::Easy;

    std::ostringstream requestStream;
    
    requestStream << "http://" << mHostName << ":8080/roap/api/data?target=screen_image&width="<<REQUEST_WIDTH<<"&height="<<REQUEST_HEIGHT<<"&type=0";

    mFetchRequest->setOpt<cURLpp::Options::Url>(requestStream.str());
    mFetchRequest->setOpt<cURLpp::Options::HttpGet>(true);
    mFetchRequest->setOpt<cURLpp::Options::Timeout>(5);

    std::list<std::string> header;
    header.push_back("Connection: keep-alive");
    mFetchRequest->setOpt<cURLpp::Options::HttpHeader>(header);
  } 
  std::string result;

  try
  {
    for (;; )
    {
      std::ostringstream resultStream;
      mFetchRequest->setOpt<cURLpp::Options::WriteStream>(&resultStream);
      mFetchRequest->perform();

      int status = cURLpp::Infos::ResponseCode::get(*mFetchRequest);
      if (status == 200)
      {
	// Success
	result = resultStream.str();
	break;
      }
      else 
      {
	PerformAuthenticationRequest();
      }
    }
  } 
  catch (cURLpp::LibcurlRuntimeError &e)
  {
    switch (e.whatCode())
    {
      case CURLE_COULDNT_CONNECT:
      case CURLE_COULDNT_RESOLVE_HOST:
      case CURLE_OPERATION_TIMEDOUT:
	throw ImageSource::timeout_error();
      default:
	throw e;
    }
  }

  return ImageRoutines::ImageFromJPEG(result);
}

void RoapSource::PerformAuthenticationRequest()
{
  cURLpp::Easy authRequest;

  std::ostringstream authStream;
  authStream <<"<?xml version=\"1.0\" encoding=\"utf-8\"?><auth><type>AuthReq</type><value>" << mAuthenticationCookie << "</value></auth>";
  
  std::string authRequestStr = authStream.str(); 

  std::ostringstream urlStream;
  urlStream << "http://"<<mHostName<<":8080/roap/api/auth";
  authRequest.setOpt<cURLpp::Options::Url>(urlStream.str());

  std::list<std::string> header;
  header.push_back("Content-Type: application/atom+xml");
  header.push_back("Expect:"); // Disable expect-header

  authRequest.setOpt<cURLpp::Options::HttpHeader>(header);
   
  std::istringstream curlStream(authRequestStr);
  authRequest.setOpt<cURLpp::Options::PostFieldSize>(authRequestStr.size());
  authRequest.setOpt<cURLpp::Options::ReadStream>(&curlStream);
  authRequest.setOpt<cURLpp::Options::Timeout>(5);
  authRequest.setOpt<cURLpp::Options::Post>(true);
  
  std::ostringstream ignoredOutputStream; 
  authRequest.setOpt<cURLpp::Options::WriteStream>(&ignoredOutputStream); 
  authRequest.perform();
}
