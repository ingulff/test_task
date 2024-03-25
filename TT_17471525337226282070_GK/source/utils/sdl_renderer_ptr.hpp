#ifndef TT_SDL_RENDERER_PTR_HPP
#define TT_SDL_RENDERER_PTR_HPP

#include <utility>

#include <SDL_render.h>

namespace tt_program::utilss
{

class sdl_renderer_ptr
{
public:
	sdl_renderer_ptr()
		: m_renderer(nullptr)
	{}

	sdl_renderer_ptr(SDL_Window * window_ptr, int index, Uint32 flags)
		: m_renderer(SDL_CreateRenderer(window_ptr, index, flags))
	{}

	sdl_renderer_ptr(sdl_renderer_ptr & other) = delete;
	sdl_renderer_ptr & operator=(sdl_renderer_ptr & other) = delete;

	sdl_renderer_ptr(sdl_renderer_ptr && other)
		: m_renderer(std::exchange(other.m_renderer, nullptr))
	{}

	sdl_renderer_ptr & operator=(sdl_renderer_ptr && other)
	{
		m_renderer = std::exchange(other.m_renderer, nullptr);
		return *this;
	}

	~sdl_renderer_ptr()
	{
		SDL_DestroyRenderer(m_renderer);
	}

public:
	explicit operator bool() const noexcept
	{
		return m_renderer != nullptr;
	}

public:
	typename std::add_lvalue_reference<SDL_Renderer>::type operator*()
	{
		return *m_renderer;
	}

	SDL_Renderer * operator->()
	{
		return m_renderer;
	}

	SDL_Renderer * get()
	{
		return m_renderer;
	}	

private:
	SDL_Renderer * m_renderer;
};

} // namespace tt_program::utilss

#endif TT_SDL_RENDERER_PTR_HPP
