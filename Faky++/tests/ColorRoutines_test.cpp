#include <gtest/gtest.h>

#include "../ColorRoutines.h"
#include <algorithm>

using namespace ColorRoutines;

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

TEST(RGBtoHSV, FullRed)
{
  // arrange
  uint8_t r = 255, g = 0, b = 0;
  uint8_t h, s, v;

  // act
  RGBtoHSV(r, g, b, h, s, v);

  // Assert
  ASSERT_EQ(255, v); 
  ASSERT_EQ(254, s);
  ASSERT_EQ(0, h);
}

TEST(RGBtoHSV, FullYellow)
{
  // arrange
  uint8_t r = 255, g = 255, b = 0;
  uint8_t h, s, v;

  // act
  RGBtoHSV(r, g, b, h, s, v);

  // Assert
  ASSERT_EQ(255, v); 
  ASSERT_EQ(254, s);
  ASSERT_EQ(42, h);
}

TEST(RGBtoHSV, FullGreen)
{
  // arrange
  uint8_t r = 0, g = 255, b = 0;
  uint8_t h, s, v;

  // act
  RGBtoHSV(r, g, b, h, s, v);

  // Assert
  ASSERT_EQ(255, v); 
  ASSERT_EQ(254, s);
  ASSERT_EQ(84, h);
}

TEST(RGBtoHSV, FullCyan)
{
  // arrange
  uint8_t r = 0, g = 255, b = 255;
  uint8_t h, s, v;

  // act
  RGBtoHSV(r, g, b, h, s, v);

  // Assert
  ASSERT_EQ(255, v); 
  ASSERT_EQ(254, s);
  ASSERT_EQ(126, h);
}

TEST(RGBtoHSV, FullBlue)
{
  // arrange
  uint8_t r = 0, g = 0, b = 255;
  uint8_t h, s, v;

  // act
  RGBtoHSV(r, g, b, h, s, v);

  // Assert
  ASSERT_EQ(255, v); 
  ASSERT_EQ(254, s);
  ASSERT_EQ(168, h);
}

TEST(RGBtoHSV, FullMagenta)
{
  // arrange
  uint8_t r = 255, g = 0, b = 255;
  uint8_t h, s, v;

  // act
  RGBtoHSV(r, g, b, h, s, v);

  // Assert
  ASSERT_EQ(255, v); 
  ASSERT_EQ(254, s);
  ASSERT_EQ(210, h);
}

TEST(RGBtoHSV, HueRange)
{
  // arrange
  uint8_t min = 255;
  uint8_t max = 0;
  uint8_t dummy_s, dummy_v;

  // act
  for (int componentValue = 0; componentValue <256; ++componentValue)
  {
    uint8_t contraValue = 255 - (uint8_t) componentValue;
    
    uint8_t h1,h2, h3;

    RGBtoHSV(componentValue, contraValue, 0, h1, dummy_s, dummy_v);
    RGBtoHSV(componentValue, 0, contraValue, h2, dummy_s, dummy_v);
    RGBtoHSV(0, componentValue, contraValue, h3, dummy_s, dummy_v);

    min = std::min(h1, min);
    min = std::min(h2, min);
    min = std::min(h3, min);

    max = std::max(h1, max);
    max = std::max(h2, max);
    max = std::max(h3, max);
  }

  // Assert
  ASSERT_EQ(0, min);
  ASSERT_EQ(HUE_MAX, max);
}
TEST(HSVtoRGB, FullRed)
{
  // arrange
  uint8_t r, g, b;
  uint8_t h = 0, s = 255, v = 255;

  // act
  HSVtoRGB(h, s, v, r, g, b);

  // assert
  ASSERT_EQ(255, r);
  ASSERT_EQ(0, g);
  ASSERT_EQ(0, b);
}

TEST(HSVtoRGB, FullYellow)
{
  // arrange
  uint8_t r, g, b;
  const uint8_t h = 42, s = 255, v = 255;

  // act
  HSVtoRGB(h, s, v, r, g, b);

  // assert
  ASSERT_EQ(254, r);
  ASSERT_EQ(255, g);
  ASSERT_EQ(0, b);
}

TEST(HSVtoRGB, FullGreen)
{
  // arrange
  uint8_t r, g, b;
  const uint8_t h = 84, s = 255, v = 255;

  // act
  HSVtoRGB(h, s, v, r, g, b);

  // assert
  ASSERT_EQ(0, r);
  ASSERT_EQ(255, g);
  ASSERT_EQ(0, b);
}

TEST(HSVtoRGB, FullCyan)
{
  // arrange
  uint8_t r, g, b;
  const uint8_t h = 126, s = 255, v = 255;

  // act
  HSVtoRGB(h, s, v, r, g, b);

  // assert
  ASSERT_EQ(0, r);
  ASSERT_EQ(254, g);
  ASSERT_EQ(255, b);
}

TEST(HSVtoRGB, FullBlue)
{
  // arrange
  uint8_t r, g, b;
  const uint8_t h = 168, s = 255, v = 255;

  // act
  HSVtoRGB(h, s, v, r, g, b);

  // assert
  ASSERT_EQ(0, r);
  ASSERT_EQ(0, g);
  ASSERT_EQ(255, b);
}

TEST(HSVtoRGB, FullMagenta)
{
  // arrange
  uint8_t r, g, b;
  const uint8_t h = 210, s = 255, v = 255;

  // act
  HSVtoRGB(h, s, v, r, g, b);

  // assert
  ASSERT_EQ(255, r);
  ASSERT_EQ(0, g);
  ASSERT_EQ(254, b);
}

TEST(ImageStatistics, Constructor)
{
  // Arrange

  // Act
  ImageStatistics stats;

  // Assert
  for (int i = 0; i < ImageStatistics::HueHistogramSize; ++i)
    ASSERT_EQ(0, stats.mHueHistogram[i]);

  for (int i = 0; i < 256; ++i)
    ASSERT_EQ(0, stats.mValueHistogram[i]);
   
  ASSERT_EQ(0, stats.mHueHistogramTotal);
  ASSERT_EQ(0, stats.mAverageSaturation);
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
  ImageStatistics stats = GetImageStatistics(testImage, Rect{ 0, 0, 3,1 });

  // Assert
  ASSERT_EQ(0, stats.mAverageSaturation);
  ASSERT_EQ(1, stats.mValueHistogram[255]);
  ASSERT_EQ(1, stats.mValueHistogram[0x7f]);
  ASSERT_EQ(1, stats.mValueHistogram[0]);
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
  ImageStatistics stats = GetImageStatistics(testImage, Rect{ 0, 0, 3,1 });

  // Assert
  ASSERT_EQ(211, stats.mAverageSaturation);
  ASSERT_EQ(2, stats.mValueHistogram[255]);
  ASSERT_EQ(1, stats.mValueHistogram[0x7f]);
  
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
  ImageStatistics stats = GetImageStatistics(testImage, Rect{ 0, 0, 3,1 });

  // Assert
  ASSERT_EQ(211, stats.mAverageSaturation);
  ASSERT_EQ(2, stats.mValueHistogram[255]);
  ASSERT_EQ(1, stats.mValueHistogram[0x7f]);
  
  for (int h = 1; h < ImageStatistics::HueHistogramSize; ++h)
  {
    if (stats.mHueHistogram[h] > 0)
    {
      ASSERT_EQ(stats.mHueHistogramTotal, stats.mHueHistogram[h]);
      ASSERT_EQ( 84, h);
    }
  }
}
