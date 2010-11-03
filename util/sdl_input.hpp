#ifndef _GHP_UTIL_SDL_EVENT_HPP_
#define _GHP_UTIL_SDL_EVENT_HPP_

#include "../math.hpp"

#include <boost/signals2/signal.hpp>

#include <SDL.h>

namespace sdl {

template<int UNUSED=0>
class keyboard_event_ {
public:
  keyboard_event_(const SDL_KeyboardEvent &e) 
      : evt_(e) {
  }
  ~keyboard_event_() {
  }

  inline bool down() const {
    return evt_.type == SDL_KEYDOWN;
  }
  inline bool up() const { return !down(); }
  inline bool pressed() const {
    return evt_.type == SDL_PRESSED;
  }
  inline bool released() const { return !pressed(); }

  inline uint16_t unicode() const {
    return evt_.keysym.unicode;
  }
  inline SDLKey keysym() const {
    return evt_.keysym.sym;
  }
  inline SDLMod modifiers() const {
    return evt_.keysym.mod;
  }

private:
  SDL_KeyboardEvent evt_;
};
typedef keyboard_event_<0> keyboard_event;

template<int UNUSED=0>
class mouse_button_event_ {
public:
  mouse_button_event_(const SDL_MouseButtonEvent &evt)
      : evt_(evt) {
  }
  ~mouse_button_event_() {
  }

  inline bool down() const {
    return evt_.type == SDL_MOUSEBUTTONDOWN;
  }
  inline bool up() const { return !down(); }
  inline uint16_t x() const {
    return evt_.x;
  }
  inline uint16_t y() const {
    return evt_.y;
  }
  template<typename T>
  inline ghp::vector<2, T> coords() const {
    return ghp::vector2<T>(evt_.x, evt_.y);
  }
  /** \brief which button was pressed. SDL_BUTTON_LEFT,
    SDL_BUTTON_RIGHT and SDL_BUTTON_MIDDLE are possible
    values */
  uint8_t button() const { 
    return evt_.button;
  }

private:
  SDL_MouseButtonEvent evt_;
};
typedef mouse_button_event_<0> mouse_button_event;

template<int UNUSED=0>
class mouse_motion_event_ {
public:
  mouse_motion_event_(const SDL_MouseMotionEvent &evt) 
      : evt_(evt) {
  }
  ~mouse_motion_event_() {
  }

  inline uint16_t x() const { return evt_.x; }
  inline uint16_t y() const { return evt_.y; }
  inline int16_t rel_x() const { return evt_.xrel; }
  inline int16_t rel_y() const { return evt_.yrel; }

  template<typename T>
  inline ghp::vector<2, T> coords() const { 
    return ghp::vector2<T>(evt_.x, evt_.y);
  }
  template<typename T>
  inline ghp::vector<2, T> rel_coords() const {
    return ghp::vector2<T>(evt_.xrel, evt_.yrel);
  }
private:
  SDL_MouseMotionEvent evt_;
};
typedef mouse_motion_event_<0> mouse_motion_event;

template<int UNUSED=0>
class quit_event_ {
};
typedef quit_event_<0> quit_event;

namespace {
  struct combine_or {
    typedef bool result_type;

    template<typename InputIterator>
    bool operator()(InputIterator begin, InputIterator end) const {
      for(; begin != end; ++begin) {
        if(*begin) return true;
      }
      return false;
    }
  };
}

template<int UNUSED=0>
class input_ {
public:
  typedef bool (keyboard_callback_type)(const keyboard_event&);
  typedef boost::signals2::signal<keyboard_callback_type, combine_or> 
      keyboard_slot_type;
  typedef bool (mouse_motion_callback_type)(const mouse_motion_event&);
  typedef boost::signals2::signal<mouse_motion_callback_type, combine_or>
      mouse_motion_slot_type;
  typedef bool (mouse_button_callback_type)(const mouse_button_event&);
  typedef boost::signals2::signal<mouse_button_callback_type, combine_or>
      mouse_button_slot_type;
  typedef bool (quit_callback_type)(const quit_event&);
  typedef boost::signals2::signal<quit_callback_type, combine_or>
      quit_slot_type;

  input_() {
  }
  ~input_() {
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
    SDL_Event evt;
    while(SDL_PollEvent(&evt)) {
      handle_event(evt);
    }
  }

  void handle_event(const SDL_Event &e) {
    switch(e.type) {
      case SDL_KEYDOWN:
      case SDL_KEYUP: {
        keyboard_event evt(e.key);
        keyboard_slot_(evt);
      } break;
      case SDL_MOUSEMOTION: {
        mouse_motion_event evt(e.motion);
        mouse_motion_slot_(evt);
      } break;
      case SDL_MOUSEBUTTONDOWN:
      case SDL_MOUSEBUTTONUP: {
        mouse_button_event evt(e.button);
        mouse_button_slot_(evt);
      } break;
      case SDL_QUIT: {
        quit_event evt;
        quit_slot_(evt);
      } break;
      default:
        break;
    }
  }

  inline void enable_unicode(bool b) {
    SDL_EnableUNICODE(b ? 1 : 0);
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

