#ifndef _GHP_UTIL_GENERIC_PTR_DEREF_
#define _GHP_UTIL_GENERIC_PTR_DEREF_

#include <boost/shared_array.hpp>
#include <boost/shared_ptr.hpp>

namespace ghp {

template<typename T, typename PTR> struct generic_ptr_deref { };
template<typename T> struct generic_ptr_deref<T, T*> {
  inline T* operator()(T* t) { return t; }
};
template<typename T> struct generic_ptr_deref<T, boost::shared_ptr<T> > {
  inline T* operator()(boost::shared_ptr<T> t) { return t.get(); }
};
template<typename T> struct generic_ptr_deref<T, boost::shared_array<T> > {
  inline T* operator()(boost::shared_array<T> t) { return t.get(); }
};

}

#endif

