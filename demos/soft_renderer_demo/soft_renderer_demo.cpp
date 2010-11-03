#include <ghp/gfx.hpp>
#include <ghp/gfx/sdl.hpp>
#include <ghp/math.hpp>

int main(int argc, char *argv[]) {
  sdl::soft_display display(800, 600, false);
  display.set_caption("Soft SDL Display");
}

