#ifndef _GHP_UTIL_CL_HPP_
#define _GHP_UTIL_CL_HPP_

#include <cl.h>

#include <boost/noncopyable.hpp>
#include <boost/shared_array.hpp>

#include <cstdarg>
#include <list>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace cl {

template<int UNUSED> class platform_;
template<int UNUSED> class device_;

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

private:
  cl_device_id id_;
};
typedef device_<0> device;

template<int UNUSED>
class context_ : boost::noncopyable {
public:
  context_(platform p, device d1, ...) { }
  ~context_();

private:
  platform platform_;
  std::vector<device> devices_;
};
typedef context_<0> context;

}

#endif

