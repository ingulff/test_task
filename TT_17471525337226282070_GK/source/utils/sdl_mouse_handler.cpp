/**
 *  ᛝ
 */

#include <SDL.h>
#undef main

#include "utils/sdl_mouse_handler.hpp"

namespace tt_program::utilss
{

struct mouse_t mouse_handle()
{
	mouse_t mouse_status;
	mouse_status.status = SDL_GetMouseState(&mouse_status.pos_x, &mouse_status.pos_y);

	return mouse_status;
}


enum mouse_button_t to_mouse_button_type(int type)
{
	enum mouse_button_t button;
	switch(type)
	{
	case 1:
		button = mouse_button_t::left;
		break;
	case 2:
		button = mouse_button_t::middle;
		break;
	case 3:
		button = mouse_button_t::left_mid;
		break;
	case 4:
		button = mouse_button_t::right;
		break;
	case 5:
		button = mouse_button_t::left_right;
		break;
	case 6:
		button = mouse_button_t::right_mid;
		break;
	default:
		button = mouse_button_t::unknown;
	}

	return button;
}


bool is_left_clicked(mouse_button_t status)
{
	return status == mouse_button_t::left;
}

bool is_right_clicked(mouse_button_t status)
{
	return status == mouse_button_t::right;
}

} // namespace tt_program::utilss
