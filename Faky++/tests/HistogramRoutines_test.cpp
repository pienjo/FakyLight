#include <gtest/gtest.h>
#include "../HistogramRoutines.h"

using namespace HistogramRoutines;

::testing::AssertionResult AssertTableContents(const char *m_expr, const char *n_expr, const char *index_expr, uint32_t m, uint32_t n, int index)
{
  if (m == n)
    return ::testing::AssertionSuccess();

  return ::testing::AssertionFailure() 
     << m_expr << " ( " << m <<") and " 
     << n_expr << " ( " << n <<") differ (index "<< index <<")";
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

  for (int i = 0; i < ImageStatistics::HueHistogramSize; ++i)
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
  ASSERT_EQ(15000, stats.mHueHistogramTotal);
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

  for (int i = 0; i < ImageStatistics::HueHistogramSize; ++i)
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
  ASSERT_EQ(15000, stats.mHueHistogramTotal);
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

  for (int i = 0; i < ImageStatistics::HueHistogramSize; ++i)
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
  ASSERT_EQ(15000, stats.mHueHistogramTotal);
}
