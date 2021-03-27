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
  void push(T in) {
    this->write_lock.lock();
    this->work_queue.push(in);
    this->write_lock.unlock();
  }

  bool try_pull(T *out) {
    this->read_lock.lock();
    if (!this->work_queue.empty()) {
      this->read_lock.unlock();
      return false;
    }

    *out = this->work_queue.front();

    this->work_queue.pop();
    this->read_lock.unlock();
    return true;
  }
  size_t count() { return this->work_queue.size(); }
  Buffer() {}
  ~Buffer() {}

 private:
  std::queue<T> work_queue;
  std::mutex write_lock;
  std::mutex read_lock;
};
}  // namespace cain::flow

#endif  // HDRS_BUFFER_H_
