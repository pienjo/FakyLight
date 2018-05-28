#ifndef FILESOURCE_H
#define FILESOURCE_H

#include "ImageSource.h"
#include <vector>
#include <string>

class FileSource : public ImageSource
{
  public:
    FileSource(const std::vector<std::string> &pFiles);
    FileSource(const std::string &pFile);

    Image FetchImage() override;
    
    size_t size() const { return mFileNames.size(); }

  private:

    std::vector<std::string> mFileNames;
    uint32_t mCurrentFileIndex;
};

#endif
