#ifndef IMAGESOURCE_H
#define IMAGESOURCE_H

#include "Image.h"

class ImageSource
{
  public:
    virtual ~ImageSource() { }

    virtual Image FetchImage() = 0;    
};

#endif
