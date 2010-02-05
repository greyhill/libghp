#ifndef _GHP_GL_WINDOW_HPP_
#define _GHP_GL_WINDOW_HPP_

#include "stdint.h"

#include <boost/noncopyable.hpp>

#include <string>

namespace ghp {

template<typename API>
class window : public boost::noncopyable {
public:
  static window& get();

  window(uint32_t w, uint32_t h, bool fs, std::string &caption)
      : obj_(NULL) {
    obj_ = new API::window_object(
      width_, height_, fullscreen_, caption_
    );
  }
  ~window() {
    delete obj_;
  }

  uint32_t get_width() const {
    obj_->get_width();
  }
  uint32_t get_height() const {
    obj_->get_height();
  }
  bool is_fullscreen() const {
    obj_->is_fullscreen();
  }
  float get_ratio() const {
    return float(width_)/float(height_);
  }

  window& set_caption(const std::string &s) {
    obj_->set_caption(s);
    return *this;
  }
  std::string get_caption() const {
    return obj_->get_caption();
  }

private:
  void init_();

  static window *window_;
  API::window_object *obj_;
};

}

#endif

