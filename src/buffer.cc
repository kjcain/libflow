/**
 * @file buffer.cc
 * @author Kyler Cain (kyler@kylercain.com)
 * @brief
 * @version 0.1
 * @date 2021-03-26
 *
 * @copyright (c) 2021, Kyler Cain
 *
 */

#include "buffer.h"

namespace cain::flow {

template <class T>
Buffer<T>::Buffer() {}

template <class T>
Buffer<T>::~Buffer() {}

template <class T>
void Buffer<T>::push(T in) {
  this->write_lock.lock();
  this->work_queue.push(in);
  this->write_lock.unlock();
}

template <class T>
T Buffer<T>::pull() {
  this->read_lock.lock();
  T next = this->work_queue.front();
  if (nullptr != next) {
    this->work_queue.pop();
  }
  this->read_lock.unlock();
  return next;
}

template <class T>
size_t Buffer<T>::count() {
    return this->work_queue.size();
}

}  // namespace cain::flow
