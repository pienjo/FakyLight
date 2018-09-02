#include <gtest/gtest.h>

#include "../ColorRoutines.h"
#include <algorithm>

using namespace ColorRoutines;


namespace {

  class TestImage_T : public Image 
  {
    public:
      template<int arraySize>
      TestImage_T(const uint8_t (&testImageContents)[arraySize])
      {
        static_assert( arraySize % 3 == 0, "Arraysize must be a multiple of 3");

        Claim(arraySize /3, 1);
        memcpy(data(), testImageContents, arraySize);
      }
  };

  ::testing::AssertionResult AssertTableContents(const char *m_expr, const char *n_expr, const char *index_expr, bool m, bool n, int index)
  {
    if (m == n)
      return ::testing::AssertionSuccess();

    return ::testing::AssertionFailure() 
       << m_expr << " ( " << m <<") and " 
       << n_expr << " ( " << n <<") differ (index "<< index <<")";
  }
}

TEST(RGBtoHSV, FullRed)
{
  // arrange
  const RGBColor rgb = { 255,0,0 };
  HSVColor hsv;

  // act
  hsv = RGBtoHSV(rgb);

  // Assert
  ASSERT_EQ(255, hsv.v); 
  ASSERT_EQ(254, hsv.s);
  ASSERT_EQ(0, hsv.h);
}
TEST(RGBtoHSV, FullYellow)
{
  // arrange
  const RGBColor rgb = { 255,255,0 };
  HSVColor hsv;

  // act
  hsv = RGBtoHSV(rgb);

  // Assert
  ASSERT_EQ(255, hsv.v); 
  ASSERT_EQ(254, hsv.s);
  ASSERT_EQ(42, hsv.h);
}

TEST(RGBtoHSV, FullGreen)
{
  // arrange
  const RGBColor rgb = { 0,255,0 };
  HSVColor hsv;

  // act
  hsv = RGBtoHSV(rgb);

  // Assert
  ASSERT_EQ(255, hsv.v); 
  ASSERT_EQ(254, hsv.s);
  ASSERT_EQ(84, hsv.h);
}

TEST(RGBtoHSV, FullCyan)
{
  // arrange
  const RGBColor rgb = { 0,255,255 };
  HSVColor hsv;

  // act
  hsv = RGBtoHSV(rgb);

  // Assert
  ASSERT_EQ(255, hsv.v); 
  ASSERT_EQ(254, hsv.s);
  ASSERT_EQ(126, hsv.h);
}

TEST(RGBtoHSV, FullBlue)
{
  // arrange
  const RGBColor rgb = { 0,0,255 };
  HSVColor hsv;

  // act
  hsv = RGBtoHSV(rgb);
  
  // Assert
  ASSERT_EQ(255, hsv.v); 
  ASSERT_EQ(254, hsv.s);
  ASSERT_EQ(168, hsv.h);
}

TEST(RGBtoHSV, FullMagenta)
{
  // arrange
  const RGBColor rgb = { 255,0,255 };
  HSVColor hsv;

  // act
  hsv = RGBtoHSV(rgb);

  // Assert
  ASSERT_EQ(255, hsv.v); 
  ASSERT_EQ(254, hsv.s);
  ASSERT_EQ(210, hsv.h);
}

TEST(RGBtoHSV, HueRange)
{
  // arrange
  uint8_t min = 255;
  uint8_t max = 0;
  
  std::vector<bool> visited;
  visited.resize(256, false);

  // act
  for (int componentValue = 0; componentValue <256; ++componentValue)
  {
    RGBColor rgb = { (uint8_t) componentValue, 255, 0 };
    HSVColor hsv;

    hsv = RGBtoHSV(rgb);
    min = std::min(hsv.h, min);
    max = std::max(hsv.h, max);
    visited[hsv.h] = true;

    rgb = { 255, (uint8_t) componentValue, 0 };
    
    hsv = RGBtoHSV(rgb);
    min = std::min(hsv.h, min);
    max = std::max(hsv.h, max);
    visited[hsv.h] = true;

    rgb = { (uint8_t) componentValue, 0, 255 };
    
    hsv = RGBtoHSV(rgb);
    min = std::min(hsv.h, min);
    max = std::max(hsv.h, max);
    visited[hsv.h] = true;
    
    rgb = { 255, 0, (uint8_t) componentValue };
    
    hsv = RGBtoHSV(rgb);
    min = std::min(hsv.h, min);
    max = std::max(hsv.h, max);
    visited[hsv.h] = true;
    
    rgb = { 0, (uint8_t) componentValue, 255};
    
    hsv = RGBtoHSV(rgb);
    min = std::min(hsv.h, min);
    max = std::max(hsv.h, max);
    visited[hsv.h] = true;

    rgb = { 0, 255, (uint8_t) componentValue};
    
    hsv = RGBtoHSV(rgb);
    min = std::min(hsv.h, min);
    max = std::max(hsv.h, max);
    visited[hsv.h] = true;
  }

  // Assert
  ASSERT_EQ(0, min);
  ASSERT_EQ(HSVColor::HUE_MAX, max);
  for (int i = min; i <= max; ++i)
    EXPECT_PRED_FORMAT3(AssertTableContents, true, visited[i], i);
   
}

TEST(HSVtoRGB, FullRed)
{
  // arrange
  const HSVColor hsv = { 0,255,255 };
  
  // act
  RGBColor rgb = HSVtoRGB(hsv);

  // assert
  ASSERT_EQ(255, rgb.r);
  ASSERT_EQ(0, rgb.g);
  ASSERT_EQ(0, rgb.b);
}

TEST(HSVtoRGB, NearlyRed_1)
{
  // arrange
  const HSVColor hsv = { 1,255,255 };
  
  // act
  RGBColor rgb = HSVtoRGB(hsv);

  // assert
  ASSERT_EQ(255, rgb.r);
  ASSERT_EQ(6, rgb.g);
  ASSERT_EQ(0, rgb.b);
}

TEST(HSVtoRGB, NearlyRed_2)
{
  // arrange
  const HSVColor hsv = { HSVColor::HUE_MAX ,255,255 };
  
  // act
  RGBColor rgb = HSVtoRGB(hsv);

  // assert
  ASSERT_EQ(255, rgb.r);
  ASSERT_EQ(0, rgb.g);
  ASSERT_EQ(9, rgb.b);
}

TEST(HSVtoRGB, FullYellow)
{
  // arrange
  const HSVColor hsv = { 42,255,255 };
  
  // act
  RGBColor rgb = HSVtoRGB(hsv);

  // assert
  ASSERT_EQ(254, rgb.r);
  ASSERT_EQ(255, rgb.g);
  ASSERT_EQ(0, rgb.b);
}

TEST(HSVtoRGB, FullGreen)
{
  // arrange
  const HSVColor hsv = { 84,255,255 };
  
  // act
  RGBColor rgb = HSVtoRGB(hsv);

  // assert
  ASSERT_EQ(0, rgb.r);
  ASSERT_EQ(255, rgb.g);
  ASSERT_EQ(0, rgb.b);
}

TEST(HSVtoRGB, NearlyGreen_1)
{
  // arrange
  const HSVColor hsv = { 85,255,255 };
  
  // act
  RGBColor rgb = HSVtoRGB(hsv);

  // assert
  ASSERT_EQ(0, rgb.r);
  ASSERT_EQ(255, rgb.g);
  ASSERT_EQ(6, rgb.b);
}

TEST(HSVtoRGB, NearlyGreen_2)
{
  // arrange
  const HSVColor hsv = { 83 ,255,255 };
  
  // act
  RGBColor rgb = HSVtoRGB(hsv);

  // assert
  ASSERT_EQ(9, rgb.r);
  ASSERT_EQ(255, rgb.g);
  ASSERT_EQ(0, rgb.b);
}
TEST(HSVtoRGB, FullCyan)
{
  // arrange
  const HSVColor hsv = { 126,255,255 };
  
  // act
  RGBColor rgb = HSVtoRGB(hsv);

  // assert
  ASSERT_EQ(0, rgb.r);
  ASSERT_EQ(254, rgb.g);
  ASSERT_EQ(255, rgb.b);
}

TEST(HSVtoRGB, FullBlue)
{
  // arrange
  const HSVColor hsv = { 168,255,255 };
  
  // act
  RGBColor rgb = HSVtoRGB(hsv);

  // assert
  ASSERT_EQ(0, rgb.r);
  ASSERT_EQ(0, rgb.g);
  ASSERT_EQ(255, rgb.b);
}

TEST(HSVtoRGB, NearlyBlue_1)
{
  // arrange
  const HSVColor hsv = { 169,255,255 };
  
  // act
  RGBColor rgb = HSVtoRGB(hsv);

  // assert
  ASSERT_EQ(6, rgb.r);
  ASSERT_EQ(0, rgb.g);
  ASSERT_EQ(255, rgb.b);
}

TEST(HSVtoRGB, NearlyBlue_2)
{
  // arrange
  const HSVColor hsv = { 167,255,255 };
  
  // act
  RGBColor rgb = HSVtoRGB(hsv);

  // assert
  ASSERT_EQ(0, rgb.r);
  ASSERT_EQ(9, rgb.g);
  ASSERT_EQ(255, rgb.b);
}
TEST(HSVtoRGB, FullMagenta)
{
  // arrange
  const HSVColor hsv = { 210,255,255 };
  
  // act
  RGBColor rgb = HSVtoRGB(hsv);

  // assert
  ASSERT_EQ(255, rgb.r);
  ASSERT_EQ(0, rgb.g);
  ASSERT_EQ(254,rgb.b);
}

TEST(ImageStatistics, Constructor)
{
  // Arrange

  // Act
  ImageStatistics stats;

  // Assert
  for (int i = 0; i < ImageStatistics::HueHistogramSize; ++i)
    ASSERT_EQ(0U, stats.mHueHistogram[i]);

  for (int i = 0; i < 256; ++i)
    ASSERT_EQ(0U, stats.mValueHistogram[i]);
   
  ASSERT_EQ(0U, stats.mHueHistogramTotal);
  ASSERT_EQ(0U, stats.mAverageSaturation);
}
TEST(getHueValueHistogram, ShadesofGray)
{
  // arrange
  const uint8_t testImageContents[] = {
    0xff, 0xff, 0xff, // white
    0x7f, 0x7f, 0x7f, // grey
    0x0 , 0x0,  0x0 , // black 
    };

  TestImage_T testImage(testImageContents);

  // Act
  ImageStatistics stats = GetImageStatistics(testImage, AbsoluteRect{ 0, 0, 3,1 });

  // Assert
  ASSERT_EQ(0, stats.mAverageSaturation);
  ASSERT_EQ(1U, stats.mValueHistogram[255]);
  ASSERT_EQ(1U, stats.mValueHistogram[0x7f]);
  ASSERT_EQ(1U, stats.mValueHistogram[0]);
}
TEST(getHueValueHistogram, ShadesOfRed)
{
  // arrange
  const uint8_t testImageContents[] = {
    0xff, 0x00, 0x00, // Red
    0x7f, 0x00, 0x00, // dark red
    0xff , 0x7f,  0x7f , // pink 
    };

  TestImage_T testImage(testImageContents);

  // Act
  ImageStatistics stats = GetImageStatistics(testImage, AbsoluteRect{ 0, 0, 3,1 });

  // Assert
  ASSERT_EQ(211, stats.mAverageSaturation);
  ASSERT_EQ(2U, stats.mValueHistogram[255]);
  ASSERT_EQ(1U, stats.mValueHistogram[0x7f]);
  
  for (int h = 1; h < ImageStatistics::HueHistogramSize; ++h)
  {
    if (stats.mHueHistogram[h] > 0)
    {
      ASSERT_EQ(stats.mHueHistogramTotal, stats.mHueHistogram[h]);
      ASSERT_EQ( 0, h);
    }
  }
}

TEST(getHueValueHistogram, ShadesOfGreen)
{
  // arrange
  const uint8_t testImageContents[] = {
    0x00, 0xff, 0x00,  // Green
    0x00, 0x7f, 0x00,  // dark green
    0x7f, 0xff , 0x7f, // pastel green
    };

  TestImage_T testImage(testImageContents);

  // Act
  ImageStatistics stats = GetImageStatistics(testImage, AbsoluteRect{ 0, 0, 3,1 });

  // Assert
  ASSERT_EQ(211, stats.mAverageSaturation);
  ASSERT_EQ(2U, stats.mValueHistogram[255]);
  ASSERT_EQ(1U, stats.mValueHistogram[0x7f]);
  
  for (int h = 1; h < ImageStatistics::HueHistogramSize; ++h)
  {
    if (stats.mHueHistogram[h] > 0)
    {
      ASSERT_EQ(stats.mHueHistogramTotal, stats.mHueHistogram[h]);
      ASSERT_EQ( 84, h);
    }
  }
}
