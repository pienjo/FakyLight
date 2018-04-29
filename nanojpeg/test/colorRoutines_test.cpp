#include <gtest/gtest.h>

extern "C" {
  #include "../colorRoutines.h"
}

TEST(RGBtoHSV, FullRed)
{
  // arrange
  uint8_t r = 255, g = 0, b = 0;
  uint8_t h, s, v;

  // act
  RGBtoHSV(r,g,b, &h,&s, &v);

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
  RGBtoHSV(r,g,b, &h,&s, &v);

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
  RGBtoHSV(r,g,b, &h,&s, &v);

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
  RGBtoHSV(r,g,b, &h,&s, &v);

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
  RGBtoHSV(r,g,b, &h,&s, &v);

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
  RGBtoHSV(r,g,b, &h,&s, &v);

  // Assert
  ASSERT_EQ(255, v); 
  ASSERT_EQ(254, s);
  ASSERT_EQ(210, h);
}

TEST(HSVtoRGB, FullRed)
{
  // arrange
  uint8_t r,g,b;
  uint8_t h = 0, s = 255, v = 255;

  // act
  HSVtoRGB(h,s,v, &r, &g, &b);

  // assert
  ASSERT_EQ(255, r);
  ASSERT_EQ(0, g);
  ASSERT_EQ(0, b);
}

TEST(HSVtoRGB, FullYellow)
{
  // arrange
  uint8_t r,g,b;
  const uint8_t h = 42, s = 255, v = 255;

  // act
  HSVtoRGB(h,s,v, &r, &g, &b);

  // assert
  ASSERT_EQ(254, r);
  ASSERT_EQ(255, g);
  ASSERT_EQ(0, b);
}

TEST(HSVtoRGB, FullGreen)
{
  // arrange
  uint8_t r,g,b;
  const uint8_t h = 84, s = 255, v = 255;

  // act
  HSVtoRGB(h,s,v, &r, &g, &b);

  // assert
  ASSERT_EQ(0, r);
  ASSERT_EQ(255, g);
  ASSERT_EQ(0, b);
}

TEST(HSVtoRGB, FullCyan)
{
  // arrange
  uint8_t r,g,b;
  const uint8_t h = 126, s = 255, v = 255;

  // act
  HSVtoRGB(h,s,v, &r, &g, &b);

  // assert
  ASSERT_EQ(0, r);
  ASSERT_EQ(254, g);
  ASSERT_EQ(255, b);
}

TEST(HSVtoRGB, FullBlue)
{
  // arrange
  uint8_t r,g,b;
  const uint8_t h = 168, s = 255, v = 255;

  // act
  HSVtoRGB(h,s,v, &r, &g, &b);

  // assert
  ASSERT_EQ(0, r);
  ASSERT_EQ(0, g);
  ASSERT_EQ(255, b);
}

TEST(HSVtoRGB, FullMagenta)
{
  // arrange
  uint8_t r,g,b;
  const uint8_t h = 210, s = 255, v = 255;

  // act
  HSVtoRGB(h,s,v, &r, &g, &b);

  // assert
  ASSERT_EQ(255, r);
  ASSERT_EQ(0, g);
  ASSERT_EQ(254, b);
}
