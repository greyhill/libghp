#ifndef _GHP_INPUT_MOUSE_HPP_
#define _GHP_INPUT_MOUSE_HPP_

#include <boost/util.hpp>
#include <boost/signals.hpp>

namespace ghp {
namespace input {

class mouse : boost::noncopyable {
public:
  typedef void (*callback)(const mouse_event&);

  static mouse& create();
  static mouse& get();
  static bool is_created();
  static void destroy();

  ~mouse();

  mouse& set_grab(bool b);
  bool set_grab() const;
  mouse& bind(mouse_event &evt, const callback &c) const;

private:
  mouse();
};

}}

#endif

