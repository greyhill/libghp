#ifndef _GHP_UTIL_SQLITE_HPP_
#define _GHP_UTIL_SQLITE_HPP_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

#include <sqlite3.h>

#include <cassert>
#include <stdexcept>
#include <string>

#include <stdint.h>

namespace sqlite {

template<int UNUSED> class statement_;
typedef statement_<0> statement;
typedef boost::shared_ptr<statement> statement_ptr;

template<int UNUSED=0>
class connection_ : boost::noncopyable {
public:
  connection_(const std::string &path)
      : db_handle_(NULL) {
    if(sqlite3_open(path.c_str(), &db_handle_) != SQLITE_OK) {
      throw std::runtime_error(sqlite3_errmsg(db_handle_));
    }
    last_id_stmt_ = (*this)("SELECT last_insert_rowid()");
  }
  ~connection_() {
    if(db_handle_ != NULL) {
      if(sqlite3_close(db_handle_) != SQLITE_OK) {
        throw std::runtime_error(sqlite3_errmsg(db_handle_));
      }
    }
  }

  statement_ptr operator()(const std::string &s);
  int last_insert_rowid();

private:
  sqlite3 *db_handle_;
  statement_ptr last_id_stmt_;

  friend class statement_<0>;
};
typedef connection_<0> connection;

template<typename T> struct bind_functor { };
template<> struct bind_functor<double> {
  inline bool operator()(sqlite3_stmt *stmt, int argnum, double d) {
    return sqlite3_bind_double(stmt, argnum, d) == SQLITE_OK;
  }
};
template<> struct bind_functor<int> {
  inline bool operator()(sqlite3_stmt *stmt, int argnum, int i) {
    return sqlite3_bind_int(stmt, argnum, i) == SQLITE_OK;
  }
};
template<> struct bind_functor<int64_t> {
  inline bool operator()(sqlite3_stmt *stmt, int argnum, int64_t i) {
    return sqlite3_bind_int64(stmt, argnum, i) == SQLITE_OK;
  }
};
template<> struct bind_functor<std::string> {
  inline bool operator()(sqlite3_stmt *stmt, int argnum, 
      const std::string &s) {
    return sqlite3_bind_text(stmt, argnum, s.c_str(), sizeof(char)*s.size(),
        SQLITE_TRANSIENT) == SQLITE_OK;
  }
};
template<> struct bind_functor<char*> {
  inline bool operator()(sqlite3_stmt *stmt, int argnum, const char *c) {
    return sqlite3_bind_text(stmt, argnum, c, -1, SQLITE_TRANSIENT) 
        == SQLITE_OK;
  }
};

template<typename T> struct get_functor { };
template<> struct get_functor<double> {
  inline double operator()(sqlite3_stmt *stmt, int colid) {
    return sqlite3_column_double(stmt, colid);
  }
};
template<> struct get_functor<int> {
  inline int operator()(sqlite3_stmt *stmt, int colid) {
    return sqlite3_column_int(stmt, colid);
  }
};
template<> struct get_functor<int64_t> {
  inline int64_t operator()(sqlite3_stmt *stmt, int colid) {
    return sqlite3_column_int64(stmt, colid);
  }
};
template<> struct get_functor<std::string> {
  inline std::string operator()(sqlite3_stmt *stmt, int colid) {
    return std::string(
      reinterpret_cast<const char*>(sqlite3_column_text(stmt, colid)));
  }
};
template<> struct get_functor<uint8_t*> {
  inline uint8_t* operator()(sqlite3_stmt *stmt, int colid) {
    int bytes = sqlite3_column_bytes(stmt, colid);
    uint8_t *buf = new uint8_t[bytes];
    const uint8_t *src = reinterpret_cast<const uint8_t*>(
        sqlite3_column_blob(stmt, colid));
    for(int i=0; i<bytes; ++i) buf[i] = src[i];
    return buf;
  }
};

template<int UNUSED=0>
class statement_ : boost::noncopyable {
public:
  ~statement_() {
    if(stmt_ != NULL) {
      if(sqlite3_finalize(stmt_) != SQLITE_OK) {
        throw std::runtime_error(sqlite3_errmsg(connection_.db_handle_));
      }
    }
  }

  void reset() {
    if(sqlite3_reset(stmt_) != SQLITE_OK) {
      throw std::runtime_error(sqlite3_errmsg(connection_.db_handle_));
    }
  }

  template<typename T> statement_* bind(int argnum, const T &t) {
    bind_functor<T> bf;
    if(!bf(stmt_, argnum, t)) {
      throw std::runtime_error(sqlite3_errmsg(connection_.db_handle_));
    }
    return this;
  }
  statement_* bind(int argnum, const uint8_t *t, int size) {
    if(sqlite3_bind_blob(stmt_, argnum, 
          reinterpret_cast<const void*>(t),
          size,
          SQLITE_TRANSIENT) != SQLITE_OK) {
      throw std::runtime_error(sqlite3_errmsg(connection_.db_handle_));
    }
  }

  void execute() {
    int rc = sqlite3_step(stmt_);
    if(rc == SQLITE_ERROR) {
      throw std::runtime_error(sqlite3_errmsg(connection_.db_handle_));
    }
    if(rc == SQLITE_DONE) {
      reset();
    }
  }

  bool step() {
    int rc = sqlite3_step(stmt_);
    if(rc == SQLITE_ERROR) {
      throw std::runtime_error(sqlite3_errmsg(connection_.db_handle_));
    }
    return rc != SQLITE_DONE;
  }

  template<typename T> T get(int column) {
    get_functor<T> gf;
    return gf(stmt_, column);
  }

private:
  statement_(connection &conn, const std::string &text) 
      : connection_(conn),
      stmt_(NULL) {
    if(sqlite3_prepare_v2(connection_.db_handle_,
          text.c_str(),
          sizeof(char) * text.size(),
          &stmt_,
          NULL) != SQLITE_OK) {
      throw std::runtime_error(sqlite3_errmsg(conn.db_handle_));
    }
  }

  connection &connection_;
  sqlite3_stmt *stmt_;

  friend class sqlite::connection_<0>; 
};

// placed here
template<int UNUSED>
statement_ptr connection_<UNUSED>::operator()(const std::string &s) {
  statement_ptr st(new statement_<0>(*this, s));
  return st;
}
template<int UNUSED>
int connection_<UNUSED>::last_insert_rowid() {
  assert(last_id_stmt_->step());
  int id = last_id_stmt_->get<int>(0);
  last_id_stmt_->reset();
  return id;
}

}

#endif

