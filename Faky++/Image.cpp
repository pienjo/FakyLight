#include "Image.h"

#include <cstring>

Image::Image() 
  : mHeight(0)
  , mWidth(0)
  , mData(nullptr)
{
}

Image::Image(const Image &pOther) : Image()
{
  Copy(pOther);
}

Image::Image(Image &&pOther)
  : mHeight(pOther.mHeight)
  , mWidth(pOther.mWidth)
  , mData(pOther.mData)
{
  pOther.mData = nullptr;
  pOther.mWidth = 0;
  pOther.mHeight = 0;
}

Image::~Image() 
{
  Release();
}

Image &Image::operator=(const Image &pOther)
{
  if (&pOther != this)
    Copy(pOther);

  return *this;
}

Image &Image::operator=(Image &&pOther)
{
  if (&pOther != this)
  {
    Release(); 
    mData = pOther.mData; pOther.mData = nullptr;
    mWidth = pOther.mWidth; pOther.mWidth = 0;
    mHeight = pOther.mHeight; pOther.mHeight = 0;
  }
  return *this;
}

void Image::Copy(const Image &pOther)
{
  Claim(pOther.Width(), pOther.Height());
  memcpy(data(), pOther.data(), length());
}

void Image::Claim(uint32_t Width, uint32_t Height)
{
  Release();
  if (Width > 0 && Height > 0)
  {
    mWidth = Width;
    mHeight = Height;
    mData = new uint8_t[length()];
  }
}

size_t Image::length() const
{
  return (size_t) (mHeight * mWidth * 3);
}
void Image::Release() 
{
  delete[] mData;
  mData = nullptr;
  mWidth = 0;
  mHeight = 0;
}
