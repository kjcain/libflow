/**
 * @file synchronous_flow_test.cc
 * @author Kyler Cain (kyler.cain@yahoo.com)
 * @brief
 * @version 0.1
 * @date 2021-03-26
 *
 * @copyright (c) 2021, Kyler Cain
 */

#include <gtest/gtest.h>

#include <iostream>

#include "block.h"

namespace cain::flow {

class AdditionBlock : public Block<int, int, int> {
 public:
  void work(int in, int param) override { this->send_downstream(in + param); }
  explicit AdditionBlock(int add_val) : Block<int, int, int>(add_val, this->work) {}
};

class PrintBlock : public Block<int, int, nullptr_t> {
 public:
  PrintBlock() : Block<int, int, nullptr_t>(nullptr) {}
  void work(int in, nullptr_t) override {
    std::cout << in << std::endl;
    this->send_downstream(in);
  }
};

class SumBlock : public Block<int, nullptr_t, int *> {
 public:
  explicit SumBlock(int *acc) : Block<int, nullptr_t, int *>(acc) {}
  void work(int in, int *accumulator) override { *accumulator += in; }
};

TEST(sync, basic) {
  int test_vals[] = {1, 2, 3, 4, 5, 6};

  int add_val = 5;
  int accumulator = 0;

  AdditionBlock *add = new AdditionBlock(add_val);
  PrintBlock *printer = new PrintBlock();
  SumBlock *summation = new SumBlock(&accumulator);

  add->add_downstream(printer);
  printer->add_downstream(summation);

  int check = 0;

  for (auto val : test_vals) {
    add->recieve(val);

    // testing only, replace soon
    // TODO(cain): replace this
    add->do_work();
    printer->do_work();
    summation->do_work();

    check += val + add_val;
  }

  ASSERT_EQ(check, accumulator);
}
}  // namespace cain::flow

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
