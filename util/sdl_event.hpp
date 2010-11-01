#ifndef _GHP_UTIL_SDL_EVENT_HPP_
#define _GHP_UTIL_SDL_EVENT_HPP_

#include <boost/noncopyable.hpp>
#include <boost/signals2.hpp>

#include <SDL.h>

namespace sdl {

template<int UNUSED=0>
class keyboard_event_ {
};
typedef keyboard_event_<0> keyboard_event;

template<int UNUSED=0>
class mouse_button_event_ {
};
typedef mouse_button_event_<0> mouse_button_event;

template<int UNUSED=0>
class mouse_motion_event_ {
};
typedef mouse_motion_event_<0> mouse_motion_event;

template<int UNUSED=0>
class quit_event_ {
};
typedef quit_event_<0> quit_event;

namespace {
  struct combine_or {
    template<typename InputIterator>
    bool operator()(InputIterator begin, InputIterator end) {
      for(; begin != end; ++begin) {
        if(*begin) return true;
      }
      return false;
    }
  };
}

template<int UNUSED=0>
class input_ : boost::noncopyable {
public:
  typedef bool (*keyboard_callback_type)(const keyboard_event&);
  typedef boost::signals2::signal<keyboard_callback_type, combine_or> 
      keyboard_slot_type;
  typedef bool (*mouse_motion_callback_type)(const mouse_motion_event&);
  typedef boost::signals2::signal<mouse_motion_callback_type, combine_or>
      mouse_motion_slot_type;
  typedef bool (*mouse_button_callback_type)(const mouse_button_event&);
  typedef boost::signals2::signal<mouse_button_callback_type, combine_or>
      mouse_button_slot_type;
  typedef bool (*quit_callback_type)(const quit_event&);
  typedef boost::signals2::signal<quit_callback_type, combine_or>
      quit_slot_type;

  input() {
  }
  ~input() {
  }

  keyboard_slot_type& keyboard_slot() {
    return keyboard_slot_;
  }
  mouse_motion_slot_type& mouse_motion_slot() {
    return mouse_motion_slot_;
  }
  mouse_button_slot_type& mouse_button_slot() {
    return mouse_button_slot_;
  }
  quit_slot_type& quit_slot() {
    return quit_slot_;
  }

  void handle_events() {
  }

private:
  keyboard_slot_type keyboard_slot_;
  mouse_motion_slot_type mouse_motion_slot_;
  mouse_button_slot_type mouse_button_slot_;
  quit_slot_type quit_slot_;
};
typedef input_<0> input;

}

#endif

