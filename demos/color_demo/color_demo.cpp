#include <ghp/gfx.hpp>
#include <ghp/gfx/sdl.hpp>

#include <iostream>

int main(int argc, char *argv[]) {
  ghp::color<ghp::RGB<uint8_t> > what_you_expect(127, 0, 0);
  ghp::color<ghp::RGB<double> > eh_why_not(0, 1, 0);

  std::cout << what_you_expect << "\n"
    << eh_why_not << std::endl;
  eh_why_not = what_you_expect;
  std::cout << eh_why_not << std::endl;

  ghp::texture<ghp::RGBA<float> > xbox_icon;
  sdl::load_texture("xbox.png", xbox_icon);
  ghp::texture<ghp::RGB<uint8_t> > xbox_large(
    xbox_icon.get_width() * 10,
    xbox_icon.get_height() * 10);
  ghp::scale_texture<ghp::direct_copy>(xbox_icon, xbox_large);
  sdl::save_bmp("xbox_large.bmp", xbox_large);

  return 0;
}

