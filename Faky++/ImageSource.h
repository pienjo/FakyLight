#ifndef IMAGESOURCE_H
#define IMAGESOURCE_H

#include "Image.h"
#include <stdexcept>

class ImageSource
{
  public:
    virtual ~ImageSource() { }

    virtual Image FetchImage() = 0;    

    class timeout_error : public std::runtime_error
    {
      public:
	timeout_error() : std::runtime_error("Timeout")
	{
	}
    };
};

#endif
