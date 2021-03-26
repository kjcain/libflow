/**
 * @file block.cc
 * @author Kyler Cain (kyler@kylercain.com)
 * @brief
 * @version 0.1
 * @date 2021-03-26
 *
 * @copyright (c) 2021, Kyler Cain
 *
 */

#include "block.h"

namespace cain::flow {
template <class Tin, class Tout, class Tparam>
Block<Tin, Tout, Tparam>::Block(Tparam param) {
  this->param = param;
  this->copy_func = nullptr;
}

template <class Tin, class Tout, class Tparam>
Block<Tin, Tout, Tparam>::~Block() {}

template <class Tin, class Tout, class Tparam>
void Block<Tin, Tout, Tparam>::set_broadcast(copy_func_t *copy_func) {
  this->copy_func = copy_func;
}

template <class Tin, class Tout, class Tparam>
void Block<Tin, Tout, Tparam>::add_downstream(RecieveBlock<Tout> *downstream) {
  if (typeid(Tout) != typeid(nullptr)) {
    this->downstream_lock.lock();
    this->downstream->push(downstream);
    this->downstream_lock.unlock();
  } else {
    throw std::runtime_error("cannot add downstream to a block with null Tout");
  }
}

template <class Tin, class Tout, class Tparam>
void Block<Tin, Tout, Tparam>::work(Tin in, Tparam) {
  if (typeid(Tin) == typeid(Tout)) {
    send_downstream(in);
  } else {
    throw std::runtime_error("cannot pass through a block with Tin != Tout");
  }
}

template <class Tin, class Tout, class Tparam>
void Block<Tin, Tout, Tparam>::do_work() {
  Tin next = this->work_buffer.pull();
  if (next != nullptr) {
    this->work(next, this->param);
  }
}

template <class Tin, class Tout, class Tparam>
size_t Block<Tin, Tout, Tparam>::remaining_work() {
  return this->work_buffer->count();
}

template <class Tin, class Tout, class Tparam>
void Block<Tin, Tout, Tparam>::recieve(Tin in) {
  this->work_buffer.push(in);
}

template <class Tin, class Tout, class Tparam>
void Block<Tin, Tout, Tparam>::send_downstream(Tout out) {
  if (!this->downstream->empty()) {
    this->downstream_lock.lock();

    if (this->downstream.size() == 1) {
      RecieveBlock<Tout> *reciever = this->downstream.front();
      reciever->recieve(out);
    } else if (copy_func == nullptr) {
      RecieveBlock<Tout> *reciever = this->downstream.front();
      this->downstream.pop();

      reciever->recieve(out);

      this->downstream.push(reciever);
    } else {
      for (size_t index = 0; index < this->downstream.size(); index++) {
        RecieveBlock<Tout> *reciever = this->downstream.front();
        this->downstream.pop();

        reciever->recieve(out);

        this->downstream.push(reciever);
      }
    }
    this->downstream_lock.unlock();
  } else {
    throw std::runtime_error(
        "cannot send downstream without any downstream blocks");
  }
}

template <class Tin, class Tout, class Tparam>
void Block<Tin, Tout, Tparam>::run() {
  throw std::runtime_error("run is not yet implemented");
}

//   std::vector<RecieveBlock<Tout> *> downstream;
//   Buffer<Tin> *work_buffer;
}  // namespace cain::flow
