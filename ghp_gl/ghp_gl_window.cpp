#include "ghp_gl_window.hpp"
#include <SDL/SDL.h>
#include <GL/glew.h>

namespace ghp {
namespace gl {

// static members
boost::scoped_ptr<window> window::global_window_(NULL);

window& window::create(int32_t width, int32_t height,
		bool fullscreen) {
	bool hasCreated = false;
	if(hasCreated) {
		throw std::runtime_error(
			"Attempted to create 2nd window");
	}
	window *newWindow = new window(width, height, fullscreen);
	global_window_.reset(newWindow);
	return *newWindow;
}

window& window::get() {
	window *w = global_window_.get();
	if(w == NULL) {
		throw std::runtime_error(
			"Window hasn't been created yet!");
	}
	return *w;
}

bool window::is_created() {
	return global_window_.get() != NULL;
}

// instance members
window::window(int32_t width, int32_t height, bool fs)
		: width_(width),
		height_(height),
		fullscreen_(fs) {
	init();
}

window::~window() {
	SDL_Quit();
}

int32_t window::get_width() const {
	return width_;
}

int32_t window::get_height() const {
	return height_;
}

bool window::get_fullscreen() const {
	return fullscreen_;
}

window& window::set_grab_input(bool b) {
	SDL_WM_GrabInput(
		b ? SDL_GRAB_ON : SDL_GRAB_OFF
	);
	return *this;
}

bool window::get_grab_input() const {
	return SDL_WM_GrabInput(SDL_GRAB_QUERY) == SDL_GRAB_ON;
}

window& window::set_caption(const std::string &s) {
	SDL_WM_SetCaption(s.c_str(), NULL);
	return *this;
}

std::string window::get_caption() const {
	char *title, *icon;
	SDL_WM_GetCaption(&title, &icon);
	return std::string(title);
}

void window::init() {
	if(SDL_Init(SDL_INIT_VIDEO) == -1) {
		throw std::runtime_error(SDL_GetError());
	}
	if(SDL_SetVideoMode(width_, height_, 0,
			SDL_OPENGL | SDL_DOUBLEBUF |
			(fullscreen_ ? SDL_FULLSCREEN : 0)) == NULL) {
		throw std::runtime_error(SDL_GetError());
	}
	// use glew to setup extensions (awesome.)
	glewInit();
}

}
}

// eof //

