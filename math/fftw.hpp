#ifndef _GHP_MATH_FFTW_HPP_
#define _GHP_MATH_FFTW_HPP_

#include <fftw3.h>

#include <complex>

namespace fftw {

template<typename T> struct container_traits {
  typedef typename T::value_type value_type;
};
template<typename T> struct container_traits<T*> {
  typedef T value_type;
};
template<typename T> struct container_traits<const T*> {
  typedef const T value_type;
};

template<typename T> class planner { };

template<> class planner<std::complex<float> > {
};

template<> class planner<std::complex<double> > { 
};

template<> class planner<std::complex<long double> > {
};

template<typename IN, typename OUT>
void fft(const A &in, B &out, std::size_t size) {
  typedef typename container_traits<A>::value_type value_type;
  planner<value_type>::fft(in, out, size);
}


}

#endif

