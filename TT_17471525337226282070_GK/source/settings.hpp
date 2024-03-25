#ifndef TT_SETTINGS_HPP
#define TT_SETTINGS_HPP

namespace tt_program
{

struct settings_t
{
	std::int32_t window_width;
	std::int32_t window_height;
	std::int32_t cell_side;
	std::int32_t board_width;
	std::int32_t board_height;
	bool is_loop_board;
	bool is_fullscreen;
};

settings_t make_default_settings();


} // namespace tt_program

#endif // TT_SETTINGS_HPP
