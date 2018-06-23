#include "FileSink.h"
#include "ImageRoutines.h"
#include <stdio.h>

FileSink::FileSink(const std::string &pFileName)
  : mFilename(pFileName)
{

}

FileSink::~FileSink()
{
}

void FileSink::Flush()
{
  ImageRoutines::WritePPM(mCanvasImage, mFilename);
}

void FileSink::SetColor(const RelativeRect &iRect, const RGBColor &iColor) 
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
