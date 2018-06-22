#include "RoapSource.h"
#include "ImageRoutines.h"
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Infos.hpp>
#include <sstream>

RoapSource::RoapSource(const std::string &pHostName, uint32_t pAuthenticationCookie)
  : mHostName(pHostName)
  , mAuthenticationCookie(pAuthenticationCookie)
{
  PerformAuthenticationRequest();
}

RoapSource::~RoapSource()
{
}

Image RoapSource::FetchImage() 
{
  cURLpp::Easy fetchRequest;

  std::ostringstream requestStream;

  requestStream << "http://" << mHostName << ":8080/roap/api/data?target=screen_image&width="<<REQUEST_WIDTH<<"&height="<<REQUEST_HEIGHT<<"&type=0";

  fetchRequest.setOpt<cURLpp::Options::Url>(requestStream.str());
  fetchRequest.setOpt<cURLpp::Options::HttpGet>(true);
  
  std::string result;

  try
  {
    for (;; )
    {
      std::ostringstream resultStream;
      fetchRequest.setOpt<cURLpp::Options::WriteStream>(&resultStream);
      fetchRequest.perform();

      int status = cURLpp::Infos::ResponseCode::get(fetchRequest);
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
  authRequest.setOpt<cURLpp::Options::Post>(true);
  
  std::ostringstream ignoredOutputStream; 
  authRequest.setOpt<cURLpp::Options::WriteStream>(&ignoredOutputStream); 
  authRequest.perform();
}
