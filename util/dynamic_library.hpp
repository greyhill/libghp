#ifndef _GHP_UTIL_DYNAMIC_LIBRARY_HPP_
#define _GHP_UTIL_DYNAMIC_LIBRARY_HPP_

#include <boost/noncopyable.hpp>

#include <stdexcept>
#include <string>

namespace ghp {

#ifdef _WIN32
  #define _GHP_UTIL_DYNAMIC_LIBRARY_OK
  #include <windows.h>
  typedef HMODULE library_type;
  typedef FARPROC symbol_type;

  inline library_type load_library_(const std::string &s) {
    return LoadLibrary(s.c_str());
  }
  inline symbol_type get_symbol_(library_type lib, const std::string &s) {
    return GetProcAddress(lib, s.c_str());
  }
  inline void close_library_(library_type lib) {
    FreeLibrary(lib);
  }
#elif defined(__unix__)
  #define _GHP_UTIL_DYNAMIC_LIBRARY_OK_
  #include <dlfcn.h>
  typedef void* library_type;
  typedef void* symbol_type;

  inline library_type load_library_(const std::string &s) {
    return dlopen(s.c_str(), RTLD_LAZY);
  }
  inline symbol_type get_symbol_(library_type lib, const std::string &s) {
    return dlsym(lib, s.c_str());
  }
  inline void close_library_(library_type lib) {
    dlclose(lib);
  }
#endif

#ifdef _GHP_UTIL_DYNAMIC_LIBRARY_OK_
/** \brief dynamically loaded library
  dynamic_library provides a multiplatform abstraction of a dynamically
  loaded library.  dynamic_library's are noncopyable, and are available
  only on Windows and UNIX-like operating systems right now.
 */
template<int N=0>
class dynamic_library_ : public boost::noncopyable {
public:
  /** \brief create a new dynamic_library
    \param path - path to, or name of, the library to be loaded.  The
      dynamic loaded will load the library if possible in an OS-specific
      fashion.
    \throws std::runtime_error on errors
    */
  dynamic_library_(const std::string &path) 
      : library_(load_library_(path)) {
    if(library_ == NULL) {
      throw std::runtime_error("couldn't load library");
    }
  }
  ~dynamic_library_() {
    close_library_(library_);
  }

  /** \brief lookup a symbol in the loaded library
    Symbols in the dynamically laoded library are looked up with this
    function, cast to the desired pointer-type and returned.  NULL
    is returned on errors.  NOTE: when this dynamic_library object is
    destroyed, all loaded symbols will become inaccessible.
      \tparam T - POINTER TYPE of data being looked up
      \returns NULL on errors
   */
  template<typename T>
  T lookup(const std::string &symbol_name) {
    symbol_type symb = get_symbol_(library_, symbol_name);
    if(symb == NULL) return NULL;
    ptr_convert_<T> convert;
    convert.addr_ = symb;
    return convert.obj_;
  }
 
private:
  template<typename T>
  union ptr_convert_ {
    T obj_;
    symbol_type addr_;
  };

  library_type library_;
};

typedef dynamic_library_<0> dynamic_library;

#endif

}

#endif

