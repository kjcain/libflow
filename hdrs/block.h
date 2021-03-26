/**
 * @file block.h
 * @author Kyler Cain (kyler@kylercain.com)
 * @brief
 * @version 0.1
 * @date 2021-03-26
 *
 * @copyright (c) 2021, Kyler Cain
 *
 */

#ifndef HDRS_BLOCK_H_
#define HDRS_BLOCK_H_

#include <iterator>
#include <queue>

#include "buffer.h"

namespace cain::flow {
template <class Tin>
class RecieveBlock {
 protected:
  virtual void recieve(Tin in) {}
};

template <class Tin, class Tout, class Tparam>
class Block : public RecieveBlock<Tin> {
 public:
  typedef Tout copy_func_t(Tout);

  Block(Tparam param);
  ~Block();

  void set_broadcast(copy_func_t *copy_func);
  void add_downstream(RecieveBlock<Tout> *downstream);

  virtual void work(Tin in, Tparam param);
  void do_work();

  size_t remaining_work();
  void recieve(Tin in);
  void send_downstream(Tout out);

  void run();

 private:
  Tparam param;
  copy_func_t *copy_func;
  std::mutex downstream_lock;
  std::queue<RecieveBlock<Tout> *> downstream;
  Buffer<Tin> work_buffer;
};
}  // namespace cain::flow

#endif  // HDRS_BLOCK_H_