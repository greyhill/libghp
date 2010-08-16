#ifndef _GHP_UTIL_SQLITE_HPP_
#define _GHP_UTIL_SQLITE_HPP_

#include <boost/shared_ptr.hpp>

#include <string>

namespace sqlite {

template<int N=0>
class connection_base {
public:
  connection_base(const std::string &path);
  ~connection_base();

  boost::shared_ptr<statement_ctor> operator()(const std::string &s);

private:
  sqlite3 *db_handle_;
};
typedef connection_base<0> connection;

template<int N=0>
class statement {
public:
  statement(const boost::shared_ptr<statement_ctor> ctor);
  ~statement();

  template<typename T> statement& bind(int n, const T &value);

  boost::shared_ptr<result_ctor> operator()();
  template<typename T1> 
  boost::shared_ptr<result_ctor> operator()(const T1 &t1) {
  }
  template<typename T1, typename T2>
  boost::shared_ptr<result_ctor> operator()(const T1 &t1, const T2 &t2) {
  }

private:
  bool auto_reset_;
};

}

#endif

