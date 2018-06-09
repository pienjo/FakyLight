#include <gtest/gtest.h>
#include "../ImageRoutines.h"

class ImageRoutinesTest : public ::testing::Test
{
  protected:
    static const uint32_t mCanvasWidth;
    static const uint32_t mCanvasHeight;

    ImageRoutinesTest()
    {
      mCanvasImage.Claim( mCanvasWidth, mCanvasHeight);
      memset(mCanvasImage.data(), 0, mCanvasImage.length());
    }

    Image mCanvasImage;
};

const uint32_t ImageRoutinesTest::mCanvasHeight = 100;
const uint32_t ImageRoutinesTest::mCanvasWidth = 200;

using ColorRoutines::RGBColor;

namespace {
  ::testing::AssertionResult AssertTableContents(const char *m_expr, const char *n_expr, const char *index_expr, uint32_t m, uint32_t n, int index)
  {
    if (m == n)
      return ::testing::AssertionSuccess();

    return ::testing::AssertionFailure() 
       << m_expr << " ( " << m <<") and " 
       << n_expr << " ( " << n <<") differ (index "<< index <<")";
  }
}

TEST_F(ImageRoutinesTest,SetRect_White_OnePixel)
{
  // Arrange
  const Rect rect = { 0,0,1,1 };
  RGBColor white = { 255, 255, 255 };

  // Act
  ImageRoutines::SetRect( mCanvasImage, rect, white);

  // Assert
  const uint8_t *d = mCanvasImage.data();
  uint32_t l = mCanvasImage.length();

  EXPECT_PRED_FORMAT3( AssertTableContents, 255, d[0], 0);
  EXPECT_PRED_FORMAT3( AssertTableContents, 255, d[1], 1);
  EXPECT_PRED_FORMAT3( AssertTableContents, 255, d[2], 2);

  // Remainder of the image is black
  for (uint32_t i = 3; i < l; ++i)
  {
    EXPECT_PRED_FORMAT3( AssertTableContents, 0, d[i], i);
  }
}

TEST_F(ImageRoutinesTest,SetRect_Red_OneRow)
{
  // Arrange
  const Rect rect = { 0,0,mCanvasWidth, 1};
  RGBColor red = { 255, 0, 0};

  // Act
  ImageRoutines::SetRect( mCanvasImage, rect, red);

  // Assert
  const uint8_t *d = mCanvasImage.data();
  uint32_t l = mCanvasImage.length();

  // First line is red
  for (uint32_t i = 0; i < mCanvasWidth; ++i)
  {
    EXPECT_PRED_FORMAT3( AssertTableContents, 255, d[3*i], 3*i);
    EXPECT_PRED_FORMAT3( AssertTableContents, 0, d[3*i+1], 3*i+1);
    EXPECT_PRED_FORMAT3( AssertTableContents, 0, d[3*i+2], 3*i+2);
  }
  // Remainder of the image is black
  for (uint32_t i = 3 * mCanvasWidth; i < l; ++i)
  {
    EXPECT_PRED_FORMAT3( AssertTableContents, 0, d[i], i);
  }
}

TEST_F(ImageRoutinesTest,SetRect_Green_OneColumn)
{
  // Arrange
  const Rect rect = { 0,0,1, mCanvasHeight };
  RGBColor green = { 0, 255, 0};

  // Act
  ImageRoutines::SetRect( mCanvasImage, rect, green);

  // Assert
  const uint8_t *d = mCanvasImage.data();

  uint32_t i = 0;

  for (uint32_t y = 0; y < mCanvasHeight; ++y)
  {
    // First pixel green
    EXPECT_PRED_FORMAT3( AssertTableContents, 0, d[i], i); i++;
    EXPECT_PRED_FORMAT3( AssertTableContents, 255, d[i], i); i++;
    EXPECT_PRED_FORMAT3( AssertTableContents, 0, d[i], i); i++;
  
    // Remainder of the row is black
    for (uint32_t x = 1; x < mCanvasWidth; ++x)
    {
      EXPECT_PRED_FORMAT3( AssertTableContents, 0, d[i], i); i++;
      EXPECT_PRED_FORMAT3( AssertTableContents, 0, d[i], i); i++;
      EXPECT_PRED_FORMAT3( AssertTableContents, 0, d[i], i); i++;
    }
  }
}

TEST_F(ImageRoutinesTest,SetRect_Blue_ColumnClips)
{
  // Arrange
  const Rect rect = { 10,0,11, 2 * mCanvasHeight };
  RGBColor blue = { 0, 0, 255};

  // Act
  ImageRoutines::SetRect( mCanvasImage, rect, blue);

  // Assert
  const uint8_t *d = mCanvasImage.data();

  uint32_t i = 0;

  for (uint32_t y = 0; y < mCanvasHeight; ++y)
  {
    // First 10 pixels black
    uint32_t x;
    for (x = 0; x < 10; ++x)
    {
      EXPECT_PRED_FORMAT3( AssertTableContents, 0, d[i], i); i++;
      EXPECT_PRED_FORMAT3( AssertTableContents, 0, d[i], i); i++;
      EXPECT_PRED_FORMAT3( AssertTableContents, 0, d[i], i); i++;
    }
    
    // One blue pixel
    EXPECT_PRED_FORMAT3( AssertTableContents, 0, d[i], i); i++;
    EXPECT_PRED_FORMAT3( AssertTableContents, 0, d[i], i); i++;
    EXPECT_PRED_FORMAT3( AssertTableContents, 255, d[i], i); i++;
  
    // Remainder of the row is black
    for (++x; x < mCanvasWidth; ++x)
    {
      EXPECT_PRED_FORMAT3( AssertTableContents, 0, d[i], i); i++;
      EXPECT_PRED_FORMAT3( AssertTableContents, 0, d[i], i); i++;
      EXPECT_PRED_FORMAT3( AssertTableContents, 0, d[i], i); i++;
    }
  }
}

TEST_F(ImageRoutinesTest,SetRect_Magenta_RowClips_right)
{
  // Arrange
  const Rect rect = { 0,10,2 * mCanvasWidth, 11};
  RGBColor magenta = { 255, 0, 255};

  // Act
  ImageRoutines::SetRect( mCanvasImage, rect, magenta);

  // Assert
  const uint8_t *d = mCanvasImage.data();
  uint32_t l = mCanvasImage.length();

  uint32_t i;

  // First 10 lines are black
  for ( i = 0; i < 10 * 3 * mCanvasWidth;  ++i)
  {
    EXPECT_PRED_FORMAT3( AssertTableContents, 0, d[i], i);
  }

  // One magenta line
  for (uint32_t x = 0; x < mCanvasWidth; ++x)
  {
    EXPECT_PRED_FORMAT3( AssertTableContents, 255, d[i], i); i++;
    EXPECT_PRED_FORMAT3( AssertTableContents, 0, d[i], i); i++;
    EXPECT_PRED_FORMAT3( AssertTableContents, 255, d[i], i); i++;
  }

  // Remainder of the image is black
  for (; i < l; ++i)
  {
    EXPECT_PRED_FORMAT3( AssertTableContents, 0, d[i], i);
  }
}

TEST_F(ImageRoutinesTest,SetRect_Cyan_RowClips_left)
{
  // Arrange
  const Rect rect = { -2,10, mCanvasWidth, 11};
  RGBColor magenta = { 255, 0, 255};

  // Act
  ImageRoutines::SetRect( mCanvasImage, rect, magenta);

  // Assert
  const uint8_t *d = mCanvasImage.data();
  uint32_t l = mCanvasImage.length();

  uint32_t i;

  // First 10 lines are black
  for ( i = 0; i < 10 * 3 * mCanvasWidth;  ++i)
  {
    EXPECT_PRED_FORMAT3( AssertTableContents, 0, d[i], i);
  }

  // One cyan line
  for (uint32_t x = 0; x < mCanvasWidth; ++x)
  {
    EXPECT_PRED_FORMAT3( AssertTableContents, 255, d[i], i); i++;
    EXPECT_PRED_FORMAT3( AssertTableContents, 0, d[i], i); i++;
    EXPECT_PRED_FORMAT3( AssertTableContents, 255, d[i], i); i++;
  }

  // Remainder of the image is black
  for (; i < l; ++i)
  {
    EXPECT_PRED_FORMAT3( AssertTableContents, 0, d[i], i);
  }
}
