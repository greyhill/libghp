__kernel
void with_div(
    __global float *in,
    __global float *out) {
  int id = get_global_id(1)*get_global_size(0) + get_global_id(0);
  float f = in[id];
  float g = 1;
  for(int i=0; i<512; ++i) {
    if(f > g) {
      g += f / 2.f;
    }
    if(g > f) {
      g -= f / 2.f;
    }
  }
  out[id] = g;
}

__kernel
void with_sync(
    __global float *in,
    __global float *out) {
  int id = get_global_id(1)*get_global_size(0) + get_global_id(0);
  float f = in[id];
  float g = 0;
  for(int i=0; i<512; ++i) {
    if(f > g) {
      g += f / 2.f;
    }
    if(g > f) {
      g -= f / 2.f;
    }
    barrier(CLK_LOCAL_MEM_FENCE);
  }
  out[id] = g;
}

__kernel
void no_div(
    __global float *in,
    __global float *out) {
  int id = get_global_id(1)*get_global_size(0) + get_global_id(0);
  float f = in[id];
  float g = 0;
  for(int i=0; i<512; ++i) {
    //g += (f > g)*(f/2.f) - (g > f)*(f/2.f);
    g += step(g, f)*(f/2.f) - step(f,g)*(f/2.f);
  }
  out[id] = g;
}

