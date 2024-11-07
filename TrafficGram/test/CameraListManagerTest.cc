#include <gtest/gtest.h>

#include "../CameraListManager.h"

// Demonstrate some basic assertions.
TEST(CameraListManagerTest, CameraFromJson) {
  // Expect two strings not to be equal.
  EXPECT_STRNE("hello", "world");
  // Expect equality.
  EXPECT_EQ(7 * 6, 42);
}
