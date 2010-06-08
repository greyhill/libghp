#ifndef _GHP_UTIL_CONTAINER_TRAITS_HPP_
#define _GHP_UTIL_CONTAINER_TRAITS_HPP_

namespace ghp {

/** \brief traits for determining the type a container holds
  type traits for indentifying the underlying value type 
  in a container.  STL containers have a value_type field,
  and specializations are provided for arrays and const arrays. */
template<typename T> struct container_traits {
  typedef typename T::value_type value_type;
};
template<typename T> struct container_traits<T*> {
  typedef T value_type;
};
template<typename T> struct container_traits<const T*> {
  typedef const T value_type;
};

}

#endif

