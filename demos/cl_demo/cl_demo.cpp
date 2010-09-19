#include <ghp/util/cl.hpp>

#include <iostream>
#include <list>

int main(int argc, char *argv[]) {
  std::list<cl::platform> platforms;
  cl::platform::get_platforms(platforms);

  cl::platform p = platforms.front();
  std::cout << p.name() << std::endl;
}

