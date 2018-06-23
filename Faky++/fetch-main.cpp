#include "RoapSource.h"
#include "ImageRoutines.h"

int main(int argc, const char *argv[])
{
  RoapSource source("192.168.64.233", 855905);

  const char *filename = "fetch.ppm";  

  if (argc > 1)
    filename = argv[1];

  source.FetchImage();
  Image img = source.FetchImage();
  ImageRoutines::WritePPM(img, filename);
  return 0;
}
