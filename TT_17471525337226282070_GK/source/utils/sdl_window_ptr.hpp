#ifndef TT_SDL_WINDOW_PTR_HPP
#define TT_SDL_WINDOW_PTR_HPP

#include <utility>

#include <SDL_video.h>

namespace tt_program::utilss
{

class sdl_window_ptr
{
public:
	sdl_window_ptr()
		: m_window(nullptr)
	{}

	sdl_window_ptr(const char *title, int x, int y, int w, int h, Uint32 flags)
		: m_window(SDL_CreateWindow(title, x, y, w, h, flags))
	{}

	sdl_window_ptr(sdl_window_ptr & other) = delete;
	sdl_window_ptr & operator=(sdl_window_ptr & other) = delete;

	sdl_window_ptr(sdl_window_ptr && other)
		: m_window(std::exchange(other.m_window, nullptr))
	{}

	sdl_window_ptr & operator=(sdl_window_ptr && other)
	{
		m_window = std::exchange(other.m_window, nullptr);
		return *this;
	}

	~sdl_window_ptr()
	{
		SDL_DestroyWindow(m_window);
	}

public:
	explicit operator bool() const noexcept
	{
		return m_window != nullptr;
	}

public:
	typename std::add_lvalue_reference<SDL_Window>::type operator*()
	{
		return *m_window;
	}

	SDL_Window * operator->()
	{
		return m_window;
	}

	SDL_Window * get()
	{
		return m_window;;
	}

private:
	SDL_Window * m_window;	
};

} // namespace tt_program::utilss

#endif // TT_SDL_WINDOW_PTR_HPP
