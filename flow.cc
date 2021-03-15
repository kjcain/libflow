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

#include "flow.h"

namespace cain::flow {

/******************************************************************************/
/* BUFFER DEFINITIONS                                                         */
/******************************************************************************/

/* #region Buffer Definitions */

template <class T>
void Buffer<T>::push(T new_item) {
  this->write_lock.lock();
  this->backlog->push(new_item);
  this->read_lock.lock();
}

template <class T>
bool Buffer<T>::try_pop(T *dest) {
  this->read_lock.lock();
  bool ret = !this->backlog->empty();
  if (ret) {
    *dest = this->backlog->front();
    this->backlog->pop();
  }
  this->read_lock.unlock();
  return ret;
}

template <class T>
Buffer<T>::Buffer() {
  this->backlog = new std::queue<T>();
}

template <class T>
Buffer<T>::~Buffer() {
  this->read_lock.lock();
  this->write_lock.lock();
  delete this->backlog;
}

/* #endregion */

/******************************************************************************/
/* SOURCE BLOCK DEFINITIONS                                                   */
/******************************************************************************/

/* #region Source Block Definitions */

template <class T>
SourceBlock<T>::SourceBlock(WorkerFunc<T>::SourceFunc *func, void *params) {
  this->func = func;
  this->params = params;
  this->out_buffer = new Buffer<T>();
}

template <class T>
SourceBlock<T>::~SourceBlock() {
  delete this->out_buffer;
}

template <class T>
SourceBlock<T>::work() {
  this->func(this->out_buffer, this->params);
}

/* #endregion */

/******************************************************************************/
/* TRANS BLOCK DEFINITIONS                                                    */
/******************************************************************************/

/* #region Trans Block Definitions */

template <class T>
TransBlock<T>::TransBlock(WorkerFunc<T>::TransFunc *func, void *params) {
  this->func = func;
  this->params = params;
  this->out_buffer = new Buffer<T>();
}

template <class T>
TransBlock<T>::~TransBlock() {
  delete this->out_buffer;
}

template <class T>
void TransBlock<T>::work(T unit_of_work) {
  this->func(unit_of_work, this->out_buffer, this->params);
}

/* #endregion */

/******************************************************************************/
/* SINK BLOCK DEFINITIONS                                                     */
/******************************************************************************/

/* #region Sink Block Definitions */

template <class T>
SinkBlock::SinkBlock(WorkerFunc<T>::SinkFunc *sink, void *params) {
  this->func = func;
  this->params = params;
}

template <class T>
SinkBlock::~SinkBlock() {}

template <class T>
SinkBlock<T>::work(T unit_of_work) {
  this->func(unit_of_work, this->params);
}

/* #endregion */

/******************************************************************************/
/* FLOW DEFINITIONS                                                           */
/******************************************************************************/

/* #region Flow Definitions */

template <class T>
BlockID Flow<T>::add_source(SourceFunc *func, void *params) {}

template <class T>
BlockID Flow<T>::add_trans(TransFunc *func, void *params) {}

template <class T>
BlockID Flow<T>::add_sink(SinkFunc *sink, void *params) {}

template <class T>
LinkID Flow<T>::link(BlockID from, BlockID to) {}

template <class T>
bool Flow<T>::validate() {}

template <class T>
bool Flow<T>::run() {}

template <class T>
Flow<T>::Flow(size_t thread_count) {}

template <class T>
Flow<T>::~Flow() {}

/* #endregion */


}  // namespace cain::flow
