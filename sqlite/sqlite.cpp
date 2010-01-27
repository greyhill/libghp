#include "sqlite.hpp"
#include <cassert>
#include <stdexcept>
#include <boost/thread/locks.hpp>

namespace ghp {
namespace sqlite {

//
// query_builder implementation
//
query_builder::query_builder(boost::shared_ptr<query_builder_impl> impl)
    : impl_(impl) {
}

query_builder::~query_builder() {
}

query_builder& query_builder::select(const std::string &s) {
  impl_->select_ = s;
  return *this;
}

query_builder& query_builder::from(const std::string &s) {
  impl_->from_ = s;
  return *this;
}

query_builder& query_builder::where(const std::string &s) {
  impl_->where_ = s;
  return *this;
}

//
// connection implementation
//
connection::connection(const std::string &path)
    : db_(NULL),
    num_statements_(0) {
  init_from_path(path);
}

connection::~connection() {
  if(db_ != NULL) {
    // keeping statements open is a programming
    // problem, not a user one.
    assert(num_statements_ == 0);
    sqlite3_close(db_);
  }
}

void connection::init_from_path(const std::string &path) {
  if(sqlite3_open(path.c_str(), &db_) != SQLITE_OK) {
    if(db_ != NULL) {
      sqlite3_close(db_);
    }
    throw std::runtime_error("error opening sqlite3 database");
  }
}

query_builder connection::select(const std::string &s) {
  boost::shared_ptr<query_builder::query_builder_impl> p(
      new query_builder::query_builder_impl(*this));
  p->select_ = s;
  return query_builder(p);
}

query_builder connection::from(const std::string &s) {
  boost::shared_ptr<query_builder::query_builder_impl> p(
      new query_builder::query_builder_impl(*this));
  p->from_ = s;
  return query_builder(p);
}

query_builder connection::where(const std::string &s) {
  boost::shared_ptr<query_builder::query_builder_impl> p(
      new query_builder::query_builder_impl(*this));
  p->where_ = s;
  return query_builder(p);
}

}}

// eof //
