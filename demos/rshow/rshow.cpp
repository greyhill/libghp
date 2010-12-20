#include "rshow_shared.hpp"

#include <ghp/gfx.hpp>
#include <ghp/gfx/sdl.hpp>
#include <ghp/gfx/gl.hpp>

#include <ghp/util/sdl_input.hpp>

#include <boost/filesystem.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/thread.hpp>

#include <list> 

// globals
struct tile {
  picture_ptr picture_;

};

volatile bool running = true;

bool quit_handler(const sdl::quit_event &evt) {
  (void)evt;
  std::cout << "user quit." << std::endl;
  running = false;
  return 0;
}

void update_physics(float seconds) {
}

int main(int argc, char *argv[]) {
  (void)argc;
  (void)argv;

  sdl::gl_display display(800, 600, false);
  display.enable_vsync(true);
  display.set_caption("ghp rshow demo");
  gl::init();

  sdl::input input;
  input.quit_slot().connect(quit_handler);

  rshow_db db("test.db");
  ghp::texture<ghp::RGBA<uint8_t> > src_tex;
  sdl::load_image("Matlab.png", src_tex);
  int id;
  {
    picture_ptr src_pic = db.new_picture(src_tex);
    id = src_pic->id();
    src_pic->tags().insert("test_tag");
  }
  {
    picture_ptr dst_pic = db.get_picture(id);
    std::cout << "tags: " << "\n";
    for(std::set<std::string>::iterator it = dst_pic->tags().begin();
        it != dst_pic->tags().end(); ++it) {
      std::cout << *it << ", ";
    }
    std::cout << std::endl;
    sdl::save_bmp("out.bmp", dst_pic->texture());
  }


  while(running) {
    /* draw scene */

    /* update display, etc. */
    float secs = display.update();
    input.handle_events();
    update_physics(secs);
  }

  return 0;
}

