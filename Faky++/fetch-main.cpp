#include "RoapSource.h"
#include "ImageRoutines.h"

int main(void)
{
  RoapSource source("192.168.64.233", 855905);
	
  source.FetchImage();
  Image img = source.FetchImage();
  ImageRoutines::WritePPM(img, "fetch.ppm");
  return 0;
}
