#include <gtest/gtest.h>

#include "Image.h"

TEST(Image, constructor_noData)
{
  // Arrange

  // Act
  Image test;

  // Assert
  ASSERT_EQ(nullptr, test.data());
}

TEST(Image, constructor_nolength)
{
  // Arrange

  // Act
  Image test;

  // Assert
  ASSERT_EQ((uint32_t) 0, test.length());
}


TEST(Image, constructor_noWidth)
{
  // Arrange

  // Act
  Image test;

  // Assert
  ASSERT_EQ((uint32_t) 0, test.Width());
}

TEST(Image, constructor_noHeight)
{
  // Arrange

  // Act
  Image test;

  // Assert
  ASSERT_EQ((uint32_t) 0, test.Height());
}

TEST(Image, Claim)
{
  // Arrange
  Image test;

  // Act;
  test.Claim(12,34);

  // Assert
  
  ASSERT_NE(nullptr, test.data());
  ASSERT_EQ((uint32_t) (12 * 34 * 3) , test.length());
}

TEST(Image, copyConstructor)
{
  // Arrange
  Image sourceImage;
  sourceImage.Claim(12,34);
  
  // Act
  Image destinationImage(sourceImage);
  
  // Assert

  ASSERT_EQ(sourceImage.length(), destinationImage.length());
  ASSERT_NE(nullptr, destinationImage.data());
  ASSERT_NE(sourceImage.data(), destinationImage.data());
}

TEST(Image, moveConstructor)
{
  // Arrange
  Image sourceImage;
  sourceImage.Claim(12,34);
  const uint8_t *originalData = sourceImage.data();
  const size_t originalLength = sourceImage.length();

  // Act
  Image destinationImage(std::move(sourceImage));
  
  // Assert
  ASSERT_EQ(originalLength, destinationImage.length());
  ASSERT_EQ(originalData, destinationImage.data());

  ASSERT_EQ((uint32_t) 0, sourceImage.length());
  ASSERT_EQ(nullptr, sourceImage.data());
}

TEST(Image, assignment)
{
  // Arrange
  Image sourceImage, destinationImage;
  sourceImage.Claim(12,34);
  destinationImage.Claim(56,78);

  // Act
  destinationImage = sourceImage;
  
  // Assert

  ASSERT_EQ(sourceImage.length(), destinationImage.length());
  ASSERT_NE(nullptr, destinationImage.data());
  ASSERT_NE(sourceImage.data(), destinationImage.data());
}
TEST(Image, moveAssignment)
{
  // Arrange
  Image sourceImage, destinationImage;
  sourceImage.Claim(12,34);

  // Act
  destinationImage = std::move(sourceImage);

  // Assert
  ASSERT_EQ(nullptr, sourceImage.data());
  ASSERT_EQ((uint32_t) 0, sourceImage.length());
}
