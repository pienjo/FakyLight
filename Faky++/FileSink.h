#ifndef FILESINK_H
#define FILESINK_H
#include "ColorSink.h"
#include "Image.h"
#include <string>

class FileSink : public ColorSink
{
  public:
    FileSink(const std::string &pFileName);
    ~FileSink();

    void SetCanvas(const Image &pImage);
    void Flush() override;	
    virtual void SetColor(const RelativeRect &iRect, const ColorRoutines::RGBColor &iColor) override;

  private:
    const std::string mFilename;
    Image mCanvasImage;
};

#endif

