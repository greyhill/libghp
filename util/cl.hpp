#ifndef _GHP_UTIL_CL_HPP_
#define _GHP_UTIL_CL_HPP_

#include <cl.h>

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_array.hpp>

#include <iostream>
#include <list>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace cl {

template<int UNUSED> class platform_;
template<int UNUSED> class device_;
template<int UNUSED> class context_;

/* right now, all parameters return strings */
template<cl_platform_info I> struct cl_platform_info_retval {
  typedef std::string value;
};

template<typename RETVAL> RETVAL platform_get_info_(cl_platform_id id, 
    cl_platform_info param) {
  /* use specializations only */
  assert(false);
}

template<> std::string platform_get_info_<std::string>(cl_platform_id id, 
    cl_platform_info param) {
  std::vector<char> buf;
  int err;
  std::size_t size;
  err = clGetPlatformInfo(id, param, 0, NULL, &size);
  if(err != CL_SUCCESS) throw std::runtime_error("cl error");
  buf.reserve(size);
  err = clGetPlatformInfo(id, param, size, &buf[0], NULL);
  if(err != CL_SUCCESS) throw std::runtime_error("cl error");

  return std::string(&buf[0]);
}

template<int UNUSED>
class platform_ {
public:
  inline platform_(cl_platform_id id) 
      : id_(id) { }
  inline ~platform_() { }

  template<cl_platform_info PARAM>
  typename cl_platform_info_retval<PARAM>::value
  inline get_info() const {
    return platform_get_info_<typename cl_platform_info_retval<PARAM>::value>
      (id_, PARAM);
  }

  template<typename C>
  static void get_platforms(C &c) {
    int err;
    uint32_t num_platforms;
    std::vector<cl_platform_id> platform_ids;
    err = clGetPlatformIDs(0, NULL, &num_platforms);
    if(err != CL_SUCCESS) throw std::runtime_error("cl error");
    platform_ids.reserve(num_platforms);
    err = clGetPlatformIDs(num_platforms, &platform_ids[0], NULL);
    if(err != CL_SUCCESS) throw std::runtime_error("cl error");

    for(int i=0; i<num_platforms; ++i) {
      c.insert(c.end(), platform_(platform_ids[i]));
    }
  }

  template<typename C>
  void get_devices(C &c) {
    int err;
    uint32_t num_devices;
    std::vector<cl_device_id> device_ids;
    err = clGetDeviceIDs(id_, CL_DEVICE_TYPE_ALL, 0, NULL, &num_devices);
    if(err != CL_SUCCESS) throw std::runtime_error("cl error");
    device_ids.reserve(num_devices);
    err = clGetDeviceIDs(id_, CL_DEVICE_TYPE_ALL, num_devices,
        &device_ids[0], NULL);
    if(err != CL_SUCCESS) throw std::runtime_error("cl error");

    for(int i=0; i<num_devices; ++i) {
      c.insert(c.end(), device_<0>(device_ids[i]));
    }
  }

  inline std::string profile() const {
    return get_info<CL_PLATFORM_PROFILE>();
  }
  inline std::string version() const {
    return get_info<CL_PLATFORM_VERSION>();
  }
  inline std::string name() const {
    return get_info<CL_PLATFORM_NAME>();
  }
  inline std::string vendor() const {
    return get_info<CL_PLATFORM_VENDOR>();
  }
  inline std::string extensions() const {
    return get_info<CL_PLATFORM_EXTENSIONS>();
  }
  inline cl_platform_id id() const {
    return id_;
  }

private:
  cl_platform_id id_;
};
typedef platform_<0> platform;

template<cl_device_info I> struct cl_device_info_retval {
};
#define DEVICE_INFO_RETVAL(x,y) \
  template<> struct cl_device_info_retval<x> { \
    typedef y value; \
  };
#include "cl_device_extensions"
#undef DEVICE_INFO_RETVAL

template<typename RETVAL> RETVAL device_get_info_(cl_device_id id,
    cl_device_info param) {
  /* by default, we're just filling a little space in memory.  this
    can be done generically */
  int err;
  RETVAL retval;
  err = clGetDeviceInfo(id, param,
      sizeof(RETVAL), &retval, NULL);
  if(err != CL_SUCCESS) throw std::runtime_error("cl error");
  return retval;
}

template<> std::string device_get_info_<std::string>(cl_device_id id,
    cl_device_info param) {
  int err;
  std::vector<char> buffer;
  std::size_t size;
  err = clGetDeviceInfo(id, param, 0, NULL, &size);
  if(err != CL_SUCCESS) throw std::runtime_error("cl error");
  buffer.reserve(size);
  err = clGetDeviceInfo(id, param, size, &buffer[0], NULL);
  if(err != CL_SUCCESS) throw std::runtime_error("cl error");
  return std::string(&buffer[0]);
}

template<> bool device_get_info_<bool>(cl_device_id id,
    cl_device_info param) {
  return device_get_info_<cl_bool>(id, param) == CL_TRUE;
}

template<> std::vector<std::size_t> 
device_get_info_<std::vector<std::size_t> >(cl_device_id id,
    cl_device_info param) {
  int err;
  std::vector<std::size_t> buffer;
  std::size_t size;
  err = clGetDeviceInfo(id, param, 0, NULL, &size);
  if(err != CL_SUCCESS) throw std::runtime_error("cl error");
  buffer.reserve(size);
  err = clGetDeviceInfo(id, param, size, &buffer[0], NULL);
  if(err != CL_SUCCESS) throw std::runtime_error("cl error");
  return buffer;
}

template<> platform_<0> device_get_info_<platform_<0> >(cl_device_id id,
    cl_device_info param) {
  return platform_<0>(device_get_info_<cl_platform_id>(id, param));
}

template<int UNUSED>
class device_ {
public:
  inline device_(cl_device_id id)
      : id_(id) { }
  inline ~device_() { }

  template<cl_device_info PARAM>
  typename cl_device_info_retval<PARAM>::value
  inline get_info() const {
    return device_get_info_
        <typename cl_device_info_retval<PARAM>::value>
        (id_, PARAM);
  }

  /* ugly preprocessor magic for incredibly repetitive 
    method signatures.  this is generated from
    the file cl_device_extensions with 
    mk_cl_device_extensions_methods.pl */
  #include "cl_device_extensions_methods"

  inline cl_device_id id() const {
    return id_;
  }

private:
  cl_device_id id_;
};
typedef device_<0> device;

template<int UNUSED>
class command_queue_ : boost::noncopyable {
public:
  inline ~command_queue_() {
    clReleaseCommandQueue(id_);
  }

  /* TODO will add profiling support later 
  bool profiling_enabled() const {
  }
  void set_profiling_enabled(bool b) {
  }*/

  cl_command_queue id() const {
    return id_;
  }

private:
  command_queue_(cl_command_queue id)
      : id_(id) {
  }

  cl_command_queue id_;

  friend class context_<0>;
};
typedef command_queue_<0> command_queue;

void context_errfun_(const char *errinfo, const void *pinfo,
    std::size_t cb, void *user_data);

void context_errfun_noop_(const std::string &err, const void *private_info,
    std::size_t private_size) {
  std::cerr << "OpenCL context error: " << err << std::endl;
}

template<int UNUSED>
class context_ : boost::noncopyable {
public:
  typedef boost::function<void(const std::string&, const void*, std::size_t)>
    error_function_type;

  context_(platform p, device d)
      : context_id_(NULL),
      error_function_(context_errfun_noop_) {
    devices_.push_back(d);
    init_(p);
  }

  template<typename ITER1, typename ITER2>
  context_(platform p, ITER1 begin, ITER2 end) 
      : context_id_(NULL),
      error_function_(context_errfun_noop_) {
    cl_context_properties props[] = { CL_CONTEXT_PLATFORM, p.id(), NULL };
    devices_.insert(begin, end);
    init_(p);
  }

  inline ~context_() {
    std::size_t num_devices = devices_.size();
    for(uint32_t i=0; i<num_devices; ++i) {
      delete command_queues_[i];
    }
    clReleaseContext(context_id_);
  }

  inline const error_function_type& error_function() const {
    return error_function_;
  }
  inline error_function_type& error_function() {
    return error_function_;
  }

  inline std::size_t num_devices() const {
    return devices_.size();
  }
  inline device devices(int i) const {
    return devices_[i];
  }
  inline command_queue& command_queues(int i) const {
    return *command_queues_[i];
  }

  inline cl_context id() const {
    return context_id_;
  }
  
private:
  void init_(platform p) {
    /* setup context */
    int err;
    std::size_t num_devices = devices_.size();
    cl_context_properties props[] = { CL_CONTEXT_PLATFORM, 
        reinterpret_cast<cl_context_properties>(p.id()), 
        0 };
    context_id_ = clCreateContext(props,
        num_devices,
        reinterpret_cast<cl_device_id*>(&devices_[0]),
        context_errfun_,
        this,
        &err);
    if(err != CL_SUCCESS) 
      throw std::runtime_error("error creating cl context");

    /* setup command queues */
    for(uint32_t i=0; i<num_devices; ++i) {
      cl_command_queue q;
      q = clCreateCommandQueue(context_id_,
          devices_[i].id(),
          NULL,
          &err);
      if(err != CL_SUCCESS) {
        throw std::runtime_error("error creating cl command queue");
      }
      command_queues_.push_back(new command_queue(q));
    }
  }

  cl_context context_id_;
  std::vector<device> devices_;
  std::vector<command_queue*> command_queues_;
  error_function_type error_function_;
};
typedef context_<0> context;

void context_errfun_(const char *errinfo, const void *pinfo, 
    std::size_t cb, void *user_data) {
  class context_<0> *cptr = reinterpret_cast<class context_<0>*>(user_data);
  cptr->error_function()(std::string(errinfo), pinfo, cb);
}

template<int UNUSED>
class program_ : boost::noncopyable {
public:
  program_(const context &c, const std::string &source,
    const std::string &args = "") 
      : id_(NULL),
      parent_context_(c) {
    init_(source, args);
  }
  ~program_() {
    clReleaseProgram(id_);
  }
 
private:
  inline void init_(const std::string &source,
      const std::string &args) {
    int err;
    const char *tmp = source.c_str();
    id_ = clCreateProgramWithSource(parent_context_.id(),
        1,
        &tmp,
        NULL,
        &err);
    if(err != CL_SUCCESS)
      throw std::runtime_error("error creating cl program!");
    err = clBuildProgram(id_,
        0, // build for all devices
        NULL,
        args.c_str(),
        NULL,
        NULL);
    if(err != CL_SUCCESS) {
      switch(err) {
        case CL_INVALID_PROGRAM:
          throw std::runtime_error("cl: invalid program object");
        case CL_INVALID_VALUE:
          throw std::runtime_error("cl: invalid device config");
        case CL_INVALID_DEVICE:
          throw std::runtime_error("cl: invalid device");
        case CL_INVALID_BUILD_OPTIONS:
          throw std::runtime_error("cl: invalid build options");
        case CL_INVALID_OPERATION:
          throw std::runtime_error("cl: can't build for that device now");
        case CL_COMPILER_NOT_AVAILABLE:
          throw std::runtime_error("cl: compiler not available");
        case CL_BUILD_PROGRAM_FAILURE:
          throw std::runtime_error("cl: build program error");
        case CL_OUT_OF_HOST_MEMORY:
          throw std::runtime_error("cl: out of host memory");
      }
      throw std::runtime_error("error compiling cl program!");
    }

    /* build all kernels */
    uint32_t num_kernels;
    err = clCreateKernelsInProgram(id_,
        0,
        NULL,
        &num_kernels);
  }

  cl_program id_;
  context &parent_context_;
};
typedef program_<0> program;

}

#endif

