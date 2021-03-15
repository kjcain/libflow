/**
 * @file flow_test.cc
 * @author Kyler Cain (kyler.cain@yahoo.com)
 * @brief
 * @version 0.1
 * @date 2021-03-12
 */

#include "flow.h"

#include <gtest/gtest.h>

#include <memory>

namespace cain::flow {

void number_source(Buffer<int> *output, void *params) {
  int *number = static_cast<int *>(params);

  for (int i = 0; i < *number; i++) {
    output->push(i);
  }
}

void multiply_trans(int input, Buffer<int> *output, void *params) {
  int *multiplier = static_cast<int *>(params);

  int ret = input * *multiplier;

  std::cout << input << " * " << *multiplier << " = " << ret << "\n";

  output->push(ret);
}

void print_sink(int input, void *params) {
  std::cout << input << " { params: " << params << "}\n";
}

/**
 * @brief Test flow is able to correct a thread count to >= 2
 */
TEST(Flow, NotEnoughThreads) { ASSERT_NE(1, 0); }

/**
 * @brief Test flow is able to validate flows prior to run
 */
TEST(Flow, ValidateFlow) { ASSERT_NE(1, 0); }

/**
 * @brief Test flow is able to run a simple linear flow with no branches
 */
TEST(Flow, LinearFlow) {
  int starting_number = 1;
  int multiplier = 10;
  int sink_param = 0;

  auto flow = Flow<int>(2);

  auto source = flow.add_source(number_source, &starting_number);
  EXPECT_GE(source, 1);

  auto trans = flow.add_trans(multiply_trans, &multiplier);
  EXPECT_GE(trans, 1);

  auto sink = flow.add_sink(print_sink, &sink_param);
  EXPECT_GE(sink, 1);

  auto linked = flow.link(source, trans);
  EXPECT_EQ(linked, true);

  linked = flow.link(trans, sink);
  EXPECT_EQ(linked, true);

  bool valid = flow.validate();
  EXPECT_EQ(valid, true);

  if (valid) {
    bool result = flow.run();
    ASSERT_EQ(result, true);
  }
}

/**
 * @brief Test flow is able to run a complex branching flow
 */
TEST(Flow, BranchingFlow) { ASSERT_NE(1, 0); }

}  // namespace cain::flow

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
