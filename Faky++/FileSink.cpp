#include "FileSink.h"
#include "ImageRoutines.h"
#include <stdio.h>

FileSink::FileSink(const std::string &pFileName)
  : mFilename(pFileName)
{

}

FileSink::~FileSink()
{
  Flush();
}

void FileSink::Flush()
{
  if (!mFilename.empty() && mCanvasImage.length() > 0)
  {
    FILE *outfile = fopen(mFilename.c_str(), "wb");
    if (outfile)
    {
      fprintf(outfile, "P6\n%d %d\n255\n", mCanvasImage.Width(), mCanvasImage.Height());
      fwrite(mCanvasImage.data(), mCanvasImage.length(), 1, outfile);
      fclose(outfile);
    }
  }
}

void FileSink::SetColor(const RelativeRect &iRect, const ColorRoutines::RGBColor &iColor) 
{
  if(mCanvasImage.data() != nullptr)
  {
    ImageRoutines::SetRect(mCanvasImage, iRect, iColor);
  }
}

void FileSink::SetCanvas(const Image &pImage)
{
  mCanvasImage = pImage;
}
