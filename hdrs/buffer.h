/**
 * @file buffer.h
 * @author kyler@kylercain.com (kyler@kylercain.com)
 * @brief
 * @version 0.1
 * @date 2021-03-26
 *
 * @copyright Copyright (c) 2021, Kyler Cain
 */

#ifndef HDRS_BUFFER_H_
#define HDRS_BUFFER_H_

#include <mutex>
#include <queue>

namespace cain::flow {
template <class T>
class Buffer {
 public:
  void push(T in);
  T pull();
  size_t count();
  Buffer();
  ~Buffer();

 private:
  std::queue<T> work_queue;
  std::mutex write_lock;
  std::mutex read_lock;
};
}  // namespace cain::flow

#endif  // HDRS_BUFFER_H_
