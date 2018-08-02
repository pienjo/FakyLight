#include "RoapSource.h"
#include "ImageRoutines.h"
#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>

int main(int argc, const char *argv[])
{
  RoapSource source("192.168.64.233", 855905);

  std::string filename = "fetch.ppm";  
  bool continuous = false;

  for (int i = 1; i < argc; ++i)
  {
    std::string thisArg(argv[i]);
    
    if (thisArg[0] == '-')
    {
      if (thisArg[1] == 'c')
	continuous = true;
    }
    else
    {
      filename = thisArg;
    }
  }
  
  if (continuous)
  {
    size_t period = filename.find('.');
    std::string basename, ext;

    basename = filename.substr(0, period);
    if (period != std::string::npos)
    {
      ext = filename.substr(period);
    }
    else
    {
      ext = ".ppm";
    }

    for (int count = 0;;++count)
    {
      std::stringstream ss;
      ss << basename << "-" << std::setw(3) << std::setfill('0') << count << ext;
      Image img = source.FetchImage();
      ImageRoutines::WritePPM(img, ss.str());
      std::cout << ss.str() << std::endl;
    }
    
  }
  else
  {
    Image img = source.FetchImage();
    ImageRoutines::WritePPM(img, filename);
  }
  return 0;
}
