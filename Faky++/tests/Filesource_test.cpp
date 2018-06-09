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
  ASSERT_NO_THROW( {
  destinationImage = test.FetchImage();
  } );

  ASSERT_NE(0u, destinationImage.length());
  ASSERT_NE(nullptr, destinationImage.data());
}

TEST(FileSource, fileFound_secondInvocation)
{
  // Arrange 
  FileSource test("../../TestImages/bamigo.ppm");
  
  ASSERT_NO_THROW( test.FetchImage() );

  // Act
  ASSERT_ANY_THROW( test.FetchImage());
}


