#ifndef CAFFE_SYNCEDMEM_HPP_
#define CAFFE_SYNCEDMEM_HPP_

#include <cstdlib>
#include "caffe/common.hpp"


namespace caffe {
  
inline void CaffeMallocHost(void** ptr, size_t size, bool* use_cuda) {
  *ptr = malloc(size);
  *use_cuda = false;
  CHECK(*ptr) << "host allocation of size " << size << " failed";
}

inline void CaffeFreeHost(void* ptr, bool use_cuda) {
  free(ptr);
}


/* @brief Manages memory allocation and synchronization between the host (CPU).
 * TODO(dox): more thorough description. */
class SyncedMemory {
 public:
  SyncedMemory();
  explicit SyncedMemory(size_t size);
  ~SyncedMemory();

  const void* cpu_data();
  void set_cpu_data(void* data);
  void* mutable_cpu_data();
  enum SyncedHead { UNINITIALIZED, HEAD_AT_CPU, SYNCED };
  SyncedHead head() const { return head_; }
  size_t size() const { return size_; }

 private:
  void to_cpu();
  void* cpu_ptr_;
  size_t size_;
  SyncedHead head_;
  bool own_cpu_data_;
  bool cpu_malloc_use_cuda_;
  int device_;

  DISABLE_COPY_AND_ASSIGN(SyncedMemory);
};  // class SyncedMemory

}  // namespace caffe
#endif  // CAFFE_SYNCEDMEM_HPP_
