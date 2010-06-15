#ifndef _GHP_UTIL_DELEGATED_ASSIGNMENT_HPP_
#define _GHP_UTIL_DELEGATED_ASSIGNMENT_HPP_

namespace ghp {

/** \brief utility for delegating assignment and copy construction
    outside the body of a class.  The notion works like this:
    
    I'm writing some type A, and I want to allow users to automatically
    convert to A from other types.  If A has the following two 
    methods:

    template<typename T>
    A(const T &t) {
      ghp::delegated_assignment<A, T> ctor(*this, t);
      ctor();
    }

    template<typename T>
    A& operator=(const T &t) {
      ghp::delegated_assignment<A, T> ctor(*this, t);
      ctor();
      return *this;
    }

    Then the user need only implement specializations of 
    ghp::delegated_assignment to add conversions to A nonintrusively */
template<typename A, typename B> struct delegated_assignment { };

}

#endif

