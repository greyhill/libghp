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

template<int UNUSED> class platform_;

template<int UNUSED=0> class device_ : boost::noncopyable {
public:
  inline ~device_() { }

  inline cl_device_id device_id() const {
    return id_;
  }

  uint32_t address_bits() const {
  }
  bool available() const {
  }
  bool compiler_available() const {
  }
  bool little_endian() const {
  }
  const std::set<std::string>& extensions() const {
  }
  uint64_t global_mem_cache_size() const {
  }
  uint64_t global_mem_cacheline_size() const {
  }
  uint64_t global_mem_size() const {
  }
  uint64_t local_mem_size() const {
  }
  uint32_t max_compute_units() const {
  }
  uint32_t max_constant_args() const {
  }
  uint64_t max_constant_buffer_size() const {
  }
  uint64_t max_mem_alloc_size() const {
  }
  std::size_t max_parameter_size() const {
  }
  std::size_t max_work_group_size() const {
  }
  uint32_t max_work_item_dimensions() const {
  }
  boost::shared_array<std::size_t> max_work_item_sizes() const {
  }
  std::string name() const {
  }
  const platform_<UNUSED>& platform() const {
  }
  std::string profile() const {
  }
  bool is_gpu() const {
  }
  bool is_cpu() const {
  }
  std::string vendor() const {
  }
  uint32_t vendor_id() const {
  }
  std::string device_version() const {
  }
  std::string driver_version() const {
  }

private:
  device_(cl_device_id id, platform_<UNUSED> *plat) 
      : id_(id), parent_platform_(plat) {
  }

  cl_device_id id_;
  platform_<UNUSED> *parent_platform_;

  friend class platform_<UNUSED>;
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

