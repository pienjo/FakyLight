#include <gtest/gtest.h>

#include "FileSource.h"

TEST(FileSource, constructor_single)
{
  // Arrange

  // Act
  FileSource test("Foo.pnm");

  // Assert
  ASSERT_EQ(1U, test.size());
}

TEST(FileSource, constructor_multiple)
{
  // Arrange
  const std::string files_arr[] = { "Foo.pnm", "bar.pnm" };
  std::vector<std::string> files( files_arr, files_arr + sizeof(files_arr) / sizeof(std::string));

  // Act
  FileSource test(files);

  // Assert
  ASSERT_EQ(2U, test.size());
}

TEST(FileSource, fileNotFound)
{
  // Arrange 
  FileSource test("Foo.pnm");

  // Act / assert
  ASSERT_ANY_THROW( test.FetchImage());
}

TEST(FileSource, fileFound)
{
  // Arrange 
  FileSource test("../../TestImages/bamigo.ppm");
  
  Image destinationImage;

  // Act
  destinationImage = test.FetchImage();
  
  // Assert
  ASSERT_NE(0u, destinationImage.length());
  ASSERT_NE(nullptr, destinationImage.data());
}

TEST(FileSource, dimensionsCorrect)
{
  // Arrange 
  FileSource test("../../TestImages/100nl.ppm");
  
  Image destinationImage;

  // Act
  destinationImage = test.FetchImage();

  // Assert
  ASSERT_EQ(320u, destinationImage.Width());
  ASSERT_EQ(180u, destinationImage.Height());
}

TEST(FileSource, topLeftPixel)
{
  // Arrange 
  FileSource test("../../TestImages/100nl.ppm");
  
  Image destinationImage;

  // Act
  destinationImage = test.FetchImage();
  const uint8_t *d = destinationImage.data();

  // Assert
  ASSERT_EQ(0x11, d[0]);
  ASSERT_EQ(0x0f, d[1]);
  ASSERT_EQ(0x12, d[2]);
}

TEST(FileSource, bottomRightPixel)
{
  // Arrange 
  FileSource test("../../TestImages/100nl.ppm");
  
  Image destinationImage;

  // Act
  destinationImage = test.FetchImage();
  const uint8_t *d = destinationImage.data() + destinationImage.length() - 3;

  // Assert
  ASSERT_EQ(0x11, d[0]);
  ASSERT_EQ(0x0f, d[1]);
  ASSERT_EQ(0x12, d[2]);
}

TEST(FileSource, fileFound_secondInvocation)
{
  // Arrange 
  FileSource test("../../TestImages/bamigo.ppm");
  
  ASSERT_NO_THROW( test.FetchImage() );

  // Act
  ASSERT_ANY_THROW( test.FetchImage());
}


