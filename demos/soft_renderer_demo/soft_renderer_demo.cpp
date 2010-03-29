#include <ghp/gfx.hpp>
#include <ghp/gfx/sdl.hpp>
#include <ghp/math.hpp>

int main(int argc, char *argv[]) {
  sdl::soft_display<ghp::RGB<uint8_t> > display(800, 600, false);
  display.set_caption("Soft SDL Display");
}

