#ifndef _GHP_GFX_COLOR_HPP_
#define _GHP_GFX_COLOR_HPP_

#include <ghp/math.hpp>

#include <algorithm>
#include <iostream>

#include <cstdarg>

namespace ghp {

template<typename T> struct color_traits { };

template<> struct color_traits<float> {
  static const float min_value;
  static const float max_value;
};
const float color_traits<float>::min_value = 0;
const float color_traits<float>::max_value = 1;

template<> struct color_traits<double> {
  static const double min_value;
  static const double max_value;
};
const double color_traits<double>::min_value = 0;
const double color_traits<double>::max_value = 1;

template<> struct color_traits<uint8_t> {
  static const uint8_t min_value;
  static const uint8_t max_value;
};
const uint8_t color_traits<uint8_t>::min_value = 0;
const uint8_t color_traits<uint8_t>::max_value = 0xFF;

template<> struct color_traits<uint16_t> {
  static const uint16_t min_value;
  static const uint16_t max_value;
};
const uint16_t color_traits<uint16_t>::min_value = 0;
const uint16_t color_traits<uint16_t>::max_value = 0xFFFF;

template<> struct color_traits<uint32_t> {
  static const uint32_t min_value;
  static const uint32_t max_value;
};
const uint32_t color_traits<uint32_t>::min_value = 0;
const uint32_t color_traits<uint32_t>::max_value = 0xFFFFFFFF;

template<> struct color_traits<uint64_t> {
  static const uint64_t min_value;
  static const uint64_t max_value;
};
const uint64_t color_traits<uint64_t>::min_value = 0;
const uint64_t color_traits<uint64_t>::max_value = 
  0xFFFFFFFFFFFFFFFFL;

/** \brief pixel type for RGB layouts
 * \tparam T - underlying type of data
 */
template<typename T>
class RGB {
public:
  typedef T value_t;
  enum { num_channels = 3 };
  enum { bytes_per_pixel = num_channels * sizeof(T) };

  RGB() {
    for(uint8_t i=0; i<num_channels; ++i) 
      data_[i] = color_traits<T>::min_value;
  }
  RGB(T r, 
      T g = color_traits<T>::min_value, 
      T b = color_traits<T>::min_value) {
    data_[0] = r;
    data_[1] = g;
    data_[2] = b;
  }
  ~RGB() { }

  inline T& red() { return data_[0]; }
  inline const T& red() const { return data_[0]; }
  inline T& green() { return data_[1]; }
  inline const T& green() const { return data_[1]; }
  inline T& blue() { return data_[2]; }
  inline const T& blue() const { return data_[2]; }

  inline T& operator()(uint8_t i) { return data_[i]; }
  inline const T& operator()(uint8_t i) const { return data_[i]; }
  inline T& operator[](uint8_t i) { return data_[i]; }
  inline const T& operator[](uint8_t i) const { return data_[i]; }

private:
  T data_[3];
};

/**
  \brief pixel type for RGBA pixel layouts
  \tparam T - underlying pixel type
 */
template<typename T>
class RGBA {
public:
  typedef T value_t;
  enum { num_channels = 4 };
  enum { bytes_per_pixel = num_channels * sizeof(T) };

  RGBA() {
    for(uint8_t i=0; i<num_channels; ++i) 
      data_[i] = color_traits<T>::min_value;
  }
  RGBA(T r, 
      T g = color_traits<T>::min_value, 
      T b = color_traits<T>::min_value, 
      T a = color_traits<T>::min_value) {
    data_[0] = r;
    data_[1] = g;
    data_[2] = b;
    data_[3] = a;
  }
  ~RGBA() { }

  inline T& red() { return data_[0]; }
  inline const T& red() const { return data_[0]; }
  inline T& green() { return data_[1]; }
  inline const T& green() const { return data_[1]; }
  inline T& blue() { return data_[2]; }
  inline const T& blue() const { return data_[2]; }
  inline T& alpha() { return data_[3]; }
  inline const T& alpha() const { return data_[3]; }

  inline T& operator()(uint8_t i) { return data_[i]; }
  inline const T& operator()(uint8_t i) const { return data_[i]; }
  inline T& operator[](uint8_t i) { return data_[i]; }
  inline const T& operator[](uint8_t i) const { return data_[i]; }

private:
  T data_[4];
};

/**
  \brief convertable, generic color interface
  the color<PIXELT> class exposes concepts of color across multiple
  representations.  For example, a color<RGB<uint8_t> > represents
  RGB color with 256 different values, while a color<RGBA<float> >
  represents the OpenGL-like 4-channel HDR values between 0 and 1.
  color<PIXELT>'s are reasonably convertible between different
  formats.
  \tparam PIXELT - some pixel type, like RGB<T> or RGBA<T>.
 */
template<typename PIXELT> class color : public PIXELT {
public:
  color() {
  }
  color(typename PIXELT::value_t a0) 
      : PIXELT(a0) {
  }
  color(typename PIXELT::value_t a0,
    typename PIXELT::value_t a1)
      : PIXELT(a0, a1) {
  }
  color(typename PIXELT::value_t a0,
    typename PIXELT::value_t a1,
    typename PIXELT::value_t a2)
      : PIXELT(a0, a1, a2) {
  }
  color(typename PIXELT::value_t a0,
    typename PIXELT::value_t a1,
    typename PIXELT::value_t a2,
    typename PIXELT::value_t a3)
      : PIXELT(a0, a1, a2, a3) {
  }

  inline color<PIXELT> operator+(const color<PIXELT> &c) const {
    color<PIXELT> ret;
    for(int i=0; i<PIXELT::num_channels; ++i) {
      ret(i) = (*this)(i) + c(i);
    }
    return ret;
  }
  inline color<PIXELT> operator-(const color<PIXELT> &c) const {
    color<PIXELT> ret;
    for(int i=0; i<PIXELT::num_channels; ++i) {
      ret(i) = (*this)(i) - c(i);
    }
    return ret;
  }
  template<typename T>
  inline color<PIXELT> operator*(const T &t) const {
    color<PIXELT> ret;
    for(int i=0; i<PIXELT::num_channels; ++i) {
      ret(i) = (*this)(i) *= t;
    }
    return ret;
  }
  template<typename T>
  inline color<PIXELT> operator/(const T &t) const {
    return (*this) * (static_cast<T>(1) / t);
  }
  inline color<PIXELT>& operator+=(const color<PIXELT> &c) {
    for(int i=0; i<PIXELT::num_channels; ++i) {
      (*this)(i) += c(i);
    }
    return *this;
  }
  inline color<PIXELT>& operator-=(const color<PIXELT> &c) {
    for(int i=0; i<PIXELT::num_channels; ++i) {
      (*this)(i) -= c(i);
    }
    return *this;
  }
  template<typename T>
  inline color<PIXELT>& operator*=(const T &t) {
    for(int i=0; i<PIXELT::num_channels; ++i) {
      (*this)(i) *= t;
    }
    return *this;
  }
  template<typename T>
  inline color<PIXELT>& operator/=(const T &t) {
    return (*this) *= (static_cast<T>(1) / t);
  }

  /** \brief convert between different pixel types */
  template<typename PIXEL2>
  inline operator color<PIXEL2>() const {
    color<PIXEL2> to_return;
    convert_color(*this, to_return);
    return to_return;
  }
};

// template-specialize this function for more sophisticated
// conversions, e.g. between CMY and RGB.  specializations
// will be automatically picked up by color<>'s cast
// operator.
template<typename PIXELT1, typename PIXELT2>
inline void convert_color(const color<PIXELT1> &src, color<PIXELT2> &dest) {
  int min_ch = std::min(
    static_cast<int>(PIXELT1::num_channels),
    static_cast<int>(PIXELT2::num_channels));
  for(int i=0; i<min_ch; ++i) {
    dest(i) = linear_interpolate(
      color_traits<typename PIXELT2::value_t>::min_value,
      color_traits<typename PIXELT2::value_t>::max_value,
      color_traits<typename PIXELT1::value_t>::min_value,
      color_traits<typename PIXELT2::value_t>::max_value,
      src(i)
    );
  }
}

template<typename PIXELT>
std::ostream& operator<<(std::ostream& o, const color<PIXELT> &c) {
  o << "channels: "
    << PIXELT::num_channels 
    << ", bytes per pixel: "
    << PIXELT::bytes_per_pixel
    << ", data: (";
  for(int i=0; i<PIXELT::num_channels; ++i) {
    o << c(i);
    if(i == PIXELT::num_channels - 1) {
      o << ")";
    } else {
      o << ", ";
    }
  }
  return o;
}

}

#endif

