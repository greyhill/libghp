#ifndef _GHP_UTIL_CL_GL_HPP_
#define _GHP_UTIL_CL_GL_HPP_

#include "../gfx/gl.hpp"
#include "cl.hpp"
#include <cl_gl.h>

namespace cl {

/*
template<>
class image2d_ref_<0> : public buffer_ref_base {
public:
  image2d_ref_(context_ref context,
      const cl_image_format &format,
      std::size_t width,
      std::size_t height,
      bool kernel_can_read = true,
      bool kernel_can_write = true,
      bool mappable = false) {
    int err;
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

  template<typename T>
  image2d_ref_(context_ref context,
      const cl_image_format &format,
      T *buffer,
      std::size_t width,
      std::size_t height,
      bool kernel_can_read = true,
      bool kernel_can_write = true,
      bool mappable = false) {
    int err;
    id_ = clCreateImage2D(context.id(),
        ((kernel_can_read && kernel_can_write) ? CL_MEM_READ_WRITE : 0 ) |
        ((!kernel_can_read && kernel_can_write) ? CL_MEM_WRITE_ONLY : 0) |
        ((kernel_can_read && !kernel_can_write) ? CL_MEM_READ_ONLY : 0) |
        (mappable ? CL_MEM_ALLOC_HOST_PTR : 0),
        &format,
        width,
        height,
        0,
        reinterpret_cast<void*>(buffer),
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


// use specialization trick to replace regular object with
// one including GL/CL interop code
template<> 
class command_queue_ref_<0> {
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
    events.insert(wait_begin, wait_end);
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

  event_ref write_image2d(buffer_ref buffer,
      const ghp::vector<2, std::size_t> &size, 
      void *src,
      bool blocking=false,
      const ghp::vector<2, std::size_t> &offset = 
          ghp::vector2<std::size_t>(0,0)) {
    cl_event event;
    int err;
    err = clEnqueueWriteImage(id_,
        buffer.id(),
        blocking,
        &offset[0],
        &size[0],
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
  event_ref write_image2d(buffer_ref buffer,
      const ghp::vector<2, std::size_t> &size,
      void *src,
      ITER1 event_begin, ITER2 event_end,
      bool blocking = false,
      const ghp::vector<2, std::size_t> &offset =
          ghp::vector2<std::size_t>(0,0)) {
    cl_event event;
    int err;
    std::vector<cl::event_ref> events;
    events.insert(events.begin(), event_begin, event_end);
    err = clEnqueueWriteImage(id_,
        buffer.id(),
        blocking,
        &offset[0],
        &size[0],
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

*/
}

#endif

