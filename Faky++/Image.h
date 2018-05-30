#ifndef IMAGE_H
#define IMAGE_H

#include <cinttypes>
#include <sys/types.h>

class Image 
{
  public:
    Image();
    Image(const Image &pOther);
    Image(Image &&pOther);

    virtual ~Image();
    Image &operator=(const Image &pOther);
    Image &operator=(Image &&pOther);

    void Claim(uint32_t Width, uint32_t Height);
    virtual void Release();

    uint32_t Width() const { return mWidth; }
    uint32_t Height() const { return mHeight; }
    uint32_t Pitch() const { return Stride() * mWidth; }
    uint32_t Stride() const { return 3; }
    const uint8_t *data() const { return mData; }
    uint8_t *data() { return mData; }
    size_t length() const;
  private:
    uint32_t mHeight, mWidth;
    uint8_t *mData;
    void Copy(const Image &pSource);
};
#endif
