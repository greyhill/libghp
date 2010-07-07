#ifndef _GHP_JANUS_JANUS_DATA_STACK_HPP_
#define _GHP_JANUS_JANUS_DATA_STACK_HPP_

#include "../util.hpp"

#include <list>
#include <vector>

#include <stdint.h>

namespace ghp {

/** \brief data stack for janus stack machine.  this type is 
  used internally in the Janus stack machine.  it is a (potentially)
  decently performant stack which can contain elements of 1, 2, 4 or 8
  bytes (sufficient for Janus's operation).
    \tparam STACK - some type with back(), push_back() and pop_back(), 
      operating on uint8_t's.  std::list and std::vector are appropriate
      candidates.  std::vector may be more performant but waste some memory.
 */
template<typename STACK=std::list<uint8_t> > class janus_data_stack {
public:
  typedef STACK stack_t;

  inline janus_data_stack() {
  }
  inline janus_data_stack(const janus_exec_stack &j)
      : stack_(j.stack_) { }
  inline ~janus_data_stack() { }

  inline stack_t& raw_stack() { return stack_; }

  inline template<typename T> void push(const T &t) {
    janus_data_stack_pusher<stack_t, T> pusher;
    pusher(raw_stack(), t);
  }
  inline template<typename T> T pop() {
    janus_data_stack_popper<stack_t, T> popper;
    return popper(raw_stack());
  }

private:
  stack_t stack_;
};

/*
  basic push / pop interface for uint8_t containers
 */
template<typename STACK, typename T> struct janus_data_stack_pusher {
  inline void operator()(STACK &stack, const T &t) {
    push_by_size_<sizeof(T)>(stack, t);
  }
private:
  template<int SIZE>
  inline void push_by_size_(STACK &stack, const T &t) {
    typedef int_by_size<SIZE>::unsigned_type int_type;
    int_type buffer = *reinterpret_cast<const int_type*>(&t);
    buffer = hton(buffer);
    for(int i=SIZE-1; i>=0; --i) {
      uint8_t byte = static_cast<uint8_t>((buffer & (0xFF << i)) >> i);
      stack.push_back(byte);
    }
  }
};

template<typename STACK, typename T> struct janus_data_stack_popper {
  inline T operator()(STACK &stack) {
    return pop_by_size_<sizeof(T)>(stack);
  }
private:
  template<int SIZE>
  inline T pop_by_size_(STACK &stack) {
    typedef int_by_size<SIZE>::unsigned_type int_type;
    int_type buffer = 0;
    for(int i=0; i<SIZE; ++i) {
      buffer |= (stack.back() << i);
      stack.pop_back();
    }
    return ntoh(*reinterpret_cast<T*>(&buffer));
  }
};

}

#endif

