#ifndef _LIBGHP_SQLITE_HPP_
#define _LIBGHP_SQLITE_HPP_

#include <string>
#include <sstream>
#include <vector>

#include <sqlite3.h>
#include <stdint.h>

#include <boost/any.hpp>
#include <boost/bimap.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/static_assert.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/noncopyable.hpp>

namespace ghp {
namespace sqlite {

enum type {
  BLOB,
  DOUBLE,
  INT,
  INT64,
  TEXT,
  //TEXT16,
  //VALUE,
  ZEROBLOB,
  NONE
};
struct null_t { }; static null_t NULL_T;
template<typename T> type get_type() { assert(false); }
template<> type get_type<void*>() { return BLOB; }
template<> type get_type<uint8_t*>() { return BLOB; }
template<> type get_type<double>() { return DOUBLE; }
template<> type get_type<int32_t>() { return INT; }
template<> type get_type<int64_t>() { return INT64; }
template<> type get_type<char*>() { return TEXT; }
template<> type get_type<null_t>() { return NONE; }


class row {
public:
  ~row();

  template<typename T> const T& operator()(uint32_t idx) const;
  template<typename T> const T& operator()(const std::string &s);

private:
  // a row _never_ owns the columns registry; it just needs one
  // in order to operate.
  boost::bimap<uint32_t, std::string> *columns_;
  std::vector<boost::any> values_;
};

class row_set {
public:
  row_set(class row_iterator &it);

private:
  // the row_set owns the column list; each row refers to it.
  boost::bimap<uint32_t, std::string> columns_;
  std::vector<row> rows_;
};

class row_iterator {
public:
  row_iterator();
  ~row_iterator();

  const row& operator*() const;
  template<typename T> const T& operator()(uint32_t idx) const;
  template<typename T> const T& operator()(const std::string &s);

  row_iterator operator++();
  bool operator==(const row_iterator &it) const;

private:
  row_iterator(boost::shared_ptr<class row_iterator_impl> ptr);

  // only one row_iterator can exist per query at a time.
  // we enforce this with a lock.  attempting to have two
  // row_iterators active for the same query at once will
  // result in an exception.
  class row_iterator_impl : boost::noncopyable {
    class query &query_;
    boost::lock_guard<boost::mutex> query_lock_;
  };
  boost::shared_ptr<row_iterator_impl> impl_;

  friend class query;
};

class query {
public:
  query(const class query_builder &q);
  ~query();

  // perform the query
  row_iterator operator()();

  // the following are methods to perform a parameterized
  // query.  unlike in the C interface to SQLite,
  template<typename T>
  row_iterator operator()(const T &t);
  template<typename T1, typename T2>
  row_iterator operator()(const T1 &t1, const T2 &t2);
  template<typename T1, typename T2, typename T3, typename T4>
  row_iterator operator()(const T1 &t1, const T2 &t2, const T3 &t3);

private:
  sqlite3_stmt *statement_;
};

class query_builder {
public:
  ~query_builder();

  query_builder& select(const std::string &s);
  query_builder& from(const std::string &s);
  query_builder& where(const std::string &s);
  // etc.

private:
  struct query_builder_impl : boost::noncopyable {
    std::string select_;
    std::string from_;
    std::string where_;
    // etc.
    class connection &conn_;

    query_builder_impl(class connection &conn) : conn_(conn) { }
  };

  query_builder(boost::shared_ptr<query_builder_impl> impl);

  boost::shared_ptr<query_builder_impl> impl_;

  friend class query;
  friend class connection;
};

class connection : boost::noncopyable {
public:
  connection(const std::string &path);
  ~connection();

  query_builder select(const std::string &s);
  query_builder from(const std::string &s);
  query_builder where(const std::string &s);
  // etc... keep in line with query_builder

private:
  void init_from_path(const std::string &path);

  sqlite3 *db_;
  uint32_t num_statements_;
  boost::mutex statement_mutex_;
};

}}

#endif /* LIBGHP_SQLITE_HPP_ */
