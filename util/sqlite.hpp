#ifndef _GHP_UTIL_SQLITE_HPP_
#define _GHP_UTIL_SQLITE_HPP_

#include <boost/noncopyable.hpp>

#include <sqlite3.h>

#include <cassert>
#include <string>

namespace sqlite {

template<int N=0>
class statement_ctor_base {
public:
  inline ~statement_ctor_base() {  }
  inline class connection_base<N>* connection() const {
    return connection_;
  }
  inline const std::string& statement() const {
    return statement_;
  }
private:
  inline statement_ctor_base(class connection_base<N> *conn,
      const std::string &statement)
      : connection_(conn),
      statement_(statement) { }
  class connection_base<N> *connection_;
  std::string statement_;
friend class connection_base<0>;
};
typedef statement_ctor_base<0> statement_ctor;

template<int N=0>
class connection_base : boost::noncopyable {
public:
  connection_base(const std::string &path);
  ~connection_base();

  statement_ctor* operator()(const std::string &s);

private:
  sqlite3 *db_handle_;
};
typedef connection_base<0> connection;

template<int N=0>
class statement : boost::noncopyable {
public:
  statement(const statement_ctor *ctor);
  ~statement();

  template<typename T> statement& bind(int n, const T &value) {
    assert(false);
  }
  template<> statement& bind<int>(int n, const int &value) {
  }

  boost::shared_ptr<result_ctor> operator()();
  template<typename T1> 
  boost::shared_ptr<result_ctor> operator()(const T1 &t1) {
  }
  template<typename T1, typename T2>
  boost::shared_ptr<result_ctor> operator()(const T1 &t1, const T2 &t2) {
  }

private:
  bool auto_reset_;
  connection &connection_;
};

}

#endif

