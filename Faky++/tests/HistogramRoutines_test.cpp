#include <gtest/gtest.h>
#include "../HistogramRoutines.h"

using namespace HistogramRoutines;

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

TEST(SmoothHistogram, Pulse_center)
{
  // Arrange
  ImageStatistics stats;
  const uint32_t kernelSize = 7; 
  stats.mHueHistogram[ImageStatistics::HueHistogramSize/2] = 1000;
  stats.mHueHistogramTotal = 1000;
  // Act
  SmoothHueHistogram(stats, kernelSize);

  // Assert

  for (uint32_t i = 0; i < ImageStatistics::HueHistogramSize; ++i)
  {
    if (i >= ImageStatistics::HueHistogramSize/2 - kernelSize && i <= ImageStatistics::HueHistogramSize/2+kernelSize)
    {
      EXPECT_PRED_FORMAT3(AssertTableContents, 1000, stats.mHueHistogram[i], i);
    }
    else
    {
      EXPECT_PRED_FORMAT3(AssertTableContents, 0, stats.mHueHistogram[i], i);
    }
  }
  ASSERT_EQ(15000U, stats.mHueHistogramTotal);
}

TEST(SmoothHistogram, Pulse_start)
{
  // Arrange
  ImageStatistics stats;
  const uint32_t kernelSize = 7; 
  stats.mHueHistogram[0] = 1000;
  stats.mHueHistogramTotal = 1000;
  // Act
  SmoothHueHistogram(stats, kernelSize);

  // Assert

  for (uint32_t i = 0; i < ImageStatistics::HueHistogramSize; ++i)
  {
    if (i >= ImageStatistics::HueHistogramSize - kernelSize || i <= kernelSize)
    {
      EXPECT_PRED_FORMAT3(AssertTableContents, 1000, stats.mHueHistogram[i], i);
    }
    else
    {
      EXPECT_PRED_FORMAT3(AssertTableContents, 0, stats.mHueHistogram[i], i);
    }
  }
  ASSERT_EQ(15000U, stats.mHueHistogramTotal);
}

TEST(SmoothHistogram, Pulse_end)
{
  // Arrange
  ImageStatistics stats;
  const uint32_t kernelSize = 7; 
  stats.mHueHistogram[ColorRoutines::HUE_MAX] = 1000;
  stats.mHueHistogramTotal = 1000;
  // Act
  SmoothHueHistogram(stats, kernelSize);

  // Assert

  for (uint32_t i = 0; i < ImageStatistics::HueHistogramSize; ++i)
  {
    if (i >= ImageStatistics::HueHistogramSize - kernelSize - 1 || i <= kernelSize - 1)
    {
      EXPECT_PRED_FORMAT3(AssertTableContents, 1000, stats.mHueHistogram[i], i);
    }
    else
    {
      EXPECT_PRED_FORMAT3(AssertTableContents, 0, stats.mHueHistogram[i], i);
    }
  }
  ASSERT_EQ(15000U, stats.mHueHistogramTotal);
}
TEST(GetMode_Hue, modeBegin)
{
  // Arrange
  ImageStatistics stats;
  stats.mHueHistogram[0] = 1234;

  // Act
  size_t mode = GetMode_Hue(stats);

  // Assert
  ASSERT_EQ(0U, mode);
}

TEST(GetMode_Hue, modeEnd)
{
  // Arrange
  ImageStatistics stats;
  stats.mHueHistogram[ColorRoutines::HUE_MAX] = 1234;

  // Act
  size_t mode = GetMode_Hue(stats);

  // Assert
  ASSERT_EQ(ColorRoutines::HUE_MAX, mode);
}

TEST(GetMode_Hue, modeMiddle)
{
  // Arrange
  ImageStatistics stats;
  stats.mHueHistogram[ColorRoutines::HUE_MAX/2] = 1234;
  stats.mHueHistogram[ColorRoutines::HUE_MAX] = 345;

  // Act
  size_t mode = GetMode_Hue(stats);

  // Assert
  ASSERT_EQ((size_t) ColorRoutines::HUE_MAX/2, mode);
}

TEST(GetModal_Value, flatline)
{
  // Arrange
  ImageStatistics stats;
  
  for (int i = 0; i < 256; ++ i)
    stats.mValueHistogram[i] = 1;

  // Act
  size_t modal = GetModal_Value(stats);

  // Assert
  ASSERT_EQ(127U, modal);
}

TEST(GetModal_Value, empty)
{
  // Arrange
  ImageStatistics stats;
  
  // Act
  size_t modal = GetModal_Value(stats);

  // Assert
  ASSERT_EQ(0U, modal);
}

TEST(GetModal_Value, singlePeak)
{
  // Arrange
  ImageStatistics stats;
  
  for (int i = 120; i < 127; ++ i)
    stats.mValueHistogram[i] = 100;

  // Act
  size_t modal = GetModal_Value(stats);

  // Assert
  ASSERT_EQ(123U, modal);
}

TEST(GetModal_Value, twoPeaks)
{
  // Arrange
  ImageStatistics stats;
  
  for (int i = 0; i < 256; ++ i)
    stats.mValueHistogram[i] = 1;

  stats.mValueHistogram[64] = 10000;
  stats.mValueHistogram[192] = 10000;

  // Act
  size_t modal = GetModal_Value(stats);

  // Assert
  ASSERT_EQ(127U, modal);
}