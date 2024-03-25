/**
 *  ᛝ
 */

#include <memory>
#include <set>
#include <utility>

#include <cstdint>

#include "board.hpp"
#include "error/error.hpp"
#include "game_engine.hpp"
#include "renderer.hpp"
#include "settings.hpp"
#include "utils/sdl_mouse_handler.hpp"

namespace tt_program
{

class engine::engine_impl
{

private:
	using board_t = tt_program::board_t;
	using point_t = tt_program::utilss::point_t;

public:
	engine_impl()
		: m_settings(tt_program::make_default_settings())
		, m_renderer()
		, m_board()
		, m_reserve_board()
	{}


public:
	enum error::status_code initialize()
	{
		return initialize_impl();
	}

	enum error::status_code initialize(std::int32_t board_width, std::int32_t board_height)
	{
		m_settings.board_width = board_width;
		m_settings.board_height = board_height;
		m_settings.cell_side = ( (m_settings.window_width / board_width) < (m_settings.window_height / board_height) ) ?
			 (m_settings.window_width / board_width) : (m_settings.window_height / board_height);

		return initialize_impl();
	}

private:
	enum error::status_code initialize_impl()
	{
		auto status = m_renderer.initialize(m_settings);
		if( is_initialized(status) )
		{
			m_board = board_t(m_settings.board_width, m_settings.board_height);
			m_reserve_board = board_t(m_settings.board_width, m_settings.board_height);
		}
		
		return error::status_code::paused;
	}

public:
	void update(error::status_code & status)
	{
		if( is_playable(status) )
		{
			m_reserve_board.clear();
			calculate_next_step();

			std::swap(m_board, m_reserve_board);
		}

		m_renderer.update(m_board);
	}

private:
	bool is_initialized(error::status_code & status)
	{
		return status == error::status_code::paused;
	}

	bool is_playable(error::status_code & status)
	{
		return status == error::status_code::active;
	}

private:
	void calculate_next_step()
	{
		for(auto it = m_board.begin(), end_it = m_board.end(); it != end_it; ++it)
		{
			auto point = m_board.coordinates(*it);
			auto neighbors = (m_settings.is_loop_board) ? calclulate_neighbors_loop(point) : calclulate_neighbors(point);
			m_reserve_board[point] = will_be_aline(point, neighbors) | will_and_was_alive(point, neighbors);
		}
	}

private:

	bool will_be_aline(tt_program::utilss::point_t & point, std::int32_t neighbors)
	{
 
		return ( (neighbors == 3) && !m_board[point].is_alive() );
 	}

 	bool will_and_was_alive(tt_program::utilss::point_t & point, std::int32_t neighbors)
 	{
 		return ( (neighbors == 2 || neighbors == 3) && m_board[point].is_alive() );
 	}


	std::uint8_t calclulate_neighbors(tt_program::utilss::point_t & point)
	{
		std::int32_t prev_x = point.x - 1;
		std::int32_t next_x = point.x + 1;
		std::int32_t prev_y = point.y - 1;
		std::int32_t next_y = point.y + 1;
		
		std::uint8_t neighbors = 0;
		
		bool has_prev_x = (prev_x > -1);
		bool has_next_x = (next_x < m_settings.board_width);
		bool hax_prev_y = (prev_y > -1);
		bool has_next_y = (next_y < m_settings.board_height);

		if( hax_prev_y )
		{
			if( has_prev_x )
			{
				neighbors += m_board[ { prev_x, prev_y } ].is_alive();
			}

			neighbors += m_board[ { point.x, prev_y } ].is_alive();

			if( has_next_x )
			{
				neighbors += m_board[ { next_x, prev_y } ].is_alive();
			}
		}

		if( has_next_y )
		{
			if( has_prev_x )
			{
				neighbors += m_board[ { prev_x, next_y } ].is_alive();
			}

			neighbors += m_board[ { point.x, next_y } ].is_alive();

			if( has_next_x )
			{
				neighbors += m_board[ { next_x, next_y } ].is_alive();
			}
		}

		if( has_prev_x )
		{
			neighbors += m_board[ { prev_x, point.y } ].is_alive();
		}

		if( has_next_x )
		{
			neighbors += m_board[ { next_x, point.y } ].is_alive();
		}

		return neighbors;
	}

	std::uint8_t calclulate_neighbors_loop(tt_program::utilss::point_t & point)
	{
		std::int32_t prev_x = (point.x - 1) < 0 ? m_settings.board_width-1 : point.x - 1;
		std::int32_t next_x = (point.x + 1) % m_settings.board_width;
		std::int32_t prev_y = (point.y - 1) < 0 ? m_settings.board_height-1 : point.y - 1;
		std::int32_t next_y = (point.y + 1) % m_settings.board_height;

		std::uint8_t neighbors = 0;
		neighbors += m_board[ { prev_x, prev_y } ].is_alive();
		neighbors += m_board[ { prev_x, point.y} ].is_alive();
		neighbors += m_board[ { prev_x, next_y } ].is_alive();
		neighbors += m_board[ { point.x, prev_y } ].is_alive();
		neighbors += m_board[ { point.x, next_y } ].is_alive();
		neighbors += m_board[ { next_x, prev_y } ].is_alive();
		neighbors += m_board[ { next_x, point.y } ].is_alive();
		neighbors += m_board[ { next_x, next_y } ].is_alive();
		

		return neighbors;
	}

public:
	void change_cell_handle(point_t mouse_position, bool is_alive_cell)
	{
		mouse_position.x -= m_renderer.x_offset();
		mouse_position.y -= m_renderer.y_offset();
		mouse_position.x /= m_settings.cell_side;
		mouse_position.y /= m_settings.cell_side;

		if(mouse_position.x < 0 || mouse_position.y < 0 
			|| mouse_position.x >= m_settings.board_width || mouse_position.y >= m_settings.board_height)
		{
			return;
		}

		m_board[mouse_position] = is_alive_cell;
	}

	void clear_board_handle()
	{
		m_reserve_board.clear();
		m_board.clear();
	}

	void loop_board_handle()
	{
		m_settings.is_loop_board = !m_settings.is_loop_board;
	}

	void fullscreen_handle()
	{
		m_renderer.fullscreen_handle();
	}

private:
	tt_program::settings_t m_settings;
	tt_program::renderer m_renderer;
	board_t m_board;
	board_t m_reserve_board;
};


engine::engine()
	: m_impl(std::make_unique<engine_impl>())
{}

engine::engine(engine && other) = default;
engine & engine::operator=(engine && other) = default;

engine::~engine()
{}


enum error::status_code engine::initialize()
{
	return m_impl->initialize();
}

enum error::status_code engine::initialize(std::int32_t board_width, std::int32_t board_height)
{
	return m_impl->initialize(board_width, board_height);
}


void engine::update(error::status_code & status)
{
	m_impl->update(status);
}


void engine::change_cell_handle(tt_program::utilss::point_t mouse_position, bool is_alive_cell)
{
	m_impl->change_cell_handle(mouse_position, is_alive_cell);
}

void engine::clear_board_handle()
{
	m_impl->clear_board_handle();
}

void engine::loop_board_handle()
{
	m_impl->loop_board_handle();
}

void engine::fullscreen_handle()
{
	m_impl->fullscreen_handle();
}

} // namespace tt_program
