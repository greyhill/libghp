#ifndef _GHP_GFX_COLOR_UTIL_HPP_
#define _GHP_GFX_COLOR_UTIL_HPP_

namespace ghp {

template<typename PIXELT> struct standard_colors { };
template<typename T> struct standard_colors<RGB<T> > {
  static const color<RGB<T> > RED;
  static const color<RGB<T> > GREEN;
  static const color<RGB<T> > BLUE;
  static const color<RGB<T> > WHITE;
  static const color<RGB<T> > BLACK;
  static const color<RGB<T> > PURPLE;
  static const color<RGB<T> > YELLOW;
  static const color<RGB<T> > CYAN;
};
template<typename T> const color<RGB<T> > 
  standard_colors<RGB<T> >::RED(
    color_traits<T>::max_value, 
    color_traits<T>::min_value, 
    color_traits<T>::min_value);
template<typename T> const color<RGB<T> > 
  standard_colors<RGB<T> >::GREEN(
    color_traits<T>::min_value, 
    color_traits<T>::max_value, 
    color_traits<T>::min_value);
template<typename T> const color<RGB<T> > 
  standard_colors<RGB<T> >::BLUE(
    color_traits<T>::min_value, 
    color_traits<T>::min_value, 
    color_traits<T>::max_value);
template<typename T> const color<RGB<T> > 
  standard_colors<RGB<T> >::WHITE(
    color_traits<T>::max_value, 
    color_traits<T>::max_value, 
    color_traits<T>::max_value);
template<typename T> const color<RGB<T> >
  standard_colors<RGB<T> >::BLACK(
    color_traits<T>::min_value, 
    color_traits<T>::min_value, 
    color_traits<T>::min_value);
template<typename T> const color<RGB<T> >
  standard_colors<RGB<T> >::PURPLE(
    color_traits<T>::max_value, 
    color_traits<T>::min_value, 
    color_traits<T>::max_value);
template<typename T> const color<RGB<T> >
  standard_colors<RGB<T> >::YELLOW(
    color_traits<T>::max_value, 
    color_traits<T>::max_value, 
    color_traits<T>::min_value);
template<typename T> const color<RGB<T> >
  standard_colors<RGB<T> >::CYAN(
    color_traits<T>::min_value, 
    color_traits<T>::max_value, 
    color_traits<T>::max_value);

template<typename T> struct standard_colors<RGBA<T> > {
  static const color<RGBA<T> > RED;
  static const color<RGBA<T> > GREEN;
  static const color<RGBA<T> > BLUE;
  static const color<RGBA<T> > WHITE;
  static const color<RGBA<T> > BLACK;
  static const color<RGBA<T> > PURPLE;
  static const color<RGBA<T> > YELLOW;
  static const color<RGBA<T> > CYAN;
  static const color<RGBA<T> > TRANSPARENT;
};
template<typename T> const color<RGBA<T> >
  standard_colors<RGBA<T> >::RED(
    color_traits<T>::max_value, 
    color_traits<T>::min_value, 
    color_traits<T>::min_value, 
    color_traits<T>::max_value);
template<typename T> const color<RGBA<T> >
  standard_colors<RGBA<T> >::GREEN(
    color_traits<T>::min_value, 
    color_traits<T>::max_value, 
    color_traits<T>::min_value, 
    color_traits<T>::max_value);
template<typename T> const color<RGBA<T> >
  standard_colors<RGBA<T> >::BLUE(
    color_traits<T>::min_value, 
    color_traits<T>::min_value,
    color_traits<T>::max_value, 
    color_traits<T>::max_value);
template<typename T> const color<RGBA<T> >
  standard_colors<RGBA<T> >::WHITE(
    color_traits<T>::max_value, 
    color_traits<T>::max_value, 
    color_traits<T>::max_value,
    color_traits<T>::max_value);
template<typename T> const color<RGBA<T> >
  standard_colors<RGBA<T> >::BLACK(
    color_traits<T>::min_value, 
    color_traits<T>::min_value, 
    color_traits<T>::min_value, 
    color_traits<T>::max_value);
template<typename T> const color<RGBA<T> >
  standard_colors<RGBA<T> >::PURPLE(
    color_traits<T>::max_value, 
    color_traits<T>::min_value, 
    color_traits<T>::max_value, 
    color_traits<T>::max_value);
template<typename T> const color<RGBA<T> >
  standard_colors<RGBA<T> >::YELLOW(
    color_traits<T>::max_value, 
    color_traits<T>::max_value, 
    color_traits<T>::min_value, 
    color_traits<T>::max_value);
template<typename T> const color<RGBA<T> >
  standard_colors<RGBA<T> >::CYAN(
    color_traits<T>::min_value, 
    color_traits<T>::max_value, 
    color_traits<T>::max_value, 
    color_traits<T>::max_value);
template<typename T> const color<RGBA<T> >
  standard_colors<RGBA<T> >::TRANSPARENT(
    color_traits<T>::min_value,
    color_traits<T>::min_value, 
    color_traits<T>::min_value, 
    color_traits<T>::max_value);

}

#endif

