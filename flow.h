/**
 * @file flow.h
 * @author Kyler Cain (kyler.cain@yahoo.com)
 * @version 0.1
 * @date 2021-03-12
 *
 * @brief this is a header-only implementation of dataflow programming
 * implemented under c++17.
 *
 * @note if a block is given multiple downstream links only one will recieve
 * each message (see todo about broadcast_link)
 *
 * @todo: for now all messages passed internally are the same type
 * would be nice to be able to pass specific types in/out of blocks as
 * this will likely lead towards the usage of godlike unions
 *
 * @todo: implement broadcast_link to allow sending the same message to
 * all downstream links
 *
 * @todo: implement batch_link to allow grouping messages together
 *
 * @todo: thread pool could use optimization
 *
 * @todo: buffers could use limits and optimizations, preferably at the flow
 * level, however could potentially be done at the block out buffer level
 */

#include <map>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

#ifndef FLOW_H
#define FLOW_H

namespace cain::flow {

typedef size_t BlockID;
typedef size_t LinkID;

template <class T>
class Buffer {
 protected:
  bool try_pop(T *dest);
  Buffer();
  ~Buffer();

 public:
  void push(T new_item);

 private:
  std::queue<T> *backlog;
  std::mutex read_lock;
  std::mutex write_lock;
};

template <typename T>
class WorkerFunc {
  public:
  typedef void SourceFunc(Buffer<T> *output, void *params);
  typedef void TransFunc(T input, Buffer<T> *output, void *params);
  typedef void SinkFunc(T input, void *params);
};

template <class T>
class SourceBlock {
 public:
  SourceBlock(typename WorkerFunc<T>::SourceFunc *func, void *params);
  ~SourceBlock();

 protected:
  Buffer<T> *out_buffer;
  void work();

 private:
  typename WorkerFunc<T>::SourceFunc *func;
  void *params;
};

template <class T>
class TransBlock {
 public:
  TransBlock(typename WorkerFunc<T>::TransFunc *func, void *params);
  ~TransBlock();

 protected:
  Buffer<T> *out_buffer;
  void work(T unit_of_work);

 private:
  typename WorkerFunc<T>::TransFunc *func;
  void *params;
};

template <class T>
class SinkBlock {
 public:
  SinkBlock(typename WorkerFunc<T>::SinkFunc *sink, void *params);
  ~SinkBlock();

 protected:
  void work(T unit_of_work);

 private:
  typename WorkerFunc<T>::SinkFunc *func;
  void *params;
};

template <class T>
class Flow {
 public:
  BlockID add_source(typename WorkerFunc<T>::SourceFunc *func, void *params);
  BlockID add_trans(typename WorkerFunc<T>::TransFunc *func, void *params);
  BlockID add_sink(typename WorkerFunc<T>::SinkFunc *sink, void *params);

  LinkID link(BlockID from, BlockID to);

  bool validate();
  bool run();

  Flow(size_t thread_count);
  ~Flow();

 private:
  std::map<BlockID, SourceBlock<T>> source_blocks;
  std::map<BlockID, TransBlock<T>> trans_blocks;
  std::map<BlockID, SinkBlock<T>> sink_blocks;
  std::vector<std::tuple<BlockID, BlockID>> links;
};

}  // namespace cain::flow

#endif  // FLOW_H
