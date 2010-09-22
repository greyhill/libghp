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

template<int UNUSED> class platform_ref_;
template<int UNUSED> class device_ref_;
template<int UNUSED> class context_ref_;
template<int UNUSED> class event_ref_;
template<int UNUSED> class command_queue_ref_;
template<typename T> class buffer_ref;
template<int UNUSED> class program_ref_;
template<int UNUSED> class kernel_ref_;

typedef platform_ref_<0> platform_ref;
typedef device_ref_<0> device_ref;
typedef context_ref_<0> context_ref;
typedef event_ref_<0> event_ref;
typedef command_queue_ref_<0> command_queue_ref;
typedef program_ref_<0> program_ref;
typedef kernel_ref_<0> kernel_ref;

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
class platform_ref_ {
public:
  inline platform_ref_(cl_platform_id id) 
      : id_(id) { }
  inline ~platform_ref_() { }

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
      c.insert(c.end(), platform_ref_(platform_ids[i]));
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
      c.insert(c.end(), device_ref_<0>(device_ids[i]));
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

template<> platform_ref_<0> device_get_info_<platform_ref_<0> >(cl_device_id id,
    cl_device_info param) {
  return platform_ref_<0>(device_get_info_<cl_platform_id>(id, param));
}

template<int UNUSED>
class device_ref_ {
public:
  inline device_ref_(cl_device_id id)
      : id_(id) { }
  inline ~device_ref_() { }

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

template<int UNUSED>
class context_ref_ {
public:
  inline context_ref_(cl_context c)
      : id_(c) {
  }
  inline context_ref_(const context_ref &c)
      : id_(c.id_) {
    clRetainContext(c.id_);
  }
  inline ~context_ref_() {
    clReleaseContext(c.id_);
  }

  inline context_ref_& operator=(const context_ref_ &r) {
    if(r.id_ != id_) {
      clReleaseContext(id_);
      id_ = r.id_;
      clRetainContext(id_);
    }
    return *this;
  }
  inline bool operator==(const context_ref_ &r) const {
    return id_ == r.id_;
  }

private:
  cl_context id_;
};

template<int UNUSED>
class command_queue_ref_ {
public:
  inline command_queue_ref_(cl_command_queue q) 
      : id_(q) {
  }
  inline command_queue_ref_(const command_queue_ref_ &r) 
      : id_(r.id_) {
    clRetainCommandQueue(id_);
  }
  inline command_queue_ref_(context_ref_ context,
    device_ref device,
    bool out_of_order = false,
    bool profiling = false)
      : id_(NULL) {
    int err;
    id_ = clCreateCommandQueue(context.id(), device.id(),
          (out_of_order ? CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE : 0) |
          (profiling ? CL_QUEUE_PROFILING_ENABLE : 0)
        &err);
    if(err != CL_SUCCESS) {
      throw std::runtime_error("error creating command queue");
    }
  }
  inline ~command_queue_ref_() {
    clReleaseCommandQueue(id_);
  }

  inline command_queue_ref_& operator=(const command_queue_ref_ &q) {
    if(id_ != q.id_) {
      clReleaseCommandQueue(id_);
      id_ = q.id_;
      clRetainCommandQueue(id_);
    }
    return *this;
  }
  inline bool operator=(const command_queue_ref_ &q) {
    return id_ == q.id_;
  }

private:
  cl_command_queue id_;
};

template<int UNUSED>
class event_ref_ {
};

template<typename T>
class buffer_ref {
public:
  buffer_ref(context_ref context,
      std::size_t num_elements,
      bool kernel_can_read = true,
      bool kernel_can_write = true,
      bool mappable = false)
      : id_(NULL) {
    int err;
    id_ = clCreateBuffer(context.id(),
          ((kernel_can_read && kernel_can_write) ? CL_MEM_READ_WRITE : 0) |
          ((!kernel_can_read && kernel_can_write) ? CL_MEM_WRITE_ONLY : 0 ) |
          ((kernel_can_read && !kernel_can_write) ? CL_MEM_READ_ONLY : 0) |
          (mappable ? CL_MEM_ALLOC_HOST_PTR : 0),
        sizeof(T) * num_elements,
        NULL,
        &err);
    if(err != CL_SUCCESS) {
      throw std::runtime_error("error creating cl buffer");
    }
  }
  buffer_ref(context_ref context,
      T *buffer,
      std::size_t num_elements,
      bool kernel_can_read = true,
      bool kernel_can_write = true,
      bool mappable = false)
      : id_(NULL) {
    int err;
    id_ = clCreateBuffer(context.id(),
          ((kernel_can_read && kernel_can_write) ? CL_MEM_READ_WRITE : 0) |
          ((!kernel_can_read && kernel_can_write) ? CL_MEM_WRITE_ONLY : 0) |
          ((kernel_can_read && !kernel_can_write) ? CL_MEM_READ_ONLY : 0) |
          CL_MEM_COPY_HOST_PTR |
          (mappable ? CL_MEM_ALLOC_HOST_PTR : 0),
        sizeof(T) * num_elements,
        reinterpret_cast<void*>(buffer),
        &err);
    if(err != CL_SUCCESS) {
      throw std::runtime_error("error creating cl buffer");
    }
  }
  template<typename T2>
  inline buffer_ref(const buffer_ref<T2> &r)
      : id_(r.id_) {
    clRetainMemObject(id_);
  }
  ~buffer_ref() {
  }

  template<typename T2>
  bool operator==(const buffer_ref<T2> &r) const {
    return id_ != r.id_;
  }
  template<typename T2>
  buffer_ref<T>& operator=(const buffer_ref<T2> &r) {
    if(id_ != r.id_) {
      clReleaseMemObject(id_);
      id_ = r.id_;
      clRetainMemObject(id_);
    }
    return *this;
  }

private:
  cl_mem id_;
};

template<int UNUSED>
class program_ref_ {
public:
  inline program_ref_(cl_program p)
      : id_(p) {
  }
  inline program_ref_(const program_ref_ &r)
      : id_(r.id_) {
    clRetainProgram(r.id_);
  }
  inline program_ref_(context_ref context,
      const std::string &source)
      : id_(NULL) {
    int err;
    const char *tmp = source.c_str();
    id_ = clCreateProgramWithSource(context.id(),
        1, &tmp,
        NULL, &err);
    if(err != CL_SUCCESS) {
      throw std::runtime_error("error building CL program from source");
    }
  }
  /* TODO: binary programs */
  inline ~program_ref_() {
    clReleaseProgram(r.id_);
  }

  inline void build(const std::string &compiler_opts) {
    err = clBuildProgram(id_,
        0, NULL,
        compiler_opts.c_str(),
        NULL, NULL);
    if(err != CL_SUCCESS) {
      throw std::runtime_error("error compiling CL program for all devices"
          " in context");
    }
  }

  template<typename ITER1, typename ITER2>
  inline void build_for(const std::string &compiler_opts,
      ITER1 begin, ITER2 end) {
    std::vector<device_ref> devices;
    devices.insert(begin, end);
    int err = clBuildProgram(id_,
        devices.size(), reinterpret_case<cl_device_id*>(&devices[0]),
        compiler_opts.c_str(),
        NULL, NULL);
    if(err != CL_SUCCESS) {
      throw std::runtime_error("error building CL program for specific "
          "devices");
    }
  }

  kernel_ref get_kernel(const std::string &kernel_name) {
    int err;
    cl_kernel kernel_id = clCreateKernel(id_, kernel_name.c_str(), &err);
    if(err != CL_SUCCESS) {
      throw std::runtime_error("error creating kernel from program.  "
          "has it been built?");
    }
    return kernel_ref(kernel_id);
  }

  inline program_ref_& operator=(const program_ref_ &p) {
    if(id_ != p.id_) {
      clReleaseProgram(id_);
      id_ = p.id_;
      clRetainProgram(id_);
    }
    return *this;
  }
  inline bool operator==(const program_ref_ &p) const {
    return id_ == p.id_;
  }

private:
  cl_program id_;
};

template<cl_kernel_work_group_info I>
struct cl_kernel_workgroup_info_retval { };

template<>
struct cl_kernel_workgroup_info_retval<CL_KERNEL_WORK_GROUP_SIZE> {
  typedef std::size_t value;
};

template<>
struct cl_kernel_workgroup_info_retval<CL_KERNEL_COMPILE_WORK_GROUP_SIZE> {
  typedef std::vector<std::size_t> value;
};

template<>
struct cl_kernel_workgroup_info_retval<CL_KERNEL_LOCAL_MEM_SIZE> {
  typedef uint64_t value;
};

template<typename RETVAL> RETVAL kernel_get_workgroup_info_(
    cl_kernel kernel_id,
    cl_device device_id,
    cl_kernel_work_group_info param) {
  RETVAL retval;
  int err;
  err = clGetKernelWorkGroupInfo(
      kernel_id,
      device_id,
      param,
      sizeof(RETVAL),
      &retval,
      NULL);
  if(err != CL_SUCCESS) {
    throw std::runtime_error("cl error in clGetKernelWorkGroupInfo");
  }
  return retval;
}

template<> std::vector<std::size_t>
kernel_get_workgroup_info_<std::vector<std::size_t> >(
    cl_kernel kernel_id,
    cl_device device_id,
    cl_kernel_work_group_info param) {
  std::vector<std::size_t> buffer;
  buffer.reserve(3);
  int err;
  err = clGetKernelWorkGroupInfo(
      kernel_id,
      device_id,
      param,
      sizeof(std::size_t)*3,
      &buffer[0],
      NULL);
  if(err != CL_SUCCESS) {
    throw std::runtime_error("cl error in clGetKernelWorkGroupInfo");
  }
  return buffer;
}

template<int UNUSED>
class kernel_ref_ {
public:
  inline kernel_ref_(cl_kernel id)
      : id_(id) {
  }
  inline kernel_ref_(const kernel_ref_ &k)
      : id_(k.id_) {
    clRetainKernel(id_);
  }
  inline ~kernel_ref_() { }

  inline kernel_ref_& operator=(const kernel_ref_ &k) {
    if(id_ != k.id_) {
      clReleaseKernel(id_);
      id_ = k.id_;
      clRetainKernel(id_);
    }
    return *this;
  }
  inline bool operator==(const kernel_ref_ &k) const {
    return id_ == k.id_;
  }

  template<cl_kernel_work_group_info PARAM>
  typename cl_kernel_workgroup_info_retval<PARAM>::value
  inline get_info(const device &d) const {
    return kernel_get_workgroup_info_
        <typename cl_kernel_workgrou_info_retval<PARAM>::value>
        (id_,
         d.id(),
         PARAM);
  }

private:


  cl_kernel id_;
};

}

#endif

