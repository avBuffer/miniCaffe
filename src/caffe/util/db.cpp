#include "caffe/util/db.hpp"
#include "caffe/util/db_lmdb.hpp"
#include <string>


namespace caffe { 
namespace db {

DB* GetDB(DataParameter::DB backend) {
  switch (backend) {
    case DataParameter_DB_LMDB:
      return new LMDB();
    default:
      LOG(FATAL) << "Unknown database backend";
      return NULL;
  }
}

DB* GetDB(const string& backend) {
  if (backend == "lmdb") {
    return new LMDB();
  }
  LOG(FATAL) << "Unknown database backend";
  return NULL;
}

}  // namespace db
}  // namespace caffe
