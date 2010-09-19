#ifndef _GHP_UTIL_CL_HPP_
#define _GHP_UTIL_CL_HPP_

#include <cl.h>

#include <boost/noncopyable.hpp>
#include <boost/shared_array.hpp>

#include <list>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace cl {

enum float_config {
  /** denorms are supported */
  DENORM = CL_FP_DENORM,
  /** INF and NaNs are supported */ 
  INF_NAN = CL_FP_INF_NAN,
  /** round to zero rounding mode supported */
  ROUND_TO_ZERO = CL_FP_ROUND_TO_NEAREST,
  /** round to +ve and -ve infinity rounding modes supported */
  ROUND_TO_INF = CL_FP_ROUND_TO_INF,
  /** IEEE745-2008 fused multiply-add is supported */
  FMA = CL_FP_FMA
};

enum cache_type {
  NONE = CL_NONE,
  READ_ONLY_CACHE = CL_READ_ONLY_CACHE,
  READ_WRITE_CACHE = CL_READ_WRITE_CACHE
};

template<int UNUSED=0> class device_ {
public:
  inline ~device_() { }

  uint32_t address_bits() const {
  }
  bool available() const {
  }
  bool compiler_available() const {
  }
  float_config fp_config() const {
  }
  bool little_endian() const {
  }
  const std::set<std::string>& extensions() const {
  }
  uint64_t global_mem_cache_size() const {
  }
  cache_type global_mem_cache_type() const {
  }
  uint64_t global_mem_cacheline_size() const {
  }
  uint64_t global_mem_size() const {
  }

private:
  device_(cl_device_id id) 
      : id_(id) {
  }

  friend class plaform_<UNUSED>;
  cl_device_id id_;
};
typedef device_<0> device;

template<int UNUSED=0> class platform_ : boost::noncopyable {
public:
  static const std::list<const platform_<0>* >& get_platforms() {
    static bool loaded = false;
    if(!loaded) {
      loaded = true;
      load_platforms_();
    }
    return platforms_;
  }

  inline const std::string& profile() const {
    return profile_;
  }
  inline const std::string& version() const {
    return version_;
  }
  inline const std::string& name() const {
    return name_;
  }
  inline const std::string& vendor() const {
    return vendor_;
  }
  inline const std::set<std::string>& extensions() const {
    return extensions_;
  }
 
private:
  static inline void load_platforms_() {
    cl_uint num_platforms;
    boost::shared_array<cl_platform_id> platforms;
    int err;

    err = clGetPlatformIDs(0, NULL, &num_platforms);
    if(err != CL_SUCCESS) {
      if(err == CL_INVALID_PLATFORM) {
        throw std::runtime_error("invalid platform");
      } else if(err == CL_INVALID_VALUE) {
        throw std::runtime_error("invalid value");
      }
      throw std::runtime_error("error in clGetPlatformIDs");
    }

    platforms.reset(new cl_platform_id[num_platforms]);

    err = clGetPlatformIDs(num_platforms, platforms.get(), NULL);
    if(err != CL_SUCCESS) {
      if(err == CL_INVALID_PLATFORM) {
        throw std::runtime_error("invalid platform");
      } else if(err == CL_INVALID_VALUE) {
        throw std::runtime_error("invalid value");
      }
      throw std::runtime_error("error in clGetPlatformIDs");
    }

    for(int i=0; i<num_platforms; ++i) {
      platforms_.push_back(new platform_<0>(platforms[i]));
    }
  }

  platform_(cl_platform_id id) {
    init_(id);
  }

  inline void init_(cl_platform_id id) {
    load_info_(id, CL_PLATFORM_PROFILE, profile_);
    load_info_(id, CL_PLATFORM_VERSION, version_);
    load_info_(id, CL_PLATFORM_VENDOR, vendor_);
    load_info_(id, CL_PLATFORM_NAME, name_);

    std::string tmp;
    std::stringstream sstream;
    load_info_(id, CL_PLATFORM_EXTENSIONS, tmp);
    sstream.str(tmp);

    while(sstream >> tmp) {
      extensions_.insert(tmp);
    }
  }

  void load_info_(cl_platform_id id, cl_platform_info param, 
      std::string &out) {
    int error;
    std::size_t size;
    std::vector<char> buffer;

    error = clGetPlatformInfo(id, param, 0, NULL, &size);
    if(error != CL_SUCCESS) throw std::runtime_error("error in platform ctor");
    buffer.reserve(size);
    error = clGetPlatformInfo(id, param, size, &buffer[0], 0);
    if(error != CL_SUCCESS) throw std::runtime_error("error in platform ctor");
    out = &buffer[0];
  }

  static std::list<const platform_<0>*> platforms_;

  std::string profile_;
  std::string version_;
  std::string name_;
  std::string vendor_;
  std::set<std::string> extensions_;
};
template<int UNUSED> std::list<const platform_<0>*> platform_<UNUSED>::platforms_;
typedef platform_<0> platform;

template<int UNUSED=0> class context_ {
public:
  context_(const platform &p, const device &d);
  template<typename ITER>
  context_(const platform &p, 
      ITER devices_begin,
      ITER devices_end);
  template<typename CONTAINER>
  context_(const platform &p, const CONTAINER &devices);
};
typedef context_<0> context;

template<typename T, typename PTR_T=T*> class buffer {
};

}

#endif

