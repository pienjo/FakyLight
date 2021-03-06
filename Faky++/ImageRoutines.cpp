#include "ImageRoutines.h"
#include <stdio.h>
#include <algorithm>
#include <turbojpeg.h>

void ImageRoutines::SetRect(Image &ioImage, const RelativeRect &iRect, const RGBColor &iColor)
{
  AbsoluteRect absoluteRect = ToAbsoluteRect(ioImage, iRect);
  SetRect(ioImage, absoluteRect, iColor);
}
void ImageRoutines::SetRect(Image &ioImage, const AbsoluteRect &iRect, const RGBColor &iColor)
{
  uint8_t *d = ioImage.data();
  
  for (int32_t y = std::max(0, (int32_t) iRect.mTop); y < std::min(iRect.mBottom, (int32_t) ioImage.Height()); ++y)
  {
    uint8_t *d_r = d + y * ioImage.Pitch();
    for (int32_t x = std::max(0, (int32_t) iRect.mLeft); x < std::min(iRect.mRight, (int32_t) ioImage.Width()); ++x)
    {
      d_r[3*x] = iColor.r;
      d_r[3*x+1] = iColor.g;
      d_r[3*x+2] = iColor.b;
    }
  }
}

AbsoluteRect ImageRoutines::ToAbsoluteRect(const Image &iImage, const RelativeRect &iRelativeRect)
{
  AbsoluteRect ar;
  
  ar.mTop = (iImage.Height() * iRelativeRect.mTop + 50) / 100;
  ar.mBottom = (iImage.Height() * iRelativeRect.mBottom + 50) / 100;
  ar.mLeft = (iImage.Width() * iRelativeRect.mLeft + 50) / 100;
  ar.mRight = (iImage.Width() * iRelativeRect.mRight + 50) / 100;
  return ar;
}

RelativeRect ImageRoutines::ToRelativeRect(const Image &iImage, const AbsoluteRect &iAbsoluteRect)
{
  RelativeRect ar;
  int32_t w = iImage.Width(), h = iImage.Height();
  
  if (h == 0 || w == 0)
    return ar;

  ar.mTop = (100 * iAbsoluteRect.mTop + h/2) / h;
  ar.mBottom = (100 * iAbsoluteRect.mBottom + h/2) / h;

  ar.mLeft = (100 * iAbsoluteRect.mLeft + w/2) / w;
  ar.mRight = (100 * iAbsoluteRect.mRight + w/2) / w;
  return ar;
}

void ImageRoutines::WritePPM(const Image &iImage, const std::string &iFilename)
{
  if (!iFilename.empty() && iImage.length() > 0)
  {
    FILE *outfile = fopen(iFilename.c_str(), "wb");
    if (outfile)
    {
      fprintf(outfile, "P6\n%d %d\n255\n", iImage.Width(), iImage.Height());
      fwrite(iImage.data(), iImage.length(), 1, outfile);
      fclose(outfile);
    }
  }
}

Image ImageRoutines::ImageFromJPEG(const std::string &iJpegStream)
{
  Image returnValue;

  tjhandle decompressor = tjInitDecompress();
  
  int width, height, dummy, dummy1;

  if (0 == tjDecompressHeader3(decompressor, (const uint8_t *)iJpegStream.data(), iJpegStream.size(), &width, &height, &dummy, &dummy1))
  {
    returnValue.Claim(width, height);
    if (0 != tjDecompress2(decompressor, (const uint8_t *)iJpegStream.data(), iJpegStream.size(), returnValue.data(), width, returnValue.Pitch(), height,TJPF_RGB, TJFLAG_FASTDCT))
    {
      fprintf(stderr, "Error decoding image");
      returnValue.Release();
    }
  }

  tjDestroy(decompressor);

  return returnValue;
}
