#ifndef _GHP_UTIL_MATLAB_HPP_
#define _GHP_UTIL_MATLAB_HPP_

#include <matrix.h>
#include <mex.h>

#include <string>
#include <stdexcept>
#include <vector>

namespace matlab {

template<int UNUSED> class mex_ref_;
typedef mex_ref_<0> mex_ref;

void mex_implementation(
    std::vector<mex_ref> &input,
    std::vector<mex_ref> &output);

template<typename T> struct cpp2matlab { };

#define CPP2MATLAB(CPP, MATLAB, MATLAB2) \
  template<> struct cpp2matlab<CPP> { \
    static const char *class_name; \
    static const mxClassID class_id; \
  }; \
  const char *cpp2matlab<CPP>::class_name = MATLAB; \
  const mxClassID cpp2matlab<CPP>::class_id = MATLAB2;

CPP2MATLAB(double, "double", mxDOUBLE_CLASS);
CPP2MATLAB(uint8_t, "uint8", mxUINT8_CLASS);
CPP2MATLAB(int8_t, "int8", mxINT8_CLASS);
CPP2MATLAB(uint16_t, "uint16", mxUINT16_CLASS);
CPP2MATLAB(int16_t, "int16", mxINT16_CLASS);
CPP2MATLAB(uint32_t, "uint32", mxUINT32_CLASS);
CPP2MATLAB(int32_t, "int32", mxINT32_CLASS);
CPP2MATLAB(uint64_t, "uint64", mxUINT64_CLASS);
CPP2MATLAB(int64_t, "int64", mxINT64_CLASS);
CPP2MATLAB(float, "single", mxSINGLE_CLASS);
CPP2MATLAB(bool, "logical", mxSINGLE_CLASS);

#undef CPP2MATLAB

template<int UNUSED>
class mex_ref_ {
public:
  inline mex_ref_()
      : ptr_(NULL) {
  }
  inline mex_ref_(mxArray *ptr)
      : ptr_(ptr) {
  }

  template<typename T>
  bool is_type() {
    return mxIsClass(ptr_, cpp2matlab<T>::class_name);
  }

  mex_ref_& operator=(mxArray *ptr) {
    ptr_ = ptr;
  }

  operator mxArray*() const {
    return ptr_;
  }

protected:
  mxArray *ptr_;
};

template<typename T>
class typed_array : public mex_ref {
public:
  typed_array(const mex_ref &m) 
      : mex_ref(m) {
    if(!is_type<T>()) {
      throw std::runtime_error("invalid typed_array initialization");
    }
  }
  typed_array(std::size_t n_dim, std::size_t *dims,
      bool complex=true) {
    ptr_ = mxCreateNumericArray(n_dim, dims, 
        cpp2matlab<T>::class_id, 
        (complex ? mxCOMPLEX : mxREAL));
  }
  ~typed_array() {
  }

  std::vector<std::size_t> get_dims() const {
    std::vector<std::size_t> dims;
    std::size_t num_dims = mxGetNumberOfDimensions(ptr_);
    dims.resize(num_dims);
    const mwSize *ml_dims = mxGetDimensions(ptr_);
    for(std::size_t i=0; i<num_dims; ++i) dims[i] = ml_dims[i];
    return dims;
  }

  inline T* get_ptr() {
    return reinterpret_cast<T*>(ptr_);
  }
  inline T* real_ptr() {
    return reinterpret_cast<T*>(mxGetData(ptr_));
  }
  inline T* imag_ptr() {
    return reinterpret_cast<T*>(mxGetImagData(ptr_));
  }

  typed_array& operator=(const typed_array &r) {
    ptr_ = r.ptr_;
  }
};

}

void mexFunction(int nlhs, mxArray *lhs[], 
    int nrhs, const mxArray *rhs[]) {
  std::vector<matlab::mex_ref> v_lhs;
  v_lhs.resize(nlhs);
  for(int i=0; i<nlhs; ++i) v_lhs[i] = lhs[i];
  std::vector<matlab::mex_ref> v_rhs;
  v_rhs.resize(nrhs);
  for(int i=0; i<nrhs; ++i) v_rhs[i] = const_cast<mxArray*>(rhs[i]);
  try {
    matlab::mex_implementation(v_rhs, v_lhs);
    for(int i=0; i<nlhs; ++i) lhs[i] = v_lhs[i];
  } catch(const std::exception &str) {
    mexErrMsgTxt(str.what());
  }
}

#endif

