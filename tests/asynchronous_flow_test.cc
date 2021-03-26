/**
 * @file asynchronous_flow_test.cc
 * @author Kyler Cain (kyler.cain@yahoo.com)
 * @brief
 * @version 0.1
 * @date 2021-03-26
 */

#include "block.h"

#include <gtest/gtest.h>

namespace cain::flow {
  TEST(async, basic) {
    ASSERT_EQ(1, 0);
  }
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
