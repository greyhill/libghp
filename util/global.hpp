#ifndef _GHP_UTIL_GLOBAL_HPP_
#define _GHP_UTIL_GLOBAL_HPP_

#include <boost/scoped_ptr.hpp>

namespace ghp {

/**
  \brief template class for global objects
  Use this class to create singletons, and reference them when
  you want them. e.g.
  global<globalobject>::get().doSomething()
  \tparam T - type to be wrapped
 */
template<typename T>
class global {
public:
  /**
    get the singleton reference
   */
  static T& get() {
    if(global_ref_ == NULL) {
      global_ref_.reset(new T());
    }
    return *global_ref_;
  }

private:
  static boost::scoped_ptr<T> global_ref_;
};
template<typename T>
static boost::scoped_ptr<T> global<T>::global_ref_;

}

#endif

