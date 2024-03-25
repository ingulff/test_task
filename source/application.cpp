/**
 *  ᛝ
 */

#include <utility>

#include <SDL.h>
#undef main

#include "application.hpp"
#include "error/error.hpp"
#include "game_engine.hpp"
#include "io_interactor.hpp"

namespace tt_program
{

class application_t::application_impl
{

public:
	application_impl()
		: m_engine()
		, m_io_interactor()
	{}

	application_impl(application_impl & other) = delete;
	application_impl & operator=(application_impl & other) = delete;

	application_impl(application_impl && other)
		: m_engine(std::move(other.m_engine))
		, m_io_interactor(std::move(other.m_io_interactor))
		, m_status(std::move(other.m_status))
	{}

	application_impl & operator=(application_impl && other)
	{
		m_engine = std::move(other.m_engine);
		m_io_interactor = std::move(other.m_io_interactor);
		m_status = std::move(other.m_status);
	}

	~application_impl()
	{
		this->deinitialize();
	}

public:
	void set_status(error::status_code status)
	{
		m_status = status;
	}

public:
	error::errc initialize()
	{
		if( SDL_Init(SDL_INIT_EVERYTHING) == 0 )
		{
			auto engine_status = m_engine.initialize();

			auto interactor_status = initialize_impl();

			bool is_init = is_initialized(engine_status)
				&& is_initialized(interactor_status);

			if( is_init )
			{
				m_status = error::status_code::paused;
			}
		}

		error::errc error = error::errc::invalid_initialization;
		if( is_playable() )
		{
			error = error::errc::ok;
		}

		return error;
	}


	error::errc initialize(std::int32_t board_width, std::int32_t board_height)
	{

		if( SDL_Init(SDL_INIT_EVERYTHING) == 0 )
		{
			auto engine_status = m_engine.initialize(board_width, board_height);

			auto interactor_status = initialize_impl();

			bool is_init = is_initialized(engine_status)
				&& is_initialized(interactor_status);

			if( is_init )
			{
				m_status = error::status_code::paused;
			}
		}

		error::errc error = error::errc::invalid_initialization;
		if( is_playable() )
		{
			error = error::errc::ok;
		}

		return error;
	}

	void deinitialize()
	{
		SDL_Quit();
	}

private:
	enum error::status_code initialize_impl()
	{
		auto quit_handle_callback = [this]()
		{
			this->quit_handle();
		};
		auto pause_handle_callback = [this]()
		{
			this->pause_handle();
		};
		auto change_cell_handler_callback = [this](tt_program::utilss::point_t mouse_status, bool is_alive_cell)
		{
			this->m_engine.change_cell_handle( std::move(mouse_status), is_alive_cell);
		};
		auto clear_board_hanler_callback = [this]()
		{
			this->m_engine.clear_board_handle();
		};
		auto fullscreen_handler_callback = [this]()
		{
			this->m_engine.fullscreen_handle();
		};
		auto loop_board_handler_callback = [this]()
		{
			this->m_engine.loop_board_handle();
		};
		auto interactor_status = m_io_interactor.initialize(tt_program::make_callbacks(quit_handle_callback, 
			pause_handle_callback, 
			change_cell_handler_callback,
			clear_board_hanler_callback,
			fullscreen_handler_callback,
			loop_board_handler_callback));

		return interactor_status;
	}

private:
	bool is_playable()
	{
		return m_status == error::status_code::active || m_status == error::status_code::paused;
	}

	bool is_initialized(error::status_code & status)
	{
		return status == error::status_code::paused;
	}

public:
	void quit_handle()
	{
		set_status(error::status_code::stopped);
	}

	void pause_handle()
	{
		if(m_status == error::status_code::paused)
		{
			set_status(error::status_code::active);
		}
		else
		{
			set_status(error::status_code::paused);
		}
	}

public:
	error::errc exec()
	{
		while(true)
		{
			if( !is_playable() )
			{
				break;
			}
		
			m_io_interactor.update(m_status);
			m_engine.update(m_status);
		}

		return error::to_errc(m_status);
	}

private:
	tt_program::engine m_engine;
	tt_program::io_interactor m_io_interactor;

	enum error::status_code m_status;
};


application_t::application_t()
	: m_impl(std::make_unique<application_impl>())
{}

application_t::application_t(application_t && other) = default;
application_t & application_t::operator=(application_t && other) = default;

application_t::~application_t()
{}


int application_t::initialize()
{
	return error::to_int(m_impl->initialize());
}

int application_t::initialize(std::int32_t board_width, std::int32_t board_height)
{
	return error::to_int(m_impl->initialize(board_width, board_height));
}


int application_t::exec()
{
	return error::to_int(m_impl->exec());
}

} // namespace tt_program
