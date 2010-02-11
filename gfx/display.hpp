#ifndef _GHP_DISPLAY_HPP_
#define _GHP_DISPLAY_HPP_

#include <boost/noncopyable.hpp>

#include <string>

#include <stdint.h>

namespace ghp {

class display : boost::noncopyable {
public:
  display(uint32_t width, uint32_t height, bool fs = false,
      const std::string &caption = "");
  ~display();

  uint32_t get_width() const;
  uint32_t get_height() const;
  bool get_fullscreen() const;
  float get_ratio() const;
  const std::string& get_caption() const;

  display& set_caption(const std::string &s);

private:
  uint32_t width_;
  uint32_t height_;
  bool fullscreen_;
  float ratio_;
  std::string caption_;
};

}

#endif

