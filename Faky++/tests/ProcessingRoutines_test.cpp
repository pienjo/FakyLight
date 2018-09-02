#include <gtest/gtest.h>
#include <list>
#include <ios>
#include <iostream>
#include <ostream>
#include <fstream>
#include <algorithm>

#include "../ProcessingRoutines.h"
#include "../FileSource.h"
#include "../ColorSink.h"
#include "../FileSink.h"
#include "../ImageRoutines.h"

class ImageProcessingCoordinateCheck : public ::testing::Test
{
  public:
    ImageProcessingCoordinateCheck()
      : mSource(320, 180)
      , mDestination(mSource)
    {

    }

  protected:

    class EmptyImageSource : public ImageSource
    {
      public:
	EmptyImageSource(uint32_t width, uint32_t height)
	  : mWidth(width)
	  , mHeight(height)
	{

	}

	Image FetchImage() override
	{
	  Image retval;
	  
	  retval.Claim(mWidth, mHeight);
	  memset(retval.data(), 0, retval.length());

	  return retval;
	}

      private:
	uint32_t mWidth, mHeight;
    } mSource;

    class CoordinateConvertingColorSink : public ColorSink
    {
      public:
	CoordinateConvertingColorSink(EmptyImageSource &source)
	{
	  mCanvasImage = source.FetchImage();	
	}
	
	virtual void SetColor(const RelativeRect &iRect, const RGBColor &) override
	{
	  mRects.push_back(ImageRoutines::ToAbsoluteRect(mCanvasImage, iRect));	
	}
	
	virtual void Flush() { }
	std::vector<AbsoluteRect> mRects;

	Image mCanvasImage;
    } mDestination;
};

using ProcessingRoutines::Process;

TEST_F(ImageProcessingCoordinateCheck, NrCalls)
{
  // Arrange
  
  // Assume
  ASSERT_EQ(320u, mDestination.mCanvasImage.Width());
  ASSERT_EQ(180u, mDestination.mCanvasImage.Height());

  // Act
  Process(mSource.FetchImage(), mDestination);

  // Assert
  ASSERT_EQ((size_t) 9, mDestination.mRects.size());
}

TEST_F(ImageProcessingCoordinateCheck, rect_0)
{
  // Arrange

  // Act
  Process(mSource.FetchImage(), mDestination);
  AbsoluteRect r = mDestination.mRects[0];

  // Assert
  ASSERT_EQ(240, r.mLeft);
  ASSERT_EQ(120, r.mTop);
  ASSERT_EQ(320, r.mRight);
  ASSERT_EQ(180, r.mBottom);
}

TEST_F(ImageProcessingCoordinateCheck, rect_1)
{
  // Arrange

  // Act
  Process(mSource.FetchImage(), mDestination);
  AbsoluteRect r = mDestination.mRects[1];

  // Assert
  ASSERT_EQ(240, r.mLeft);
  ASSERT_EQ(60, r.mTop);
  ASSERT_EQ(320, r.mRight);
  ASSERT_EQ(120, r.mBottom);
}

TEST_F(ImageProcessingCoordinateCheck, rect_2)
{
  // Arrange

  // Act
  Process(mSource.FetchImage(), mDestination);
  AbsoluteRect r = mDestination.mRects[2];

  // Assert
  ASSERT_EQ(256, r.mLeft);
  ASSERT_EQ(0, r.mTop);
  ASSERT_EQ(320, r.mRight);
  ASSERT_EQ(60, r.mBottom);
}

TEST_F(ImageProcessingCoordinateCheck, rect_3)
{
  // Arrange

  // Act
  Process(mSource.FetchImage(), mDestination);
  AbsoluteRect r = mDestination.mRects[3];

  // Assert
  ASSERT_EQ(192, r.mLeft);
  ASSERT_EQ(0, r.mTop);
  ASSERT_EQ(256, r.mRight);
  ASSERT_EQ(60, r.mBottom);
}

TEST_F(ImageProcessingCoordinateCheck, rect_4)
{
  // Arrange

  // Act
  Process(mSource.FetchImage(), mDestination);
  AbsoluteRect r = mDestination.mRects[4];

  // Assert
  ASSERT_EQ(128, r.mLeft);
  ASSERT_EQ(0, r.mTop);
  ASSERT_EQ(192, r.mRight);
  ASSERT_EQ(60, r.mBottom);
}

TEST_F(ImageProcessingCoordinateCheck, rect_5)
{
  // Arrange

  // Act
  Process(mSource.FetchImage(), mDestination);
  AbsoluteRect r = mDestination.mRects[5];

  // Assert
  ASSERT_EQ(64, r.mLeft);
  ASSERT_EQ(0, r.mTop);
  ASSERT_EQ(128, r.mRight);
  ASSERT_EQ(60, r.mBottom);
}

TEST_F(ImageProcessingCoordinateCheck, rect_6)
{
  // Arrange

  // Act
  Process(mSource.FetchImage(), mDestination);
  AbsoluteRect r = mDestination.mRects[6];

  // Assert
  ASSERT_EQ(0, r.mLeft);
  ASSERT_EQ(0, r.mTop);
  ASSERT_EQ(64, r.mRight);
  ASSERT_EQ(60, r.mBottom);
}

TEST_F(ImageProcessingCoordinateCheck, rect_7)
{
  // Arrange

  // Act
  Process(mSource.FetchImage(), mDestination);
  AbsoluteRect r = mDestination.mRects[7];

  // Assert
  ASSERT_EQ(0, r.mLeft);
  ASSERT_EQ(60, r.mTop);
  ASSERT_EQ(80, r.mRight);
  ASSERT_EQ(120, r.mBottom);
}

TEST_F(ImageProcessingCoordinateCheck, rect_8)
{
  // Arrange

  // Act
  Process(mSource.FetchImage(), mDestination);
  AbsoluteRect r = mDestination.mRects[8];

  // Assert
  ASSERT_EQ(0, r.mLeft);
  ASSERT_EQ(120, r.mTop);
  ASSERT_EQ(80, r.mRight);
  ASSERT_EQ(180, r.mBottom);
}

namespace 
{
  struct RegressionData 
  {
    std::string mFilename;
    std::string mFilenameOut;
    std::string mCsvFilenameOut;
    RGBColor mColors[9];
  };
  
  std::ostream & operator<< (::std::ostream &os, const RegressionData &d)
  {
    os << d.mFilename;
    return os;
  }
}

const RegressionData myRegressionData[] =
{
  { "../../TestImages/100nl.ppm", 
    "../../TestImages/100nl-out-pp.ppm",
    "../../TestImages/100nl-out-pp.csv",
    {
      { 130, 49, 0 },
      { 255, 96, 0 },
      { 255, 96, 0 },
      { 255, 102, 0 },
      { 255, 102, 0 },
      { 255, 90, 0 },
      { 255, 90, 0 },
      { 255, 96, 0 },
      { 130, 49, 0 }
    }
  },
  { "../../TestImages/bamigo.ppm", 
    "../../TestImages/bamigo-out-pp.ppm",
    "../../TestImages/bamigo-out-pp.csv",
    {
      { 121, 115, 124}, 
      { 154, 145, 159}, 
      { 148, 137, 154}, 
      { 154, 144, 160}, 
      { 158, 149, 164}, 
      { 158, 149, 164}, 
      { 154, 144, 160}, 
      { 161, 153, 166}, 
      { 106, 100, 110}
    }
  },
  { "../../TestImages/blackscreen.ppm", 
    "../../TestImages/blackscreen-out-pp.ppm",
    "../../TestImages/blackscreen-out-pp.csv",
    {
      {114, 114, 114 },
      {114, 114, 114 },
      {114, 114, 114 },
      {114, 114, 114 },
      {114, 114, 114 },
      {114, 114, 114 },
      {114, 114, 114 },
      {114, 114, 114 },
      {112, 113, 114 },
    }
  },
  { "../../TestImages/discovery.ppm", 
    "../../TestImages/discovery-out-pp.ppm",
    "../../TestImages/discovery-out-pp.csv",
    {
      { 138, 136, 137}, 
      { 255, 246, 253},
      { 255, 243, 252},
      { 255, 243, 252},
      { 255, 245, 253},
      { 255, 243, 252},
      { 255, 243, 252},
      { 255, 246, 253},
      { 135, 132, 134}
    }
  },
  { "../../TestImages/startrek/input.ppm", 
    "../../TestImages/startrek/input-out-pp.ppm",
    "../../TestImages/startrek/input-out-pp.csv",
    {
      { 124, 120, 111},
      { 179, 169, 162},
      { 41, 159, 179 },
      { 69, 137, 148 },
      { 139, 119, 107 },
      { 136, 97, 101 },
      { 145, 100, 79 },
      { 146, 80, 66 },
      { 157, 91, 65 }
    }
  },
  { "../../TestImages/startrek/input1.ppm", 
    "../../TestImages/startrek/input1-out-pp.ppm",
    "../../TestImages/startrek/input1-out-pp.csv",
    {
      { 118, 121, 126},
      { 101, 116, 127},
      { 43, 93, 137 },
      { 72, 98, 144 },
      { 140, 114, 103 },
      { 105, 115, 130 },
      { 95, 104, 132 },
      { 72, 96, 144 },
      { 117, 114, 119 }
    }
  },
  { "../../TestImages/americans1/input1.ppm", 
    "../../TestImages/americans1/input1-out-pp.ppm",
    "../../TestImages/americans1/input1-out-pp.csv",
    {
      {131,114,101},
      {154,82,78},
      {154,119,93},
      {165,119,112},
      {255,228,208},
      {231,202,185},
      {143,110,86},
      {147,108,100},
      {147,113,94},
    }
  },
  { "../../TestImages/americans1/input2.ppm", 
    "../../TestImages/americans1/input2-out-pp.ppm",
    "../../TestImages/americans1/input2-out-pp.csv",
    {
      {131,113,101},
      {154,101,84},
      {154,115,87},
      {165,117,108},
      {255,227,206},
      {231,200,183},
      {143,111,87},
      {147,111,104},
      {147,107,93},
    }
  },
  { "../../TestImages/RedFlash/redflash-01.ppm", 
    "../../TestImages/RedFlash/redflash-01-out-pp.ppm", 
    "../../TestImages/RedFlash/redflash-01-out-pp.csv", 
    {
      {241,2,12},
      {255,0,9},
      {255,0,9},
      {255,0,9},
      {255,0,9},
      {255,0,9},
      {255,0,9},
      {255,0,9},
      {241,4,14},
    }
  },
  { "../../TestImages/RedFlash/redflash-03.ppm", 
    "../../TestImages/RedFlash/redflash-03-out-pp.ppm", 
    "../../TestImages/RedFlash/redflash-03-out-pp.csv", 
    {
      {243,4,14},
      {255,0,9},
      {255,0,9},
      {255,0,9},
      {255,46,55},
      {255,0,9},
      {255,0,9},
      {255,0,9},
      {241,4,14},
    }
  },
  { "../../TestImages/RedFlash/redflash-05.ppm", 
    "../../TestImages/RedFlash/redflash-05-out-pp.ppm", 
    "../../TestImages/RedFlash/redflash-05-out-pp.csv", 
    {
      {255,0,9},
      {255,0,9},
      {255,0,9},
      {255,0,9},
      {255,45,54},
      {255,0,9},
      {255,0,9},
      {255,0,9},
      {255,0,9},
    }
  },
  { "../../TestImages/RedFlash/redflash-07.ppm", 
    "../../TestImages/RedFlash/redflash-07-out-pp.ppm", 
    "../../TestImages/RedFlash/redflash-07-out-pp.csv", 
    {
      {255,0,9},
      {255,0,9},
      {255,0,9},
      {255,0,9},
      {255,45,54},
      {255,0,9},
      {255,0,9},
      {255,0,9},
      {255,0,9},
    }
  },
  { "../../TestImages/RedFlash/redflash-09.ppm", 
    "../../TestImages/RedFlash/redflash-09-out-pp.ppm", 
    "../../TestImages/RedFlash/redflash-09-out-pp.csv", 
    {
      {255,0,9},
      {255,0,9},
      {255,0,9},
      {255,0,9},
      {255,45,54},
      {255,0,9},
      {255,0,9},
      {255,0,9},
      {255,0,9},
    }
  },
};

class ImageRegressionTest : public ::testing::Test, public ::testing::WithParamInterface<RegressionData>
{
  public:
    ImageRegressionTest() 
      : mResult( GetParam().mFilenameOut)
      , mSource(*this, GetParam().mFilename)
      , mCsvOutput(GetParam().mCsvFilenameOut)
      {
	mInhibit = true;
      }
  
    class CachingAndWritingColorSink : public FileSink
    {
      public:
        CachingAndWritingColorSink(const std::string &pFilename)
	  : FileSink(pFilename)
	  {
	  }
	
	virtual void SetColor(const RelativeRect &iRect, const RGBColor &iColor) override
	{
	  FileSink::SetColor(iRect, iColor);
	  mColors.push_back(iColor); 
	}

	std::vector<RGBColor> mColors;
    } mResult;
    
    class DuplicatingFileSource : public FileSource
    {
      public:
	DuplicatingFileSource(ImageRegressionTest &pParent, const std::string &pFile)
	  : FileSource(pFile)
	  , mParent(pParent)
	  {
	  }

	Image FetchImage() override
	{
	  Image returnValue = FileSource::FetchImage();
	  if (!mParent.mInhibit)
	     mParent.mResult.SetCanvas(returnValue);

	  return returnValue;
	}

      private:
	ImageRegressionTest &mParent;
    } mSource;

    class CsvOutput : public IDebugOutput
    {
      public:
	CsvOutput(const std::string &pFilename)
	  : mFilename(pFilename)
	{
	}
	
	~CsvOutput()
	{
	  if (!mData.empty())
	    WriteOutput();
	}

	void AddDebugOutput(const std::string &pLabel, const ColorRoutines::histogramT &pHistogram) override
	{
	  mData[pLabel].push_back(pHistogram);
	}
      private:
	size_t GetNrColumns() const
	{
	  size_t nrColumns = 0;
	  for (const auto &data : mData)
	  {
	    nrColumns = std::max(nrColumns, data.second.size()); 
	  }

	  return nrColumns;
	}

	size_t GetNrRows() const
	{
	  size_t nrRows = 0;
	  for (const auto &data : mData)
	  {
	    for (const auto &columnData : data.second)
	    {
	      nrRows = std::max(nrRows, columnData.size()); 
	    }
	  }

	  return nrRows;
	}
	void GenerateHeaderLine( std::ofstream &outputFile) const
	{
	  for (size_t i = 0; i < GetNrColumns(); ++i)
	  {
	    for (const auto &data : mData)
	    {
	      outputFile << data.first << i << ",";
	    }
	  }
	  swallowLastCharacter(outputFile);  
	  outputFile << std::endl;
	}

	static void swallowLastCharacter(std::ofstream &outputFile)
	{
	  std::streampos pos = outputFile.tellp();

	  outputFile.seekp(pos - (std::streamoff) 1);
	}

	void GenerateRow(int rowno, std::ofstream &outputFile) const
	{
	  for (size_t i = 0; i < GetNrColumns(); ++i)
	  {
	    for (const auto &data : mData)
	    {
	      if (data.second.size() >= i)
	      {
		const ColorRoutines::histogramT &hist = data.second[i];
		outputFile << hist[rowno] << ",";
	      }
	    }
	  }
	  swallowLastCharacter(outputFile);  
	  outputFile << std::endl;
	}

	void WriteOutput() const
	{
	  size_t nrRows = GetNrRows();

	  std::ofstream outputFile;
	  outputFile.open(mFilename);

	  GenerateHeaderLine( outputFile );
	  for(size_t i = 0; i < nrRows; ++i)
	    GenerateRow(i, outputFile);

	  outputFile.close();
	}

	std::map<std::string, std::vector<ColorRoutines::histogramT> > mData;
	std::string mFilename;
    } mCsvOutput;
    bool mInhibit;
};

TEST_P(ImageRegressionTest, Rect_0)
{
  // Arrange
  mInhibit = false;

  // Act
  Process(mSource.FetchImage(), mResult);

  // Assert
  ASSERT_LE(0u, mResult.mColors.size());
  ASSERT_EQ(GetParam().mColors[0], mResult.mColors[0]);
}

TEST_P(ImageRegressionTest, Rect_1)
{
  // Arrange

  // Act
  Process(mSource.FetchImage(), mResult);

  // Assert
  ASSERT_LE(1u, mResult.mColors.size());
  ASSERT_EQ(GetParam().mColors[1], mResult.mColors[1]);
}

TEST_P(ImageRegressionTest, Rect_2)
{
  // Arrange

  // Act
  Process(mSource.FetchImage(), mResult);

  // Assert
  ASSERT_LE(2u, mResult.mColors.size());
  ASSERT_EQ(GetParam().mColors[2], mResult.mColors[2]);
}

TEST_P(ImageRegressionTest, Rect_3)
{
  // Arrange

  // Act
  Process(mSource.FetchImage(), mResult);

  // Assert
  ASSERT_LE(3u, mResult.mColors.size());
  ASSERT_EQ(GetParam().mColors[3], mResult.mColors[3]);
}

TEST_P(ImageRegressionTest, Rect_4)
{
  // Arrange

  // Act
  Process(mSource.FetchImage(), mResult);

  // Assert
  ASSERT_LE(4u, mResult.mColors.size());
  ASSERT_EQ(GetParam().mColors[4], mResult.mColors[4]);
}

TEST_P(ImageRegressionTest, Rect_5)
{
  // Arrange

  // Act
  Process(mSource.FetchImage(), mResult);

  // Assert
  ASSERT_LE(5u, mResult.mColors.size());
  ASSERT_EQ(GetParam().mColors[5], mResult.mColors[5]);
}

TEST_P(ImageRegressionTest, Rect_6)
{
  // Arrange

  // Act
  Process(mSource.FetchImage(), mResult);

  // Assert
  ASSERT_LE(6u, mResult.mColors.size());
  ASSERT_EQ(GetParam().mColors[6], mResult.mColors[6]);
}

TEST_P(ImageRegressionTest, Rect_7)
{
  // Arrange

  // Act
  Process(mSource.FetchImage(), mResult);

  // Assert
  ASSERT_LE(7u, mResult.mColors.size());
  ASSERT_EQ(GetParam().mColors[7], mResult.mColors[7]);
}

TEST_P(ImageRegressionTest, Rect_8)
{
  // Arrange

  // Act
  Process(mSource.FetchImage(), mResult);

  // Assert
  ASSERT_LE(8u, mResult.mColors.size());
  ASSERT_EQ(GetParam().mColors[8], mResult.mColors[8]);
}

INSTANTIATE_TEST_CASE_P( RegressionTest, ImageRegressionTest, ::testing::ValuesIn( myRegressionData ));

