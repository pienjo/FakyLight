#include <gtest/gtest.h>
#include "../LEDSink.h"
#include "../LEDStrip.h"

namespace 
{
  ::testing::AssertionResult AssertStripContents(const char *m_expr, const char *n_expr, const char *, RGBColor m, RGBColor n, int index)
  {
    if (m == n)
      return ::testing::AssertionSuccess();

    return ::testing::AssertionFailure() 
      << m_expr << " ( " << m << " ) and "
      << n_expr << " ( " << n << " ) differ at index " << index;
  }
}

class LEDSinkTest : public ::testing::Test
{
  protected:
    LEDSinkTest() 
      : mTestSink(mNrSideLeds, mNrTopLeds, mResultStrip)
    {

    }
    
    class MockLedStrip : public LEDStrip
    {
      public:
	MockLedStrip()
	  : mNrCalls(0)
	{
	}

	void SetContents(const std::vector<RGBColor> &pContents)
	{
	  ++mNrCalls;
	  mContents = pContents;
	}

	std::vector<RGBColor> mContents;
	int mNrCalls;
    } mResultStrip;

    LEDSink mTestSink;
    
    static constexpr uint32_t GetNrLeds() { return 2 * mNrSideLeds + mNrTopLeds; }
    static const uint32_t mNrSideLeds = 14, mNrTopLeds = 25;
};

TEST_F(LEDSinkTest, ConstrucorDoesNotFillStrip)
{
  // Arrange
  // Act
  // Assert

  ASSERT_EQ(0, mResultStrip.mNrCalls);
}

TEST_F(LEDSinkTest, FlushFillsStrip)
{
  // Arrange

  // Act
  mTestSink.Flush();

  // Assert
  ASSERT_EQ( 1, mResultStrip.mNrCalls);
  ASSERT_EQ( GetNrLeds(), mResultStrip.mContents.size());
}

TEST_F(LEDSinkTest, SetColorDoesSomething)
{
  // Arrange
  mTestSink.SetColor( { 0, 0, 100., 100. }, {0xff, 0xff, 0xff });

  // Act
  mTestSink.Flush(); // flush again to propagate results

  // Assert
  bool foundSomething = false;
  for (uint32_t i = 0; i < GetNrLeds(); ++i)
  {
    if (mResultStrip.mContents[i] != RGBColor())
    {
      foundSomething = true;
      break;
    }
  }

  ASSERT_TRUE(foundSomething);
}

TEST_F(LEDSinkTest, FlushClearsContents)
{
  // Arrange
  mTestSink.SetColor( { 0, 0, 100., 100. }, {0xff, 0xff, 0xff });

  // Act
  mTestSink.Flush(); 
  mTestSink.Flush(); // flush again to propagate results

  // Assert
  for (uint32_t i = 0; i < GetNrLeds(); ++i)
  {
    EXPECT_PRED_FORMAT3(AssertStripContents, RGBColor() , mResultStrip.mContents[i], i);
  }
}

TEST_F(LEDSinkTest, CoordinateConversion_1)
{
  // Arrange
  const RGBColor testColor = { 1, 2, 3};
  const RelativeRect testRect = { 75, 66.6, 100, 100 };
  const uint32_t expectedStart = 0, expectedEnd = 4;

  // Act
  mTestSink.SetColor(testRect, testColor);
  mTestSink.Flush();


  for (uint32_t i = 0; i < GetNrLeds(); ++i)
  {
    if (i >= expectedStart && i <= expectedEnd)
    {
      EXPECT_PRED_FORMAT3(AssertStripContents, testColor , mResultStrip.mContents[i], i);
    }
    else
    {
      EXPECT_PRED_FORMAT3(AssertStripContents, RGBColor() , mResultStrip.mContents[i], i);
    }
  }
}

TEST_F(LEDSinkTest, CoordinateConversion_2)
{
  // Arrange
  const RGBColor testColor = { 1, 2, 3};
  const RelativeRect testRect = { 75, 33.3, 100, 66.6 };
  const uint32_t expectedStart = 5, expectedEnd = 8;

  // Act
  mTestSink.SetColor(testRect, testColor);
  mTestSink.Flush();


  for (uint32_t i = 0; i < GetNrLeds(); ++i)
  {
    if (i >= expectedStart && i <= expectedEnd)
    {
      EXPECT_PRED_FORMAT3(AssertStripContents, testColor , mResultStrip.mContents[i], i);
    }
    else
    {
      EXPECT_PRED_FORMAT3(AssertStripContents, RGBColor() , mResultStrip.mContents[i], i);
    }
  }
}

TEST_F(LEDSinkTest, CoordinateConversion_3)
{
  // Arrange
  const RGBColor testColor = { 1, 2, 3};
  const RelativeRect testRect = { 80, 0, 100, 33.3 };
  const uint32_t expectedStart = 9, expectedEnd = 18;

  // Act
  mTestSink.SetColor(testRect, testColor);
  mTestSink.Flush();


  for (uint32_t i = 0; i < GetNrLeds(); ++i)
  {
    if (i >= expectedStart && i <= expectedEnd)
    {
      EXPECT_PRED_FORMAT3(AssertStripContents, testColor , mResultStrip.mContents[i], i);
    }
    else
    {
      EXPECT_PRED_FORMAT3(AssertStripContents, RGBColor() , mResultStrip.mContents[i], i);
    }
  }
}

TEST_F(LEDSinkTest, CoordinateConversion_4)
{
  // Arrange
  const RGBColor testColor = { 1, 2, 3};
  const RelativeRect testRect = { 0, 0, 20, 33.3 };
  const uint32_t expectedStart = 34, expectedEnd = 43;

  // Act
  mTestSink.SetColor(testRect, testColor);
  mTestSink.Flush();


  for (uint32_t i = 0; i < GetNrLeds(); ++i)
  {
    if (i >= expectedStart && i <= expectedEnd)
    {
      EXPECT_PRED_FORMAT3(AssertStripContents, testColor , mResultStrip.mContents[i], i);
    }
    else
    {
      EXPECT_PRED_FORMAT3(AssertStripContents, RGBColor() , mResultStrip.mContents[i], i);
    }
  }

}
