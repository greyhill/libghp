#include <ghp/util/cl.hpp>

#include <iostream>
#include <list>
#include <vector>

int main(int argc, char *argv[]) {
  std::list<cl::platform> platforms;
  cl::platform::get_platforms(platforms);

  cl::platform p = platforms.front();

  std::vector<cl::device> devices;
  p.get_devices(devices);
  for(std::size_t i=0; i<devices.size(); ++i) {
    cl::device d = devices[i];
    std::cout << d.name() << std::endl;
    std::cout << d.extensions() << std::endl;
    std::cout << d.global_mem_cacheline_size() << std::endl;
  }
}

