#ifndef _GHP_UTIL_CL_HPP_
#define _GHP_UTIL_CL_HPP_

// OpenCL / OpenGL interop wrapper
#include "../gfx/gl.hpp"

#include <cl.h>
#include <cl_gl.h>

// #include "../gfx.hpp"
#include "../math.hpp"

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

template<int UNUSED> class cl_error_;
template<int UNUSED> class platform_ref_;
template<int UNUSED> class device_ref_;
template<int UNUSED> class context_ref_;
template<int UNUSED> class event_ref_;
template<int UNUSED> class command_queue_ref_;
template<int UNUSED> class buffer_ref_base_;
template<int UNUSED> class buffer_ref_;
template<int UNUSED> class image2d_ref_;
template<int UNUSED> class program_ref_;
template<int UNUSED> class kernel_ref_;

typedef cl_error_<0> cl_error;
typedef platform_ref_<0> platform_ref;
typedef device_ref_<0> device_ref;
typedef context_ref_<0> context_ref;
typedef event_ref_<0> event_ref;
typedef command_queue_ref_<0> command_queue_ref;
typedef buffer_ref_base_<0> buffer_ref_base;
typedef buffer_ref_<0> buffer_ref;
typedef image2d_ref_<0> image2d_ref;
typedef program_ref_<0> program_ref;
typedef kernel_ref_<0> kernel_ref;

namespace {
const char* cl_err_string(cl_int err) {
  switch (err) {
    case CL_SUCCESS:                          
      return "Success!";
    case CL_DEVICE_NOT_FOUND:                 
      return "Device not found.";
    case CL_DEVICE_NOT_AVAILABLE:             
      return "Device not available";
    case CL_COMPILER_NOT_AVAILABLE:           
      return "Compiler not available";
    case CL_MEM_OBJECT_ALLOCATION_FAILURE:    
      return "Memory object allocation failure";
    case CL_OUT_OF_RESOURCES:                 
      return "Out of resources";
    case CL_OUT_OF_HOST_MEMORY:               
      return "Out of host memory";
    case CL_PROFILING_INFO_NOT_AVAILABLE:     
      return "Profiling information not available";
    case CL_MEM_COPY_OVERLAP:                 
      return "Memory copy overlap";
    case CL_IMAGE_FORMAT_MISMATCH:            
      return "Image format mismatch";
    case CL_IMAGE_FORMAT_NOT_SUPPORTED:       
      return "Image format not supported";
    case CL_BUILD_PROGRAM_FAILURE:            
      return "Program build failure";
    case CL_MAP_FAILURE:                      
      return "Map failure";
    case CL_INVALID_VALUE:                    
      return "Invalid value";
    case CL_INVALID_DEVICE_TYPE:              
      return "Invalid device type";
    case CL_INVALID_PLATFORM:                 
      return "Invalid platform";
    case CL_INVALID_DEVICE:                   
      return "Invalid device";
    case CL_INVALID_CONTEXT:                  
      return "Invalid context";
    case CL_INVALID_QUEUE_PROPERTIES:         
      return "Invalid queue properties";
    case CL_INVALID_COMMAND_QUEUE:            
      return "Invalid command queue";
    case CL_INVALID_HOST_PTR:                 
      return "Invalid host pointer";
    case CL_INVALID_MEM_OBJECT:               
      return "Invalid memory object";
    case CL_INVALID_IMAGE_FORMAT_DESCRIPTOR:  
      return "Invalid image format descriptor";
    case CL_INVALID_IMAGE_SIZE:               
      return "Invalid image size";
    case CL_INVALID_SAMPLER:                  
      return "Invalid sampler";
    case CL_INVALID_BINARY:                   
      return "Invalid binary";
    case CL_INVALID_BUILD_OPTIONS:            
      return "Invalid build options";
    case CL_INVALID_PROGRAM:                  
      return "Invalid program";
    case CL_INVALID_PROGRAM_EXECUTABLE:       
      return "Invalid program executable";
    case CL_INVALID_KERNEL_NAME:              
      return "Invalid kernel name";
    case CL_INVALID_KERNEL_DEFINITION:        
      return "Invalid kernel definition";
    case CL_INVALID_KERNEL:                   
      return "Invalid kernel";
    case CL_INVALID_ARG_INDEX:                
      return "Invalid argument index";
    case CL_INVALID_ARG_VALUE:                
      return "Invalid argument value";
    case CL_INVALID_ARG_SIZE:                 
      return "Invalid argument size";
    case CL_INVALID_KERNEL_ARGS:              
      return "Invalid kernel arguments";
    case CL_INVALID_WORK_DIMENSION:           
      return "Invalid work dimension";
    case CL_INVALID_WORK_GROUP_SIZE:          
      return "Invalid work group size";
    case CL_INVALID_WORK_ITEM_SIZE:           
      return "Invalid work item size";
    case CL_INVALID_GLOBAL_OFFSET:            
      return "Invalid global offset";
    case CL_INVALID_EVENT_WAIT_LIST:          
      return "Invalid event wait list";
    case CL_INVALID_EVENT:                    
      return "Invalid event";
    case CL_INVALID_OPERATION:                
      return "Invalid operation";
    case CL_INVALID_GL_OBJECT:                
      return "Invalid OpenGL object";
    case CL_INVALID_BUFFER_SIZE:              
      return "Invalid buffer size";
    case CL_INVALID_MIP_LEVEL:                
      return "Invalid mip-map level";
    default:                                  
      return "Unknown";
  }
}
}

template<typename T> struct cpp2cl { };
template<> struct cpp2cl<int8_t> {
  static const cl_channel_type value = CL_SIGNED_INT8;
};
template<> struct cpp2cl<uint8_t> {
  static const cl_channel_type value = CL_UNSIGNED_INT8;
};
template<> struct cpp2cl<int16_t> {
  static const cl_channel_type value = CL_SIGNED_INT16;
};
template<> struct cpp2cl<uint16_t> {
  static const cl_channel_type value = CL_UNSIGNED_INT16;
};
template<> struct cpp2cl<int32_t> {
  static const cl_channel_type value = CL_SIGNED_INT32;
};
template<> struct cpp2cl<uint32_t> {
  static const cl_channel_type value = CL_UNSIGNED_INT32;
};
template<> struct cpp2cl<float> {
  static const cl_channel_type value = CL_FLOAT;
};

template<typename PIXELT> struct pixelt2cl { };

template<typename T> 
struct pixelt2cl<ghp::Single<0, ghp::RGBA<T> > > {
  static const cl_channel_order channel_order = CL_R;
  static const cl_channel_type channel_type = cpp2cl<T>::value;
};
template<typename T>
struct pixelt2cl<ghp::Single<3, ghp::RGBA<T> > > {
  static const cl_channel_order channel_order = CL_A;
  static const cl_channel_type channel_type = cpp2cl<T>::value;
};
template<typename T>
struct pixelt2cl<ghp::Single<0, ghp::RGB<T> > > { 
  static const cl_channel_order channel_order = CL_R;
  static const cl_channel_type channel_type = cpp2cl<T>::value;
};
template<typename T>
struct pixelt2cl<ghp::RGBA<T> > { 
  static const cl_channel_order channel_order = CL_RGBA;
  static const cl_channel_type channel_type = cpp2cl<T>::value;
};

template<int UNUSED>
class cl_error_ : public std::runtime_error {
public:
  cl_error_(int err_code, const std::string &msg) 
      : std::runtime_error(msg + ": " + cl_err_string(err_code)),
      err_code_(err_code),
      msg_(msg) {
  }
  ~cl_error_() throw() { }

private:
  int err_code_;
  std::string msg_;
};

/* right now, all parameters return strings */
template<cl_platform_info I> struct cl_platform_info_retval {
  typedef std::string value;
};

namespace {
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
  if(err != CL_SUCCESS) throw cl_error(err, "cl error");
  buf.reserve(size);
  err = clGetPlatformInfo(id, param, size, &buf[0], NULL);
  if(err != CL_SUCCESS) throw cl_error(err, "cl error");

  return std::string(&buf[0]);
}
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
    if(err != CL_SUCCESS) throw cl_error(err, "cl error");
    platform_ids.reserve(num_platforms);
    err = clGetPlatformIDs(num_platforms, &platform_ids[0], NULL);
    if(err != CL_SUCCESS) throw cl_error(err, "cl error");

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
    if(err != CL_SUCCESS) throw cl_error(err, "cl error");
    device_ids.reserve(num_devices);
    err = clGetDeviceIDs(id_, CL_DEVICE_TYPE_ALL, num_devices,
        &device_ids[0], NULL);
    if(err != CL_SUCCESS) throw cl_error(err, "cl error");

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

namespace {
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
  if(err != CL_SUCCESS) throw cl_error(err, "cl error");
  return retval;
}

template<> std::string device_get_info_<std::string>(cl_device_id id,
    cl_device_info param) {
  int err;
  std::vector<char> buffer;
  std::size_t size;
  err = clGetDeviceInfo(id, param, 0, NULL, &size);
  if(err != CL_SUCCESS) throw cl_error(err, "cl error");
  buffer.reserve(size);
  err = clGetDeviceInfo(id, param, size, &buffer[0], NULL);
  if(err != CL_SUCCESS) throw cl_error(err, "cl error");
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
  if(err != CL_SUCCESS) throw cl_error(err, "cl error");
  buffer.reserve(size);
  err = clGetDeviceInfo(id, param, size, &buffer[0], NULL);
  if(err != CL_SUCCESS) throw cl_error(err, "cl error");
  return buffer;
}

template<> platform_ref_<0> device_get_info_<platform_ref_<0> >(cl_device_id id,
    cl_device_info param) {
  return platform_ref_<0>(device_get_info_<cl_platform_id>(id, param));
}
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
  inline context_ref_(platform_ref platform, device_ref dev) 
      : id_(NULL) {
    int err;
    cl_device_id device_id = dev.id();
    cl_context_properties context_props[] = 
        { CL_CONTEXT_PLATFORM, 
        reinterpret_cast<cl_context_properties>(platform.id()), 
        NULL };
    id_ = clCreateContext(context_props,
        1,
        &device_id,
        NULL,
        NULL,
        &err);
    if(err != CL_SUCCESS) {
      throw cl_error(err, "error creating CL context");
    }
  }
  template<typename ITER1, typename ITER2>
  inline context_ref_(platform_ref platform, ITER1 device_begin,
      ITER2 device_end)
      : id_(NULL) {
    int err;
    std::vector<device_ref> devices;
    devices.insert(devices.begin(), device_begin, device_end);
    cl_context_properties context_props[] =
        { CL_CONTEXT_PLATFORM,
        reinterpret_cast<cl_context_properties>(platform.id()),
        NULL };
    id_ = clCreateContext(context_props,
        devices.size(),
        reinterpret_cast<cl_device_id*>(&devices[0]),
        NULL,
        NULL,
        &err);
    if(err != CL_SUCCESS) {
      throw cl_error(err, "error creating CL context");
    }
  }
  inline ~context_ref_() {
#ifdef CLREFCOUNT
    uint32_t refcount;
    clGetContextInfo(id_,
        CL_CONTEXT_REFERENCE_COUNT,
        sizeof(refcount),
        &refcount,
        NULL);
    std::cerr << "context_ref dtor.  refcount: " 
        << (refcount-1) << std::endl;
#endif
    clReleaseContext(id_);
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

  inline cl_context id() const {
    return id_;
  }

private:
  cl_context id_;
};

template<int UNUSED>
class event_ref_ {
public:
  inline event_ref_(cl_event id)
      : id_(id) {
  }
  inline event_ref_(const event_ref &r)
      : id_(r.id_) {
    clRetainEvent(id_);
  }
  inline ~event_ref_() {
#ifdef CLREFCOUNT
    uint32_t refcount;
    clGetEventInfo(id_,
        CL_EVENT_REFERENCE_COUNT,
        sizeof(refcount),
        &refcount,
        NULL);
    std::cerr << "event_ref dtor; refcount: " << 
       (refcount-1) << std::endl;
#endif
    clReleaseEvent(id_);
  }

  event_ref_& operator=(const event_ref_ &r) {
    if(id_ != r.id_) {
      clReleaseEvent(id_);
      id_ = r.id_;
      clRetainEvent(id_);
    }
    return *this;
  }
  bool operator==(const event_ref_ &r) {
    return id_ == r.id_;
  }

  void wait() {
    int err = clWaitForEvents(1, &id_);
    if(err != CL_SUCCESS) {
      throw cl_error(err, "error waiting for event!");
    }
  }

  template<typename ITER1, typename ITER2>
  static void wait_for(ITER1 begin, ITER2 end) {
    std::vector<event_ref> events;
    events.insert(begin, end);
    int err = clWaitForEvents(events.size(), 
      reinterpret_cast<cl_event*>(&events[0]));
    if(err != CL_SUCCESS) {
      throw cl_error(err, "error waiting for event!");
    }
  }
private:
  cl_event id_;
};

template<int UNUSED>
class buffer_ref_base_ {
public:
  inline buffer_ref_base_()
      : id_(NULL) {
  }
  inline buffer_ref_base_(const buffer_ref_base_ &r)
      : id_(r.id_) {
    clRetainMemObject(id_);
  }

  inline ~buffer_ref_base_() {
    clReleaseMemObject(id_);
  }

  inline bool operator==(const buffer_ref_base_ &r) const {
    return id_ == r.id_;
  }
  inline buffer_ref_base_& operator=(const buffer_ref_base_ &r) {
    if(id_ != r.id_) {
      clReleaseMemObject(id_);
      id_ = r.id_;
      clRetainMemObject(id_);
    }
    return *this;
  }

  inline cl_mem id() const {
    return id_;
  }

protected:
  cl_mem id_;
};

template<int UNUSED>
class buffer_ref_ : public buffer_ref_base {
public:
  buffer_ref_(context_ref context,
      std::size_t size,
      bool kernel_can_read = true,
      bool kernel_can_write = true,
      bool mappable = false) {
    int err;
    id_ = clCreateBuffer(context.id(),
          ((kernel_can_read && kernel_can_write) ? CL_MEM_READ_WRITE : 0) |
          ((!kernel_can_read && kernel_can_write) ? CL_MEM_WRITE_ONLY : 0 ) |
          ((kernel_can_read && !kernel_can_write) ? CL_MEM_READ_ONLY : 0) |
          (mappable ? CL_MEM_ALLOC_HOST_PTR : 0),
        size,
        NULL,
        &err);
    if(err != CL_SUCCESS) {
      throw cl_error(err, "error creating cl buffer");
    }
  }
  template<typename T>
  buffer_ref_(context_ref context,
      T *buffer,
      std::size_t size,
      bool kernel_can_read = true,
      bool kernel_can_write = true,
      bool mappable = false) {
    int err;
    id_ = clCreateBuffer(context.id(),
          ((kernel_can_read && kernel_can_write) ? CL_MEM_READ_WRITE : 0) |
          ((!kernel_can_read && kernel_can_write) ? CL_MEM_WRITE_ONLY : 0) |
          ((kernel_can_read && !kernel_can_write) ? CL_MEM_READ_ONLY : 0) |
          CL_MEM_COPY_HOST_PTR |
          (mappable ? CL_MEM_ALLOC_HOST_PTR : 0),
        size,
        reinterpret_cast<void*>(buffer),
        &err);
    if(err != CL_SUCCESS) {
      throw cl_error(err, "error creating cl buffer");
    }
  }

  inline buffer_ref_(const buffer_ref_ &r) {
    id_ = r.id_;
    clRetainMemObject(id_);
  }
};

template<int UNUSED>
class image2d_ref_ : public buffer_ref_base {
public:
  template<typename PIXELT>
  image2d_ref_(context_ref context,
      const PIXELT &pixelt,
      std::size_t width,
      std::size_t height,
      bool kernel_can_read = true,
      bool kernel_can_write = true,
      bool mappable = false) {
    int err;
    cl_image_format format = {
      pixelt2cl<PIXELT>::channel_order,
      pixelt2cl<PIXELT>::channel_type
    };
    id_ = clCreateImage2D(context.id(),
        ((kernel_can_read && kernel_can_write) ? CL_MEM_READ_WRITE : 0 ) |
        ((!kernel_can_read && kernel_can_write) ? CL_MEM_WRITE_ONLY : 0 ) |
        ((kernel_can_read && !kernel_can_write) ? CL_MEM_READ_ONLY : 0 ) | 
        (mappable ? CL_MEM_ALLOC_HOST_PTR : 0),
        &format,
        width,
        height,
        0,
        NULL,
        &err);
    if(err != CL_SUCCESS) {
      throw cl_error(err, "error creating cl image2d");
    }
  }
  template<typename PIXELT>
  image2d_ref_(context_ref context,
      const ghp::texture<PIXELT> &texture,
      bool kernel_can_read = true,
      bool kernel_can_write = true,
      bool mappable = false) {
    int err;
    cl_image_format format = {
        pixelt2cl<PIXELT>::channel_order,
        pixelt2cl<PIXELT>::channel_type
    };
    id_ = clCreateImage2D(context.id(),
        ((kernel_can_read && kernel_can_write) ? CL_MEM_READ_WRITE : 0 ) |
        ((!kernel_can_read && kernel_can_write) ? CL_MEM_WRITE_ONLY : 0) |
        ((kernel_can_read && !kernel_can_write) ? CL_MEM_READ_ONLY : 0) |
        (mappable ? CL_MEM_ALLOC_HOST_PTR : 0),
        &format,
        texture.width(),
        texture.height(),
        0,
        reinterpret_cast<void*>(texture[0]),
        &err);
    if(err != CL_SUCCESS) {
      throw cl_error(err, "error creating cl image2d");
    }
  }
  template<typename PIXELT>
  image2d_ref_(context_ref context,
      const PIXELT &pixelt,
      void *v,
      std::size_t width,
      std::size_t height,
      bool kernel_can_read = true,
      bool kernel_can_write = true,
      bool mappable = false) {
    int err;
    cl_image_format format = {
        pixelt2cl<PIXELT>::channel_order,
        pixelt2cl<PIXELT>::channel_type
    };
    id_ = clCreateImage2D(context.id(),
        ((kernel_can_read && kernel_can_write) ? CL_MEM_READ_WRITE : 0 ) |
        ((!kernel_can_read && kernel_can_write) ? CL_MEM_WRITE_ONLY : 0) |
        ((kernel_can_read && !kernel_can_write) ? CL_MEM_READ_ONLY : 0) |
        (mappable ? CL_MEM_ALLOC_HOST_PTR : 0),
        &format,
        width,
        height,
        0,
        reinterpret_cast<void*>(v),
        &err);
    if(err != CL_SUCCESS) {
      throw cl_error(err, "error creating cl image2d");
    }
  }
  template<typename PIXELT>
  image2d_ref_(context_ref context,
      const gl::texture<2, PIXELT> &gl_tex,
      bool kernel_can_read = true,
      bool kernel_can_write = true,
      bool mappable = false) {
    int err;
    id_ = clCreateFromGLTexture2D(context.id(),
        ((kernel_can_read && kernel_can_write) ? CL_MEM_READ_WRITE : 0 ) |
        ((!kernel_can_read && kernel_can_write) ? CL_MEM_WRITE_ONLY : 0) |
        ((kernel_can_read && !kernel_can_write) ? CL_MEM_READ_ONLY : 0) |
        (mappable ? CL_MEM_ALLOC_HOST_PTR : 0),
        GL_TEXTURE_2D,
        0,
        gl_tex.id(),
        &err);
    if(err != CL_SUCCESS) {
      throw cl_error(err, "error creating cl image2d");
    }
  }

  inline image2d_ref_(const image2d_ref_ &r) {
    id_ = r.id_;
    clRetainMemObject(id_);
  }
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
      throw cl_error(err, "error building CL program from source");
    }
  }
  inline ~program_ref_() {
#ifdef CLREFCOUNT
    uint32_t refcount;
    clGetProgramInfo(id_,
        CL_PROGRAM_REFERENCE_COUNT,
        sizeof(refcount),
        &refcount,
        NULL);
    std::cerr << "cl::program dtor; refcount: " << 
        (refcount-1) << std::endl;
#endif
    clReleaseProgram(id_);
  }

  inline void build(const std::string &compiler_opts="") {
    std::size_t num_chars;
    int err = clBuildProgram(id_,
        0, NULL,
        compiler_opts.c_str(),
        NULL, NULL);
    if(err != CL_SUCCESS) {
      throw cl_error(err, "error compiling CL program for all devices"
          " in context");
    }
  }

  template<typename ITER1, typename ITER2>
  inline void build_for(ITER1 begin, ITER2 end,
      const std::string &compiler_opts="") {
    std::vector<device_ref> devices;
    devices.insert(begin, end);
    int err = clBuildProgram(id_,
        devices.size(), reinterpret_cast<cl_device_id*>(&devices[0]),
        compiler_opts.c_str(),
        NULL, NULL);
    if(err != CL_SUCCESS) {
      throw cl_error(err, "error building CL program for specific "
          "devices");
    }
  }

  inline std::string get_build_log(const device_ref &d) {
    std::size_t log_size;
    int err = clGetProgramBuildInfo(id_,
        d.id(),
        CL_PROGRAM_BUILD_LOG,
        0,
        NULL,
        &log_size);
    if(err != CL_SUCCESS) {
      throw cl_error(err, "error sizing CL build log");
    }
    std::string to_return;
    to_return.resize(log_size+1);
    err = clGetProgramBuildInfo(id_,
        d.id(),
        CL_PROGRAM_BUILD_LOG,
        log_size,
        &to_return[0],
        NULL);
    if(err != CL_SUCCESS) {
      throw cl_error(err, "error getting CL build log");
    }
    return to_return;
  }

  class kernel_ref_<0> get_kernel(const std::string &kernel_name);

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

namespace {
template<typename RETVAL> RETVAL kernel_get_workgroup_info_(
    cl_kernel kernel_id,
    cl_device_id device_id,
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
    throw cl_error(err, "cl error in clGetKernelWorkGroupInfo");
  }
  return retval;
}

template<> std::vector<std::size_t>
kernel_get_workgroup_info_<std::vector<std::size_t> >(
    cl_kernel kernel_id,
    cl_device_id device_id,
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
    throw cl_error(err, "cl error in clGetKernelWorkGroupInfo");
  }
  return buffer;
}
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
  inline ~kernel_ref_() { 
#ifdef CLREFCOUNT
    uint32_t refcount;
    clGetKernelInfo(id_,
        CL_KERNEL_REFERENCE_COUNT,
        sizeof(refcount),
        &refcount,
        NULL);
    std::cerr << "cl::kernel dtor; refcount: " << 
        (refcount-1) << std::endl;
#endif
    clReleaseKernel(id_);
  }

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
  inline get_info(const device_ref &d) const {
    return kernel_get_workgroup_info_
        <typename cl_kernel_workgroup_info_retval<PARAM>::value>
        (id_,
         d.id(),
         PARAM);
  }

  inline std::size_t work_group_size(const device_ref &d) const {
    return get_info<CL_KERNEL_WORK_GROUP_SIZE>(d);
  }
  inline std::vector<std::size_t> compile_work_group_size
      (const device_ref &d) const {
    return get_info<CL_KERNEL_COMPILE_WORK_GROUP_SIZE>(d);
  }
  inline uint64_t local_mem_size(const device_ref &d) const {
    return get_info<CL_KERNEL_LOCAL_MEM_SIZE>(d);
  }

  template<typename T>
  inline kernel_ref_& set_arg(uint32_t index, const T &t) {
    int err;
    err = clSetKernelArg(id_,
        index,
        sizeof(T),
        &t);
    if(err != CL_SUCCESS) {
      throw cl_error(err, "error binding kernel argument!");
    }
  }

  inline cl_kernel id() const {
    return id_;
  }

private:
  cl_kernel id_;
};

template<int N> 
kernel_ref program_ref_<N>::get_kernel(const std::string &kernel_name) {
  int err;
  cl_kernel kernel_id = clCreateKernel(id_, kernel_name.c_str(), &err);
  if(err != CL_SUCCESS) {
    throw cl_error(err, "error creating kernel from program.  "
        "has it been built?");
  }
  return kernel_ref(kernel_id);
}

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
  inline command_queue_ref_(context_ref context,
    device_ref device,
    bool out_of_order = false,
    bool profiling = false)
      : id_(NULL) {
    int err;
    id_ = clCreateCommandQueue(context.id(), device.id(),
          (out_of_order ? CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE : 0) |
          (profiling ? CL_QUEUE_PROFILING_ENABLE : 0),
        &err);
    if(err != CL_SUCCESS) {
      throw cl_error(err, "error creating command queue");
    }
  }
  inline ~command_queue_ref_() {
#ifdef CLREFCOUNT
    uint32_t refcount;
    clGetCommandQueueInfo(id_,
        CL_QUEUE_REFERENCE_COUNT,
        sizeof(refcount),
        &refcount,
        NULL);
    std::cerr << "cl::command_queue dtor; refcount: " << 
        (refcount-1) << std::endl;
#endif
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
  inline bool operator==(const command_queue_ref_ &q) {
    return id_ == q.id_;
  }

  event_ref read_buffer(buffer_ref buffer, std::size_t size,
      void *dest, 
      bool blocking=false, 
      std::size_t offset=0) {
    cl_event event;
    int err;
    err = clEnqueueReadBuffer(id_, 
        buffer.id(), 
        blocking,
        offset,
        size,
        dest,
        0, NULL, &event);
    if(err != CL_SUCCESS) {
      throw cl_error(err, "error enqueueing cl buffer read");
    }
    return event_ref(event);
  }

  template<typename ITER1, typename ITER2>
  event_ref read_buffer(buffer_ref buffer, std::size_t size,
      void *dest, ITER1 wait_begin, ITER2 wait_end,
      bool blocking=false, std::size_t offset=0) {
    cl_event event;
    std::vector<event_ref> events;
    events.insert(wait_begin, wait_end);
    int err;
    err = clEnqueueReadBuffer(id_,
        buffer.id(),
        blocking,
        offset,
        size,
        dest,
        events.size(),
        reinterpret_cast<cl_event*>(&events[0]),
        &event);
    if(err != CL_SUCCESS) {
      throw cl_error(err, "error enqueueing cl buffer read");
    }
    return event_ref(event);
  }

  event_ref read_image2d(image2d_ref buffer, 
      void *dest,
      const ghp::vector<2, std::size_t> &size,
      bool blocking = false,
      const ghp::vector<2, std::size_t> &offset = 
          ghp::vector2<std::size_t>(0,0)) {
    cl_event event;
    int err;
    err = clEnqueueReadImage(id_,
        buffer.id(),
        blocking,
        &offset[0],
        &size[0],
        0,
        0,
        dest,
        0, NULL,
        &event);
    if(err != CL_SUCCESS) {
      throw cl_error(err, "error enqueueing cl image2d read");
    }
    return event_ref(event);
  }

  template<typename ITER1, typename ITER2>
  event_ref read_image2d(image2d_ref buffer,
      void *dest,
      const ghp::vector<2, std::size_t> &size,
      ITER1 wait_begin, ITER2 wait_end,
      bool blocking = false,
      const ghp::vector<2, std::size_t> &offset =
          ghp::vector2<std::size_t>(0,0)) {
    cl_event event;
    std::vector<event_ref> events;
    events.insert(events.begin(), wait_begin, wait_end);
    int err;
    err = clEnqueueReadImage(id_,
        buffer.id(),
        blocking,
        &offset[0],
        &size[0],
        0,0,
        dest,
        events.size(), reinterpret_cast<cl_event*>(&events[0]),
        &event);
    if(err != CL_SUCCESS) {
      throw cl_error(err, "error enqueueing cl buffer read");
    }
  }

  event_ref write_buffer(buffer_ref buffer, std::size_t size,
      void *src, 
      bool blocking=false, 
      std::size_t offset=0) {
    cl_event event;
    int err;
    err = clEnqueueWriteBuffer(id_, 
        buffer.id(), 
        blocking,
        offset,
        size,
        src,
        0, NULL, &event);
    if(err != CL_SUCCESS) {
      throw cl_error(err, "error enqueueing cl buffer write");
    }
    return event_ref(event);
  }

  template<typename ITER1, typename ITER2>
  event_ref write_buffer(buffer_ref buffer, std::size_t size,
      void *src, ITER1 wait_begin, ITER2 wait_end,
      bool blocking=false, std::size_t offset=0) {
    cl_event event;
    std::vector<event_ref> events;
    events.insert(events.begin(), wait_begin, wait_end);
    int err;
    err = clEnqueueWriteBuffer(id_,
        buffer.id(),
        blocking,
        offset,
        size,
        src,
        events.size(),
        reinterpret_cast<cl_event*>(&events[0]),
        &event);
    if(err != CL_SUCCESS) {
      throw cl_error(err, "error enqueueing cl buffer write");
    }
    return event_ref(event);
  }

  event_ref write_image2d(image2d_ref buffer,
      const ghp::vector<2, std::size_t> &size, 
      void *src,
      bool blocking=false,
      const ghp::vector<2, std::size_t> &offset = 
          ghp::vector2<std::size_t>(0,0)) {
    cl_event event;
    int err;
    ghp::vector<3, std::size_t> size2 = ghp::vector3<std::size_t>(
        size[0], size[1], 1);
    ghp::vector<3, std::size_t> offset2 = ghp::vector3<std::size_t>(
        offset[0], offset[1], 0);
    err = clEnqueueWriteImage(id_,
        buffer.id(),
        blocking,
        &offset2[0],
        &size2[0],
        0,
        0,
        src,
        0, NULL,
        &event);
    if(err != CL_SUCCESS) {
      throw cl_error(err, "error enqueueing image2d write");
    }
    return event_ref(event);
  }

  template<typename ITER1, typename ITER2>
  event_ref write_image2d(image2d_ref buffer,
      const ghp::vector<2, std::size_t> &size,
      void *src,
      ITER1 event_begin, ITER2 event_end,
      bool blocking = false,
      const ghp::vector<2, std::size_t> &offset =
          ghp::vector2<std::size_t>(0,0)) {
    cl_event event;
    int err;
    ghp::vector<3, std::size_t> size2 = ghp::vector3<std::size_t>(
        size[0], size[1], 1);
    ghp::vector<3, std::size_t> offset2 = ghp::vector3<std::size_t>(
        offset[0], offset[1], 0);
    std::vector<cl::event_ref> events;
    events.insert(events.begin(), event_begin, event_end);
    err = clEnqueueWriteImage(id_,
        buffer.id(),
        blocking,
        &offset2[0],
        &size2[0],
        0,0,
        src,
        events.size(), 
        reinterpret_cast<cl_event*>(&events[0]),
        &event);
    if(err != CL_SUCCESS) {
      throw cl_error(err, "error enqueueing cl image write");
    }
    return event_ref(event);
  }

  event_ref copy_buffer(buffer_ref src, buffer_ref dst,
      std::size_t size,
      std::size_t src_offset=0, std::size_t dst_offset=0) {
    cl_event event;
    int err;
    err = clEnqueueCopyBuffer(id_, 
        src.id(),
        dst.id(),
        src_offset,
        dst_offset,
        size,
        0, NULL,
        &event);
    if(err != CL_SUCCESS) {
      throw cl_error(err, "error enqueueing cl buffer copy");
    }
    return event_ref(event);
  }

  template<typename ITER1, typename ITER2>
  event_ref copy_buffer(buffer_ref src, buffer_ref dst,
      std::size_t size,
      ITER1 wait_begin, ITER2 wait_end,
      std::size_t src_offset=0, std::size_t dst_offset=0) {
    cl_event event;
    int err;
    std::vector<event_ref> events;
    events.insert(wait_begin, wait_end);
    err = clEnqueueCopyBuffer(id_, 
        src.id(),
        dst.id(),
        src_offset,
        dst_offset,
        size,
        events.size(), 
        reinterpret_cast<cl_event*>(&events[0]),
        &event);
    if(err != CL_SUCCESS) {
      throw cl_error(err, "error enqueueing cl buffer copy");
    }
    return event_ref(event);
  }
  
  template<typename S1, typename S2>
  event_ref run_kernel(kernel_ref kernel,
      std::size_t work_dim,
      const S1 &global_sizes,
      const S2 &local_sizes) {
    cl_event event;
    int err;
    err = clEnqueueNDRangeKernel(id_,
        kernel.id(),
        work_dim,
        NULL,
        &global_sizes[0],
        &local_sizes[0],
        0, NULL,
        &event);
    if(err != CL_SUCCESS) {
      throw cl_error(err, "error enqueueing cl kernel");
    }
    return event_ref(event);
  }

  template<typename S1, typename S2, typename ITER1, typename ITER2>
  event_ref run_kernel(kernel_ref kernel,
      std::size_t work_dim,
      const S1 &global_sizes,
      const S2 &local_sizes,
      ITER1 wait_begin,
      ITER2 wait_end) {
    cl_event event;
    int err;
    std::vector<event_ref> events;
    events.insert(events.begin(), wait_begin, wait_end);
    err = clEnqueueNDRangeKernel(id_,
        kernel.id(),
        work_dim,
        NULL,
        &global_sizes[0],
        &local_sizes[0],
        events.size(), 
        reinterpret_cast<cl_event*>(&events[0]),
        &event);
    if(err != CL_SUCCESS) {
      throw cl_error(err, "error enqueueing cl kernel");
    }
    return event_ref(event);
  }

  template<typename ITER1, typename ITER2> 
  event_ref acquire_gl_objects(ITER1 objects_start, ITER2 objects_end) {
    cl_event event;
    int err;
    std::vector<buffer_ref_base> objs;
    objs.insert(objs.begin(), objects_start, objects_end);
    err = clEnqueueAcquireGLObjects(
        id_,
        objs.size(),
        reinterpret_cast<cl_mem*>(&objs[0]),
        0, NULL,
        &event);
    if(err != CL_SUCCESS) {
      throw cl_error(err, "error enqueueing GL object acquire");
    }
    return event_ref(event);
  }

  template<typename ITER1, typename ITER2,
      typename ITER3, typename ITER4>
  event_ref acquire_gl_objects(
      ITER1 objects_start, ITER2 objects_end,
      ITER3 events_start, ITER4 events_end) {
    cl_event event;
    int err;
    std::vector<buffer_ref_base> objs;
    std::vector<event_ref> events;
    objs.insert(objs.begin(), objects_start, objects_end);
    events.insert(events.begin(), events_start, events_end);
    err = clEnqueueAcquireGLObjects(
        id_,
        objs.size(),
        reinterpret_cast<cl_mem*>(&objs[0]),
        events.size(),
        reinterpret_cast<cl_event*>(&events[0]),
        &event);
    if(err != CL_SUCCESS) {
      throw cl_error(err, "error enqueueing GL object acquire");
    }
    return event_ref(event);
  }

  template<typename ITER1, typename ITER2>
  event_ref release_gl_objects(
      ITER1 objects_start, ITER2 objects_end) {
    cl_event event;
    int err;
    std::vector<buffer_ref_base> objs;
    objs.insert(objs.begin(), objects_start, objects_end);
    err = clEnqueueReleaseGLObjects(
        id_,
        objs.size(),
        reinterpret_cast<cl_mem*>(&objs[0]),
        0, NULL,
        &event);
    if(err != CL_SUCCESS) {
      throw cl_error(err, "error enqueueing GL object release");
    }
    return event_ref(event);
  }

  template<typename ITER1, typename ITER2,
      typename ITER3, typename ITER4>
  event_ref release_gl_objects(
      ITER1 objects_start, ITER2 objects_end,
      ITER3 events_start, ITER4 events_end) {
    cl_event event;
    int err;
    std::vector<buffer_ref_base> objs;
    std::vector<event_ref> events;
    objs.insert(objs.begin(), objects_start, objects_end);
    events.insert(events.begin(), events_start, events_end);
    err = clEnqueueReleaseGLObjects(
        id_,
        objs.size(),
        reinterpret_cast<cl_mem*>(&objs[0]),
        events.size(),
        reinterpret_cast<cl_event*>(&events[0]),
        &event);
    if(err != CL_SUCCESS) {
      throw cl_error(err, "error enqueueing GL object release");
    }
    return event_ref(event);
  }

private:
  cl_command_queue id_;
};

}

#endif

