/**
 *  ᛝ
 */

#include <utility>

#include "board.hpp"
#include "color_palete.hpp"
#include "error/error.hpp"
#include "renderer.hpp"
#include "settings.hpp"
#include "utils/sdl_renderer_ptr.hpp"
#include "utils/sdl_window_ptr.hpp"

namespace tt_program
{

class renderer::renderer_impl
{

public:
	renderer_impl()
		: m_settings(tt_program::make_default_settings())
		, m_x_offset(0)
		, m_y_offset(0)
		, m_window()
		, m_renderer()
	{
		calc_x_offset(m_settings.window_width);
		calc_y_offset(m_settings.window_height);
	}

	renderer_impl(renderer_impl & other) = delete;
	renderer_impl & operator=(renderer_impl & other) = delete;

	renderer_impl(renderer_impl && other)
		: m_window(std::move(other.m_window))
		, m_renderer(std::move(other.m_renderer))
	{}

	renderer_impl & operator=(renderer_impl && other)
	{
		m_window = std::move(m_window);
		m_renderer = std::move(other.m_renderer);
	}

	~renderer_impl()
	{}

public:
	enum error::status_code initialize(tt_program::settings_t & settings)
	{
		m_window = tt_program::utilss::sdl_window_ptr("Life", 
				SDL_WINDOWPOS_CENTERED, 
				SDL_WINDOWPOS_CENTERED, 
				m_settings.window_width, 
				m_settings.window_height,
				0);
		m_renderer = tt_program::utilss::sdl_renderer_ptr(m_window.get(),  -1, SDL_RENDERER_PRESENTVSYNC);

		enum error::status_code result = error::status_code::invalid;

		if( m_renderer && m_window )
		{
			m_settings = settings;
			calc_x_offset(m_settings.window_width);
			calc_y_offset(m_settings.window_height);
			result = error::status_code::paused;
		}

		return result;
	}

public:
	void update(tt_program::board_t & board)
	{
		fill_backgrownd();
		fill_cells(board);
		draw_backgrownd_lines();

		SDL_RenderPresent(m_renderer.get());
	}

public:
	std::int32_t x_offset() const
	{
		return m_x_offset;
	}

	std::int32_t y_offset() const
	{
		return m_y_offset;
	}

private:
	void calc_x_offset(int32_t window_width)
	{
		m_x_offset =  (window_width - m_settings.board_width * m_settings.cell_side) / 2;
	}

	void calc_y_offset(int32_t window_height)
	{
		m_y_offset = (window_height - m_settings.board_height * m_settings.cell_side) / 2;
	}

private:
	void fill_backgrownd()
	{
		SDL_SetRenderDrawColor(m_renderer.get(), 
			colors::backgrownd_color.red, 
			colors::backgrownd_color.green, 
			colors::backgrownd_color.blue, 
			colors::backgrownd_color.alpha );

		SDL_RenderClear(m_renderer.get());		
	}

	void draw_backgrownd_lines()
	{
		SDL_SetRenderDrawColor(m_renderer.get(), 
			colors::lines_color.red, 
			colors::lines_color.green, 
			colors::lines_color.blue,
			colors::lines_color.alpha );

		int32_t window_width = m_settings.window_width;
		int32_t window_height = m_settings.window_height;
		if( m_settings.is_fullscreen )
		{
			SDL_GetWindowSize(m_window.get(), &window_width, &window_height);
		}

		calc_x_offset(window_width);
		calc_y_offset(window_height);
		for(std::int32_t i = 0, 
			x = m_x_offset + i * m_settings.cell_side,
			y0 = m_y_offset,
			y1 = window_height - m_y_offset; 
			i <= m_settings.board_width; 
			++i)
		{
			 x = m_x_offset + i * m_settings.cell_side;
			SDL_RenderDrawLine(m_renderer.get(), x, y0, x, y1);
		}
		
		for(std::int32_t i = 0,
			y = m_y_offset + i * m_settings.cell_side,
			x0 = m_x_offset,
			x1 = window_width - m_x_offset; 
			i <= m_settings.board_height;
			 ++i)
		{
			y = m_y_offset + i * m_settings.cell_side;
			SDL_RenderDrawLine(m_renderer.get(), x0, y, x1, y);
		}
	}

	void fill_cells(tt_program::board_t & board)
	{
		for(auto alive_it = board.alive_begin(), end_it = board.alive_end();
			alive_it != end_it; ++alive_it )
		{
			if( (*alive_it).is_alive() )
			{
				SDL_SetRenderDrawColor(m_renderer.get(), 
						colors::cell_color.red, 
						colors::cell_color.green, 
						colors::cell_color.blue,
						colors::cell_color.alpha );

				auto point = board.coordinates(alive_it);
				SDL_Rect cell{ point.x * m_settings.cell_side + m_x_offset, 
						point.y * m_settings.cell_side + m_y_offset, 
						m_settings.cell_side, 
						m_settings.cell_side };
				draw_cell( cell );
			}	
		}
	}

	void draw_cell(SDL_Rect & cell)
	{
		SDL_RenderFillRect(m_renderer.get(), &cell);
	}

public:
	void fullscreen_handle()
	{
		m_settings.is_fullscreen = !m_settings.is_fullscreen;
		if( SDL_SetWindowFullscreen(SDL_RenderGetWindow(m_renderer.get()), m_settings.is_fullscreen) < 0)
		{
			// nothink
			//SDL_GetError();
		}
	}

private:
	tt_program::settings_t m_settings;
	std::int32_t m_x_offset;
	std::int32_t m_y_offset;
	tt_program::utilss::sdl_window_ptr m_window;
	tt_program::utilss::sdl_renderer_ptr m_renderer;
};


renderer::renderer()
	: m_impl(std::make_unique<renderer_impl>())
{}

renderer::renderer(renderer && other)
	: m_impl(std::move(other.m_impl))
	, m_status(std::move(other.m_status))
{}

renderer & renderer::operator=(renderer && other)
{
	m_impl = std::move(other.m_impl);
	m_status = std::move(other.m_status);

	return *this;
}

renderer::~renderer()
{}


enum error::status_code renderer::initialize(tt_program::settings_t & settings)
{
	m_status = m_impl->initialize(settings);

	return m_status;
}


std::int32_t renderer::x_offset() const
{
	return m_impl->x_offset();
}

std::int32_t renderer::y_offset() const
{
	return m_impl->y_offset();
}


void renderer::update(tt_program::board_t & board)
{
	m_impl->update(board);
}

void renderer::fullscreen_handle()
{
	m_impl->fullscreen_handle();
}

} // namespace tt_program