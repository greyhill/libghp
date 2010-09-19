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

template<int UNUSED>
class device_ {
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

