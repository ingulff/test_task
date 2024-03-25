/**
 *  ᛝ
 */

#include <functional>
#include <utility>

#include <SDL_events.h>
#include <SDL_rect.h>

#include "error/error.hpp"
#include "io_interactor.hpp"
#include "utils/point.hpp"
#include "utils/sdl_mouse_handler.hpp"


namespace tt_program
{

class io_interactor::io_interactor_impl
{

public:
	io_interactor_impl()
		: m_event()
		, m_callbacks()
	{}

	io_interactor_impl(io_interactor_impl & other) = delete;
	io_interactor_impl & operator=(io_interactor_impl & other) = delete;

	io_interactor_impl(io_interactor_impl && other)
		: m_event(std::move(other.m_event))
		, m_callbacks(std::move(other.m_callbacks))
	{}

	io_interactor_impl & operator=(io_interactor_impl && other)
	{
		m_event = std::move(other.m_event);
		m_callbacks = std::move(other.m_callbacks);

		return *this;
	}

	~io_interactor_impl()
	{}

public:
	enum error::status_code  initialize(callbacks_t callbacks)
	{
		m_callbacks = std::move(callbacks);
		return error::status_code::paused; // start state
	}

public:
	void update(error::status_code status)
	{
		using tt_program::utilss::to_mouse_button_type;

		auto mouse_status = tt_program::utilss::mouse_handle();
		if( is_paintable(status) && m_callbacks.chagne_cell_handle )
		{
			if( tt_program::utilss::is_left_clicked(to_mouse_button_type(mouse_status.status))  )
			{
				m_callbacks.chagne_cell_handle({mouse_status.pos_x, mouse_status.pos_y}, true);
			}

			if( tt_program::utilss::is_right_clicked(to_mouse_button_type(mouse_status.status)) )
			{
				m_callbacks.chagne_cell_handle({mouse_status.pos_x, mouse_status.pos_y}, false);
			}
			
		}

		if(SDL_PollEvent(&m_event))
		{
			switch(m_event.type)
			{
			case SDL_KEYDOWN:
				key_interact();
				break;
			case SDL_QUIT:
				m_callbacks.quit_handle();
				break;
			default:
				// nothink
				break;
			}
		}
	}

private:
	bool is_paintable(error::status_code status)
	{
		return error::status_code::paused == status;
	}

private:
	void key_interact()
	{
		switch(m_event.key.keysym.sym)
		{
		case SDLK_ESCAPE:
			if(m_callbacks.quit_handle)
			{
				m_callbacks.quit_handle();
			}
			break;

		case SDLK_SPACE:
			if(m_callbacks.pause_handle)
			{
				m_callbacks.pause_handle();
			}
			break;

		case SDLK_c:
			if(m_callbacks.clear_board_handle)
			{
				m_callbacks.clear_board_handle();
			}
			break;

		case SDLK_f:
			if(m_callbacks.fullscreen_handle)
			{
				m_callbacks.fullscreen_handle();
			}
			break;

		case SDLK_l:
			if(m_callbacks.loop_board_handle)
			{
				m_callbacks.loop_board_handle();
			}
			break;

		default:
			// nothink
			break;
		}
	}

private:
	SDL_Event m_event;
	callbacks_t m_callbacks;
};


io_interactor::io_interactor()
	: m_impl(std::make_unique<io_interactor_impl>())
{}

io_interactor::io_interactor(io_interactor && other) = default;
io_interactor & io_interactor::operator=(io_interactor && other) = default;

io_interactor::~io_interactor()
{}


enum error::status_code io_interactor::initialize(callbacks_t callbacks)
{
	return m_impl->initialize( std::move(callbacks) );
}


void io_interactor::update(error::status_code status)
{
	m_impl->update(status);
}


callbacks_t make_callbacks(std::function<void()> quit_handle, 
	std::function<void()> pause_handle,
	std::function<void(tt_program::utilss::point_t mouse_point,  bool is_alive_cell)> chagne_cell_handle,
	std::function<void()> clear_board_handle,
	std::function<void()> fullscreen_handle,
	std::function<void()> loop_table_handle)
{
	return { std::move(quit_handle), 
		std::move(pause_handle), 
		std::move(chagne_cell_handle),
		std::move(clear_board_handle),
		std::move(fullscreen_handle),
		std::move(loop_table_handle)};
}

} // namespace tt_program
