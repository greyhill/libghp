#ifndef _GHP_COLOR_HPP_
#define _GHP_COLOR_HPP_

#include <stdint.h>

namespace ghp {

namespace {
  template<typename T> T _color_min() { return T(); }
  template<typename T> T _color_max() { return T(); }
  template<> inline uint8_t _color_min<uint8_t>() { return 0; }
  template<> inline uint8_t _color_max<uint8_t>() { return 255; }
  template<> inline float _color_min<float>() { return 0; }
  template<> inline float _color_max<float>() { return 1; }
}

template<typename T>
class color {
public:
  color() { for(uint8_t i=0; i<4; ++i) data_[i] = 0; }
  color(T red, T green, T blue, T alpha) {
    data_[0] = red;
    data_[1] = green;
    data_[2] = blue;
    data_[3] = alpha;
  }
  color(const color &c) {
    for(uint8_t i=0; i<4; ++i)
      data_[i] = c.data_[i];
  }
  ~color() { }

  T& red() { return data_[0]; }
  T& green() { return data_[1]; }
  T& blue() { return data_[2]; }
  T& alpha() { return data_[3]; }
  T& operator()(uint8_t i) { return data_[i]; }
  const T& red() const { return data_[0]; }
  const T& green() const { return data_[1]; }
  const T& blue() const { return data_[2]; }
  const T& alpha() const { return data_[3]; }
  const T& operator()(uint8_t i) const { return data_[i]; }

  void clamp() {
    for(uint8_t i=0; i<4; ++i) {
      if(data_[i] < _color_min<T>()) 
        data_[i] = _color_min<T>();
      else if(data_[i] > _color_max<T>()) 
        data_[i] = _color_max<T>();
    }
  }

private:
  T data_[4];
};

}

#endif

