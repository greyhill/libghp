#ifndef _GHP_MATH_FFTW_HPP_
#define _GHP_MATH_FFTW_HPP_

#include <boost/thread/thread.hpp>

#include <fftw3.h>

#include <algorithm>
#include <complex>

#include <stdlib.h>

namespace fftw {

/* type traits for indentifying the underlying value type 
  in a container.  STL containers have a value_type field,
  and specializations are provided for arrays and const arrays. */
template<typename T> struct container_traits {
  typedef typename T::value_type value_type;
};
template<typename T> struct container_traits<T*> {
  typedef T value_type;
};
template<typename T> struct container_traits<const T*> {
  typedef const T value_type;
};

template<typename T> struct fftw_type_map { };
template<> struct fftw_type_map<std::complex<float> > {
  typedef fftwf_plan plan_type;
  typedef fftwf_complex complex_type;
  inline static plan_type make_plan(
      int rank, 
      const int *dims,
      complex_type *in,
      complex_type *out,
      unsigned flags) {
    return fftwf_plan_dft(rank, dims, in, out, flags);
  }
  inline static void execute(const plan_type &t) { fftwf_execute(t); }
  inline static void destroy(const plan_type &t) { fftwf_destroy_plan(t); }
};
template<> struct fftw_type_map<std::complex<double> > {
  typedef fftw_plan plan_type;
  typedef fftw_complex complex_type;
  inline static plan_type make_plan(
      int rank,
      const int *dims,
      complex_type *in,
      complex_type *out,
      unsigned flags) {
    return fftw_plan_dft(rank, dims, in, out, flags);
  }
  inline static void execute(const plan_type &t) { fftw_execute(t); }
  inline static void destroy(const plan_type &t) { fftw_destroy_plan(t); }
};
template<> struct fftw_type_map<std::complex<long double> > {
  typedef fftwl_plan plan_type;
  typedef fftwl_complex complex_type;
  inline static plan_type make_plan(
      int rank,
      const int *dims,
      complex_type *in,
      complex_type *out,
      unsigned flags) {
    return fftwl_plan_dft(rank, dims, in, out, flags);
  }
  inline static void execute(const plan_type &t) { fftwl_execute(t); }
  inline static void destroy(const plan_type &t) { fftwl_destroy_plan(t); }
};

template<typename PLAN_TYPE>
class fftw_globals_ {
private:
  /** [In-place/Out-of-place][Forward/Backward] */
  struct plan_glob_ {
    plan_glob_(std::size_t size) {
    }
    ~plan_glob_() {
    }

    PLAN_TYPE ipf_;
    PLAN_TYPE ipb_;
    PLAN_TYPE opf_;
    PLAN_TYPE opb_;
  };

  static std::map<uint32_t, std::map<uint32_t, plan_glob*>*> plans_;
  static boost::mutex mutex_;
};

}

#endif

