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
 private:
  Buffer<Tin> work_buffer;

 public:
  size_t remaining_work() { return this->work_buffer->count(); }

  void recieve(Tin in) { this->work_buffer.push(in); }

 protected:
  bool try_get_next_work(Tin *next) { return this->work_buffer.try_pull(next); }
};

template <class Tout>
class SendBlock {
 public:
  typedef Tout copy_func_t(Tout);

 private:
  copy_func_t *copy_func = nullptr;
  std::mutex downstream_lock;
  std::queue<RecieveBlock<Tout> *> downstream;

 public:
  void add_downstream(RecieveBlock<Tout> *downstream) {
    if (typeid(Tout) != typeid(nullptr)) {
      this->downstream_lock.lock();
      this->downstream.push(downstream);
      this->downstream_lock.unlock();
    } else {
      throw std::runtime_error(
          "cannot add downstream to a block with null Tout");
    }
  }

  void send_downstream(Tout out) {
    if (!this->downstream.empty()) {
      this->downstream_lock.lock();

      if (this->downstream.size() == 1) {
        auto *reciever = this->downstream.front();
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

  void set_broadcast(copy_func_t *copy_func) { this->copy_func = copy_func; }
};

template <class Tin, class Tout, class Tparam>
class Block : public RecieveBlock<Tin>, public SendBlock<Tout> {
 private:
  Tparam param;

 public:
  Block(Tparam param) : RecieveBlock<Tin>(), SendBlock<Tout>() {
    this->param = param;
  }

  ~Block() {}

  virtual void work(Tin, Tparam);

  void do_work() {
    Tin next;
    if (this->try_get_next_work(&next)) {
      this->work(next, this->param);
    }
  }

    void run() { throw std::runtime_error("run is not yet implemented"); }
};
}  // namespace cain::flow

#endif  // HDRS_BLOCK_H_