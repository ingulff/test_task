#ifndef TT_SDL_MOUSE_HANDLER_HPP
#define TT_SDL_MOUSE_HANDLER_HPP

#include <cstdint>

namespace tt_program::utilss
{

struct mouse_t
{
	std::int32_t pos_x;
	std::int32_t pos_y;
	std::int32_t status;

};

struct mouse_t mouse_handle();


enum class mouse_button_t
{
	left = 1,
	middle = 2,
	left_mid = 3,
	right = 4,
	left_right = 5,
	right_mid = 6,
	unknown = 7,
};

enum mouse_button_t to_mouse_button_type(int type);


bool is_left_clicked(mouse_button_t status);
bool is_right_clicked(mouse_button_t status);

}

#endif // TT_SDL_MOUSE_HANDLER_HPP
