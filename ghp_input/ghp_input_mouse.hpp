#ifndef _GHP_INPUT_MOUSE_HPP_
#define _GHP_INPUT_MOUSE_HPP_

#include <boost/utility.hpp>
#include <boost/signals.hpp>
#include "ghp_input_filter.hpp"
#include "ghp_math.hpp"

namespace ghp {
namespace input {

enum mouse_button : uint8_t {
  LEFT = 0,
  RIGHT = 1,
  MIDDLE = 2
};
#define GHP_INPUT_MOUSE_BUTTONS 3
 
class mouse : boost::noncopyable {
public:
  ~mouse();

  bool is_button_pressed_now(mouse_button b) const;
  bool is_button_released_now(mouse_button b) const;
  bool is_button_down_now(mouse_button b) const;
  bool is_moved_now() const;

  const filter& button_pressed(mouse_button b) const;
  const filter& button_released(mouse_button b) const;
  const filter& button_down(mouse_button b) const;
  const filter& button_up(mouse_button b) const;
  const filter& moved() const;

private:
  mouse();

  mutable filter *pressed_[GHP_INPUT_MOUSE_BUTTONS];
  mutable filter *released_[GHP_INPUT_MOUSE_BUTTONS];
  mutable filter *button_down_[GHP_INPUT_MOUSE_BUTTONS];
  mutable filter *button_up_[GHP_INPUT_MOUSE_BUTTONS];
  mutable filter *moved_;
};

}}

#endif

