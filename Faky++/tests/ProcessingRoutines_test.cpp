#include <gtest/gtest.h>

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
	
	virtual void SetColor(const RelativeRect &iRect, const ColorRoutines::RGBColor &) override
	{
	  mRects.push_back(ImageRoutines::ToAbsoluteRect(mCanvasImage, iRect));	
	}

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
  Process(mSource, mDestination);

  // Assert
  ASSERT_EQ((size_t) 9, mDestination.mRects.size());
}

TEST_F(ImageProcessingCoordinateCheck, rect_0)
{
  // Arrange

  // Act
  Process(mSource, mDestination);
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
  Process(mSource, mDestination);
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
  Process(mSource, mDestination);
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
  Process(mSource, mDestination);
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
  Process(mSource, mDestination);
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
  Process(mSource, mDestination);
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
  Process(mSource, mDestination);
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
  Process(mSource, mDestination);
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
  Process(mSource, mDestination);
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
    ColorRoutines::RGBColor mColors[9];

  };
  
  std::ostream & operator<< (::std::ostream &os, const RegressionData &d)
  {
    os << d.mFilename;
    return os;
  }
  std::ostream & operator<< (::std::ostream &os, const ColorRoutines::RGBColor &d)
  {
    os << "(" << (int)d.r << "," << (int)d.g << "," << (int)d.b <<")";
    return os;
  }
}

const RegressionData myRegressionData[] =
{
  { "../../TestImages/100nl.ppm", 
    "../../TestImages/100nl-out-pp.ppm",
    {
      { 130, 46, 0 },
      { 255, 96, 0 },
      { 255, 96, 0 },
      { 255, 102, 0 },
      { 255, 102, 0 },
      { 255, 90, 0 },
      { 255, 90, 0 },
      { 255, 102, 0 },
      { 130, 46, 0 }
    }
  }
};

class ImageRegressionTest : public ::testing::Test, public ::testing::WithParamInterface<RegressionData>
{
  public:
    ImageRegressionTest() 
      : mResult( GetParam().mFilenameOut)
      , mSource(*this, GetParam().mFilename)
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
	
	virtual void SetColor(const RelativeRect &iRect, const ColorRoutines::RGBColor &iColor) override
	{
	  FileSink::SetColor(iRect, iColor);
	  mColors.push_back(iColor); 
	}

	std::vector<ColorRoutines::RGBColor> mColors;
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
    bool mInhibit;
};

TEST_P(ImageRegressionTest, Rect_0)
{
  // Arrange
  mInhibit = false;

  // Act
  Process(mSource, mResult);

  // Assert
  ASSERT_LE(0u, mResult.mColors.size());
  ASSERT_EQ(GetParam().mColors[0], mResult.mColors[0]);
}

TEST_P(ImageRegressionTest, Rect_1)
{
  // Arrange

  // Act
  Process(mSource, mResult);

  // Assert
  ASSERT_LE(1u, mResult.mColors.size());
  ASSERT_EQ(GetParam().mColors[1], mResult.mColors[1]);
}

TEST_P(ImageRegressionTest, Rect_2)
{
  // Arrange

  // Act
  Process(mSource, mResult);

  // Assert
  ASSERT_LE(2u, mResult.mColors.size());
  ASSERT_EQ(GetParam().mColors[2], mResult.mColors[2]);
}

TEST_P(ImageRegressionTest, Rect_3)
{
  // Arrange

  // Act
  Process(mSource, mResult);

  // Assert
  ASSERT_LE(3u, mResult.mColors.size());
  ASSERT_EQ(GetParam().mColors[3], mResult.mColors[3]);
}

TEST_P(ImageRegressionTest, Rect_4)
{
  // Arrange

  // Act
  Process(mSource, mResult);

  // Assert
  ASSERT_LE(4u, mResult.mColors.size());
  ASSERT_EQ(GetParam().mColors[4], mResult.mColors[4]);
}

TEST_P(ImageRegressionTest, Rect_5)
{
  // Arrange

  // Act
  Process(mSource, mResult);

  // Assert
  ASSERT_LE(5u, mResult.mColors.size());
  ASSERT_EQ(GetParam().mColors[5], mResult.mColors[5]);
}

TEST_P(ImageRegressionTest, Rect_6)
{
  // Arrange

  // Act
  Process(mSource, mResult);

  // Assert
  ASSERT_LE(6u, mResult.mColors.size());
  ASSERT_EQ(GetParam().mColors[6], mResult.mColors[6]);
}

TEST_P(ImageRegressionTest, Rect_7)
{
  // Arrange

  // Act
  Process(mSource, mResult);

  // Assert
  ASSERT_LE(7u, mResult.mColors.size());
  ASSERT_EQ(GetParam().mColors[7], mResult.mColors[7]);
}

TEST_P(ImageRegressionTest, Rect_8)
{
  // Arrange

  // Act
  Process(mSource, mResult);

  // Assert
  ASSERT_LE(8u, mResult.mColors.size());
  ASSERT_EQ(GetParam().mColors[8], mResult.mColors[8]);
}

INSTANTIATE_TEST_CASE_P( RegressionTest, ImageRegressionTest, ::testing::ValuesIn( myRegressionData ));

