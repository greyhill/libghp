#ifndef _GHP_MATH_FFTW_HPP_
#define _GHP_MATH_FFTW_HPP_

#include <boost/thread.hpp>

#include <fftw3.h>

#include <algorithm>
#include <complex>
#include <map>

#include <stdlib.h>

namespace fftw {

/*
  This header is broken into several parts: 
    1) an allocator is provided for creating containers with 
      SIMD-aligned contents
    2) an plan object is provided to create FFTW plans and 
      perform DFTs
    3) a number of MATLAB-like convenience methods are provided
      for performing DFTs with transient plans
 */

/** \brief C++ allocator for SIMD-aligned memory
  This allocator uses FFTW's fftw_malloc and fftw_free functions to
  provide SIMD alignment for STL and STL-like data structures.  SIMD
  alignment allows, e.g. FFTW to take advantage of processor features
  to significantly increate DFT performance.  While not necessary,
  its use is strongly encouraged.

  This type supports both the Allocator concept from the STL and
  the User Allocator concept from Boost's memory pool library
 */
template<typename T>
class fftw_alloc {
public:
  typedef T value_type;
  typedef value_type* pointer;
  typedef const value_type* const_pointer;
  typedef value_type& reference;
  typedef const value_type& const_reference;
  typedef std::size_t size_type;
  typedef std::ptrdiff_t difference_type;

  template<typename U>
  struct rebind {
    typedef fftw_alloc<U> other;
  };

  inline fftw_alloc() { }
  inline ~fftw_alloc() { }
  inline fftw_alloc(const fftw_alloc&) { }
  template<typename U>
  inline fftw_alloc(fftw_alloc<U> const&) { }

  inline pointer address(reference r) { return &r; }
  inline const_pointer address(const_reference r) { return &r; }

  inline pointer allocate(size_type cnt, 
      typename std::allocator<void>::const_pointer = 0) {
    return reinterpret_cast<pointer>(fftw_malloc(cnt*sizeof(T)));
  }
  inline void deallocate(pointer p, size_type) {
    fftw_free(p);
  }

  inline size_type max_size() const {
    return std::numeric_limits<size_type>::max() / sizeof(T);
  }

  inline void construct(pointer p, const T& t) { new(p) T(t); }
  inline void destroy(pointer p) { p->~T(); }
  inline bool operator==(const fftw_alloc&) { return true; }
  inline bool operator!=(const fftw_alloc&) { return false; }

  inline static char* malloc(size_type n) { 
    return reinterpret_cast<char*>(fftw_malloc(n)); 
  }
  inline static void free(char * const block) { fftw_free(block); }
};

template<typename T> struct fftw2cpp { };
template<> struct fftw2cpp<fftwf_complex> { 
  typedef std::complex<float> value_type;
};
template<> struct fftw2cpp<fftw_complex> {
  typedef std::complex<double> value_type;
};
template<> struct fftw2cpp<fftwl_complex> {
  typedef std::complex<long double> value_type;
};

template<typename T> struct cpp2fftw { };
template<> struct cpp2fftw<std::complex<float> > {
  typedef fftwf_complex value_type;
};
template<> struct cpp2fftw<std::complex<double> > {
  typedef fftw_complex value_type;
};
template<> struct cpp2fftw<std::complex<long double> > {
  typedef fftwl_complex value_type;
};

template<typename T, int N=0> struct fftw_type_traits { };
template<int N> struct fftw_type_traits<fftwf_complex, N> {
  typedef fftwf_plan plan_type;
  typedef fftwf_complex complex_type;

  static inline plan_type create_plan(
      int dim,
      const int *sizes,
      complex_type *in,
      complex_type *out,
      int sign,
      unsigned flags) {
    boost::mutex::scoped_lock lock(mutex_);
    return fftwf_plan_dft(dim, sizes, in, out, sign, flags);
  }
  static inline void destroy_plan(plan_type plan) {
    boost::mutex::scoped_lock lock(mutex_);
    fftwf_destroy_plan(plan);
  }
  static inline void execute_plan(plan_type plan) {
    fftwf_execute(plan);
  }

private:
  static boost::mutex mutex_;
};
template<int N> boost::mutex fftw_type_traits<fftwf_complex, N>::mutex_;

template<int N> struct fftw_type_traits<fftw_complex, N> {
  typedef fftw_plan plan_type;
  typedef fftw_complex complex_type;

  static inline plan_type create_plan(
      int dim,
      const int *sizes,
      complex_type *in,
      complex_type *out,
      int sign,
      unsigned flags) {
    boost::mutex::scoped_lock lock(mutex_);
    return fftw_plan_dft(dim, sizes, in, out, sign, flags);
  }
  static inline void destroy_plan(plan_type plan) {
    boost::mutex::scoped_lock lock(mutex_);
    fftw_destroy_plan(plan);
  }
  static inline void execute_plan(plan_type plan) {
    fftw_execute(plan);
  }

private:
  static boost::mutex mutex_;
};
template<int N> boost::mutex fftw_type_traits<fftw_complex, N>::mutex_;

// TODO figure out what's going on here
/* NOTE: my copy of FFTW doesn't like "long double"s, so I've commented out
  support here. */
/*
template<int N> struct fftw_type_traits<fftwl_complex, N> {
  typedef fftwl_plan plan_type;
  typedef fftwl_complex complex_type;

  static inline plan_type create_plan(
      int dim,
      const int *sizes,
      complex_type *in,
      complex_type *out,
      int sign,
      unsigned flags) {
    boost::mutex::scoped_lock lock(mutex_);
    return fftwl_plan_dft(dim, sizes, in, out, sign, flags);
  }
  static inline void destroy_plan(plan_type plan) {
    boost::mutex::scoped_lock lock(mutex_);
    fftwl_destroy_plan(plan);
  }
  static inline void execute_plan(plan_type plan) {
    fftwl_execute(plan);
  }

private:
  static boost::mutex mutex_;
};
template<int N> boost::mutex fftw_type_traits<fftwl_complex, N>::mutex_;
*/

/** \brief threadsafe functor for performing a DFT with FFTW
  FFTW is a library for performing Fast Fourier Transforms.  This plan functor
  wraps FFTW's "basic DFT" API in a threadsafe manner.  That is, multiple
  threads can have their own plan objects; multiple threads probably shouldn't
  operate on a single plan object at once.
  \tparam T - a std::complex<TYPE>, where TYPE can be float, double or long 
  double
 */
template<typename T>
class plan {
public:
  typedef T cpp_type;
  typedef typename cpp2fftw<T>::value_type fftw_type;
  typedef typename fftw_type_traits<fftw_type>::plan_type plan_type;
  
  /**
    \brief plan a multidimensional DFT
    \tparam IN - input type.  Must be conceptually like an array, with
      linear memory arrangement: &in[0] should point to the beginning
      of the array.  For best performance, use a container utilizing the 
      fftw_alloc allocator above.
    \tparam OUT - output type.  Same requirements as input.
    \tparam S - size array type.  Must be [.]-accessible.
    \param dim - dimension of DFT, i.e. 1 is 1-dimensional
    \param forward - true for DFT, false for IDFT
    \param in - input
    \param output - output
    \param sizes - sizes of each dimension
   */
  template<typename IN, typename OUT, typename S>
  inline plan(std::size_t dim, bool forward, IN in, OUT out,
      S sizes) {
    int dims[dim];
    for(int i=0; i<dim; ++i) dims[i] = sizes[i];
    plan_ = fftw_type_traits<fftw_type>::create_plan(
      dim,
      dims,
      in,
      out,
      forward ? 1 : 0,
      0);
    if(plan_ == NULL) {
      throw std::runtime_error("couldn't create plan in FFTW");
    }
  }
  /**
    \brief plan a one-dimensional DFT
    \tparam IN - input type.  Must be conceptually like an array, with
      linear memory arrangement: &in[0] should point to the beginning
      of the array.  For best performance, use a container utilizing the 
      fftw_alloc allocator above.  Also, must have .size() method.
    \tparam OUT - output type.  Same requirements as input.
    \tparam S - size array type.  Must be [.]-accessible.
    \param forward - true for DFT, false for IDFT
    \param in - input
    \param output - output
   */
  template<typename IN, typename OUT>
  inline plan(bool forward, IN in, OUT out) {
    int size = in.size();
    plan_ = fftw_type_traits<fftw_type>::create_plan(
      1,
      &size,
      in,
      out,
      forward ? 1 : 0,
      0);
    if(plan_ == NULL) {
      throw std::runtime_error("couldn't create plan in FFTW");
    }
  }
  inline ~plan() {
    fftw_type_traits<fftw_type>::destroy_plan(plan_);
  }

  /** \brief perform the DFT operation.  can be called multiple
    times; each time operates on the same input and output vectors,
    but you _can_ change their contents in-between runs */
  inline void operator()() {
    fftw_type_traits<fftw_type>::execute_plan(plan_);
  }

private:
  plan_type plan_;
};

}

#endif

