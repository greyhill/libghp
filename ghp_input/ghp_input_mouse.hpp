#ifndef _GHP_INPUT_MOUSE_HPP_
#define _GHP_INPUT_MOUSE_HPP_

#include <boost/utility.hpp>
#include <boost/signals.hpp>

namespace ghp {
namespace input {

class mouse : boost::noncopyable {
public:
  static mouse& create();
  static mouse& get();
  static bool is_created();
  static void destroy();

  enum button {
    left,
    right,
    middle
  };

  class move_event {
  };
  typedef void (*move_cb)(const move_event&);

  class button_event {
  };
  typedef void (*button_cb)(const button_event&);

  ~mouse();

  mouse& set_grab(bool b);
  bool set_grab() const;

  mouse& bind_move(move_cb &c);
  mouse& bind_press(button b, button_cb &c);
  mouse& bind_release(button b, button_cb &c);

private:
  mouse();
};

}}

#endif

