#ifndef _GHP_COLOR_HPP_
#define _GHP_COLOR_HPP_

#include "../math.hpp"

#include <stdint.h>

namespace ghp {

namespace {
  template<typename T> T _color_min() { return T(); }
  template<typename T> T _color_max() { return T(); }
  template<> inline uint8_t _color_min<uint8_t>() { return 0; }
  template<> inline uint8_t _color_max<uint8_t>() { return 255; }
  template<> inline float _color_min<float>() { return 0; }
  template<> inline float _color_max<float>() { return 1; }
  // no idea why someone would want double precision color values,
  // but there it is.
  template<> inline double _color_min<double>() { return 0; }
  template<> inline double _color_max<double>() { return 1; }
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

  // a little bit of color arithmetic
  color operator+(const color &c) const {
    return color( red() + c.red(),
      green() + c.green(),
      blue() + c.blue(),
      alpha() + c.alpha()
    );
  }
  color operator-(const color &c) const {
    return color( red() - c.red(),
      green() - c.green(),
      blue() - c.blue(),
      alpha() - c.alpha()
    );
  }
  template<typename S>
  color operator*(const S &s) const {
    return color(
      red() * s,
      blue() * s,
      green() * s,
      alpha() * s
    );
  }
  color& operator+=(const color &c) {
    red() += c.red();
    blue() += c.blue();
    green() += c.green();
    alpha() += c.alpha();
    return *this;
  }
  color& operator-=(const color &c) {
    red() -= c.red();
    blue() -= c.blue();
    green() -= c.green();
    alpha() -= c.alpha();
    return *this;
  }
  template<typename S>
  color& operator*=(const S &s) {
    red() *= s;
    blue() *= s;
    green() *= s;
    alpha() *= s;
    return *this;
  }

  template<typename S>
  operator const color<S>() const {
    return color<S>(
      linear_interpolate(_color_min<S>(), _color_max<S>(), 
        static_cast<float>(red() - _color_min<T>()) 
          / static_cast<float>(_color_max<T>())),
      linear_interpolate(_color_min<S>(), _color_max<S>(), 
        static_cast<float>(green() - _color_min<T>()) 
          / static_cast<float>(_color_max<T>())),
      linear_interpolate(_color_min<S>(), _color_max<S>(), 
        static_cast<float>(blue() - _color_min<T>()) 
          / static_cast<float>(_color_max<T>())),
      linear_interpolate(_color_min<S>(), _color_max<S>(), 
        static_cast<float>(alpha() - _color_min<T>()) 
          / static_cast<float>(_color_max<T>()))
    );
  }

private:
  T data_[4];
};

}

#endif

