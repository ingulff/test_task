/**
 *  ᛝ
 */

#include "default_settings.hpp"
#include "settings.hpp"


namespace tt_program
{

settings_t make_default_settings()
{
	return settings_t
	{
		default_window_width,
		default_window_height,
		default_cell_side,
		default_window_width / default_cell_side,
		default_window_height / default_cell_side,
		default_loop_boards,
		default_fullscreen
	};
}

} // namespace tt_program

