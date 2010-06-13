#ifndef _GHP_MATH_FFTW_HPP_
#define _GHP_MATH_FFTW_HPP_

#include "../util.hpp"

#include <boost/thread.hpp>

#include <fftw3.h>

#include <algorithm>
#include <complex>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <map>
#include <vector>

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
class simd_alloc {
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
    typedef simd_alloc<U> other;
  };

  inline simd_alloc() { }
  inline ~simd_alloc() { }
  inline simd_alloc(const simd_alloc&) { }
  template<typename U>
  inline simd_alloc(simd_alloc<U> const&) { }

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
  inline bool operator==(const simd_alloc&) { return true; }
  inline bool operator!=(const simd_alloc&) { return false; }

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
  typedef T fftw_type;
  typedef typename fftw_type_traits<fftw_type>::plan_type plan_type;
  
  /**
    \brief plan a multidimensional DFT
    \tparam IN - input type.  Must be conceptually like an array, with
      linear memory arrangement: &in[0] should point to the beginning
      of the array.  For best performance, use a container utilizing the 
      simd_alloc allocator above.
    \tparam OUT - output type.  Same requirements as input.
    \tparam S - size array type.  Must be [.]-accessible.
    \param dim - dimension of DFT, i.e. 1 is 1-dimensional
    \param forward - true for DFT, false for IDFT
    \param in - input
    \param output - output
    \param sizes - sizes of each dimension
   */
  template<typename IN, typename OUT, typename S>
  inline plan(std::size_t dim, bool forward, const IN &in, OUT &out,
      S sizes)
        : plan_(NULL) {
    int dims[dim];
    for(int i=0; i<dim; ++i) dims[i] = sizes[i];
    fftw_type* in_ptr = reinterpret_cast<fftw_type*>(
      &const_cast<IN&>(in)[0]);
    fftw_type* out_ptr = reinterpret_cast<fftw_type*>(&out[0]);
    if(in_ptr == out_ptr) {
      // in-place transform: first check if we can do an in-place
      // transform just using wisdom.  this would be non-destructive
      // of the input data
      plan_ = fftw_type_traits<fftw_type>::create_plan(
        dim,
        dims,
        in_ptr,
        out_ptr,
        forward ? FFTW_FORWARD : FFTW_BACKWARD,
        FFTW_WISDOM_ONLY);
      if(plan_ == NULL) {
        // plan could not be created purely from wisdom. we need to
        // save the input data, create the plan, then restore the
        // input data
        std::size_t num_samples = 1;
        for(std::size_t i=0; i<dim; ++i) num_samples *= dims[i];
        fftw_type buffer[num_samples];
        memcpy(buffer, in_ptr, sizeof(fftw_type)*num_samples);
        plan_ = fftw_type_traits<fftw_type>::create_plan(
          dim,
          dims,
          in_ptr,
          out_ptr,
          forward ? FFTW_FORWARD : FFTW_BACKWARD,
          0);
        memcpy(in_ptr, buffer, sizeof(fftw_type)*num_samples);
      }
    } else {
      // out-of-place transform; simple create the plan as normal
      plan_ = fftw_type_traits<fftw_type>::create_plan(
        dim,
        dims,
        in_ptr,
        out_ptr,
        forward ? FFTW_FORWARD : FFTW_BACKWARD,
        0);
    }
    if(plan_ == NULL) {
      // if the plan is still NULL, there was a real algorithmic
      // failure
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
  /** \brief perform the DFT operation.  can be called multiple
    times; each time operates on the same input and output vectors,
    but you _can_ change their contents in-between runs */
  inline void execute() {
    (*this)();
  }

  /** \brief perform the DFT operation on a new set of arrays.
   the operation must be identical in every way to the one set up
   originally, _including whether or not the input and output
   parameters are SIMD-aligned_.
   \tparam IN - input type.  Must be conceptually like an array, with
      linear memory arrangement: &in[0] should point to the beginning
      of the array.  For best performance, use a container utilizing the 
      simd_alloc allocator above.
   \tparam OUT - output type.  Same requirements as input.
   \param in - input
   \param out - output
  */
  template<typename IN, typename OUT>
  inline void unsafe_execute(const T &in, T &out) {
    fftw_type* in_ptr = reinterpret_cast<fftw_type*>(
      &const_cast<IN&>(in)[0]);
    fftw_type* out_ptr = reinterpret_cast<fftw_type*>(&out[0]);
    fftw_execute_dft(plan_, in_ptr, out_ptr);
  }

private:
  plan_type plan_;
};

/*
  here are a number of MATLAB-like convenience functions.  
*/

/* don't use this function */
template<typename IN, typename OUT, bool FORWARD>
inline void general_fft_(const IN &in, OUT &out, std::size_t in_size, 
    std::size_t out_size) {
  typedef typename ghp::container_traits<IN>::value_type cpp_type;
  typedef typename cpp_type::value_type real_type;
  typedef typename cpp2fftw<cpp_type>::value_type fftw_type;
  /* In the case where input and output sizes differ:
    1) in == out: Perform an [i]fft as normal
    2) in < out: Copy in to out w/ zero-padding and fft out in-place
    3) in > out: Perform an [i]fft from in to out, truncating elements of
      in. */
  if(in_size < out_size) {
    // TODO maybe replace with a memset and memcpy?
    for(std::size_t i=0; i<in_size; ++i) {
      out[i] = in[i];
    }
    for(std::size_t i=in_size; i<out_size; ++i) {
      out[i] = 0;
    }
    general_fft_<OUT, OUT, FORWARD>(out, out, out_size, out_size);
  } else {
    std::size_t sizes[] = { in_size, out_size };
    plan<fftw_type> plan_fct(
      1,
      FORWARD,
      in,
      out,
      sizes
    );
    plan_fct();
    // FFTW doesn't scale values on ifft.  We do that here.
    if(!FORWARD) {
      real_type den = real_type(1.0) / out_size;
      for(int i=0; i<out_size; ++i) out[i] *= den;
    }
  }
}

/** \brief compute a 1D DFT 
  \tparam IN - input.  Must be indexable with linear memory and
    have .size() method
  \tparam OUT - output.  Same properties as input.
  \param in - input data
  \param output - output data.  Must be sized appropriately.  To
    upsample the DFT, use an output vector larger than the 
    input vector */
template<typename IN, typename OUT>
inline void fft(const IN &in, OUT &out) {
  general_fft_<IN, OUT, true>(in, out, in.size(), out.size());
}
/** \brief compute a 1D inverse DFT 
  \tparam IN - input.  Must be indexable with linear memory and
    have .size() method
  \tparam OUT - output.  Same properties as input.
  \param in - input data
  \param output - output data.  Must be sized appropriately.  To
    upsample the IDFT, use an output vector larger than the 
    input vector */
template<typename IN, typename OUT>
inline void ifft(const IN &in, OUT &out) {
  general_fft_<IN, OUT, false>(in, out, in.size(), out.size());
}
/** \brief compute a 1D DFT 
  \tparam IN - input.  Must be indexable with linear memory.
  \tparam OUT - output.  Same properties as input.
  \param in - input data
  \param output - output data.  Must be sized appropriately.  To
    upsample the DFT, use an output vector larger than the 
    input vector
  \param in_size - size of input vector
  \param out_size - size of output vector */
template<typename IN, typename OUT>
inline void fft(const IN &in, OUT &out, std::size_t in_size,
    std::size_t out_size) {
  general_fft_<IN, OUT, true>(in, out, in_size, out_size);
}
/** \brief compute a 1D inverse DFT 
  \tparam IN - input.  Must be indexable with linear memory.
  \tparam OUT - output.  Same properties as input.
  \param in - input data
  \param output - output data.  Must be sized appropriately.  To
    upsample the IDFT, use an output vector larger than the 
    input vector
  \param in_size - size of input vector
  \param out_size - size of output vector */
template<typename IN, typename OUT>
inline void ifft(const IN &in, OUT &out, std::size_t in_size,
    std::size_t out_size) {
  general_fft_<IN, OUT, false>(in, out, in_size, out_size);
}

/** \brief compute a 1D convolution quickly using FFTs
  \tparam IN1 - input 1.  Must be indexable with linear memory.
  \tparam IN2 - input 2.  Must be indexable with linear memory.
  \tparam OUT - output.  Must be indexable with linear memory
  \param in1 - input series 1
  \param in2 - input series 2
  \param out - output vector.  Must be preallocated with enough
    room for the convolution: in1_size+in2_size-1
  \param in1_size - size of input series 1
  \param in2_size - size of input series 2
 */
template<typename IN1, typename IN2, typename OUT>
inline void conv(const IN1 &in1, const IN2 &in2, OUT &out, 
    std::size_t in1_size, std::size_t in2_size) {
  typedef typename ghp::container_traits<IN1>::value_type in1_value_type;
  typedef typename ghp::container_traits<IN2>::value_type in2_value_type;
  typedef typename cpp2fftw<in1_value_type>::value_type fftw_type_1;
  typedef typename cpp2fftw<in2_value_type>::value_type fftw_type_2;
  typedef typename fftw2cpp<fftw_type_1>::value_type cpp_type_1;
  typedef typename fftw2cpp<fftw_type_2>::value_type cpp_type_2;

  const std::size_t filt_size = in1_size + in2_size - 1;
  std::vector<cpp_type_1, simd_alloc<cpp_type_1> > filt1(filt_size);
  std::vector<cpp_type_2, simd_alloc<cpp_type_2> > filt2(filt_size);

  memcpy(&filt1[0], &in1[0], sizeof(cpp_type_1)*in1_size);
  fft(filt1, filt1, filt_size, filt_size);
  fft(in2, filt2, in2_size, filt_size);
  for(std::size_t i=0; i<filt_size; ++i) {
    out[i] = filt1[i] * filt2[i];
  }
  ifft(out, out, filt_size, filt_size);
}
/** \brief compute a 1D convolution quickly using FFTs
  \tparam IN1 - input 1.  Must be indexable with linear memory and have
    .size() method
  \tparam IN2 - input 2.  Must be indexable with linear memory and have
    .size() method
  \tparam OUT - output.  Must be indexable with linear memory
  \param in1 - input series 1
  \param in2 - input series 2
  \param out - output vector.  Must be preallocated with enough
    room for the convolution: in1_size+in2_size-1
 */
template<typename IN1, typename IN2, typename OUT>
inline void conv(const IN1 &in1, const IN2 &in2, OUT &out) {
  conv(in1, in2, out, in1.size(), in2.size());
}

}

#endif

