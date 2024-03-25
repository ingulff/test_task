#ifndef TT_COLOR_PALETE_HPP
#define TT_COLOR_PALETE_HPP

#include <cstdint>
#include <SDL.h>
#undef main

namespace colors
{

struct color_t
{
	std::uint8_t red;
	std::uint8_t green;
	std::uint8_t blue;
	std::uint8_t alpha; 
};

inline struct color_t backgrownd_color{ 255, 255, 240, SDL_ALPHA_OPAQUE };
inline struct color_t lines_color{ 0, 64, 64, SDL_ALPHA_OPAQUE };
inline struct color_t cell_color{ 134, 1, 17, SDL_ALPHA_OPAQUE };

} // namespace colors

#endif // TT_COLOR_PALETE_HPP
