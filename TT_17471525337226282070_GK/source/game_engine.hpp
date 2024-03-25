#ifndef TT_GAME_EMGINE_HPP
#define TT_GAME_EMGINE_HPP

#include <memory>

#include "error/error.hpp"
#include "utils/point.hpp"
#include "utils/sdl_mouse_handler.hpp"

namespace tt_program
{

class engine
{
public:
	engine();

	engine(engine & other) = delete;
	engine & operator=(engine & other) = delete;

	engine(engine && other);
	engine & operator=(engine && other);

	~engine();

public:
	enum error::status_code initialize();

	enum error::status_code initialize(std::int32_t board_width, std::int32_t board_height);

public:
	void update(error::status_code & status);

public:
	void change_cell_handle(tt_program::utilss::point_t, bool is_alive_cell);

	void clear_board_handle();

	void loop_board_handle();

	void fullscreen_handle();

private:
	class engine_impl;
	std::unique_ptr<engine_impl> m_impl;
};

} // namespace tt_program

#endif // TT_GAME_EMGINE_HPP
