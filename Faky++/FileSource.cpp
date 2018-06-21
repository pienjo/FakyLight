#include "FileSource.h"
#include <stdexcept>

FileSource::FileSource(const std::vector<std::string> &pFiles)
  : mFileNames (pFiles)
  , mCurrentFileIndex(0)
  {

  }

FileSource::FileSource(const std::string &pFilename)
  : mCurrentFileIndex(0)
{
  mFileNames.push_back(pFilename);
}

Image FileSource::FetchImage()
{
  if (mCurrentFileIndex >= size())
  {
    mCurrentFileIndex = 0; // reset;
    throw std::out_of_range("done!");
  }
  const char *filename =  mFileNames[mCurrentFileIndex++].c_str();
  FILE *f = fopen(filename, "rb");
  if (nullptr == f)
  {
    throw std::logic_error("file not found");
  }

  Image returnValue;
  try {
    char buf[16];
    if (!fgets(buf, sizeof(buf), f))
    {
      throw std::logic_error("Error reading header");
    }
    
    if (buf[0] != 'P' || buf[1] != '6')
    {
      throw std::logic_error("invalid filetype");
    }

    /* skip comments */

    char c = getc(f);
    while (c == '#') 
    {
      while (getc(f) != '\n') ;
      c = getc(f);
    }
    ungetc(c, f);

    uint32_t width, height, format;
    if (3 != fscanf(f, "%d %d %d\n", &width, &height, &format))
    {
      throw std::logic_error("Error parsing image header");
    }

    if (format != 255)
    {
      throw std::logic_error("Unsupported format");
    }
    
    returnValue.Claim(width, height);
    
    if (returnValue.length() != fread(returnValue.data(), 1, returnValue.length(), f))
    {
      throw std::logic_error("Short read");
    }
    
    fclose(f);
  } catch (std::exception &e)
  {
    // Close file
    fclose(f);
    // And rethrow
    throw(e);
  }

  return std::move(returnValue);
}
