#ifndef _GHP_GL_WINDOW_HPP_
#define _GHP_GL_WINDOW_HPP_

#include <stdexcept>
#include <cstdint>
#include <string>
#include <boost/scoped_ptr.hpp>
#include <boost/utility.hpp>

namespace ghp {
namespace gl {

class window 
		: boost::noncopyable {
public:
	static window& create(int32_t width, int32_t height, 
		bool fullscreen);
	static window& get();
	static bool is_created();

	~window();

	int32_t get_width() const;
	int32_t get_height() const;
	bool get_fullscreen() const;
	window& set_grab_input(bool b);
	bool get_grab_input() const;

	window& set_caption(const std::string &s);
	std::string get_caption() const;

private:
	static boost::scoped_ptr<window> global_window_;

	window(int32_t width, int32_t height, bool fs);
	void init();

	int32_t width_;
	int32_t height_;
	bool fullscreen_;
};

}
}

#endif

