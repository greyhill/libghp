#ifndef _GHP_INPUT_CORE_HPP_
#define _GHP_INPUT_CORE_HPP_

namespace ghp {

template<typename SOURCE>
class input_core : public SOURCE::input_impl {
public:
  input_core();
  ~input_core();

private:
};

}

#endif

