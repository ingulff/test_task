#ifndef TT_DEFAULT_SETTINGS_HPP
#define TT_DEFAULT_SETTINGS_HPP

#include <cstdint>

namespace tt_program
{

#ifdef DEFAULT_WINDWOW_WIDTH
inline constexpr std::int32_t default_window_width = DEFAULT_WINDWOW_WIDTH;
#else
inline constexpr std::int32_t default_window_width = 1000;
#endif // DEFAULT_WINDWOW_WIDTH
#undef DEFAULT_WINDWOW_WIDTH

#ifdef DEFAULT_WINDOW_HEIGHT
inline constexpr std::int32_t default_window_height = DEFAULT_WINDOW_HEIGHT;
#else
inline constexpr std::int32_t default_window_height = 1000;
#endif // DEFAULT_WINDOW_HEIGHT
#undef DEFAULT_WINDOW_HEIGHT

#ifdef DEFAULT_CELL_SIDE
inline constexpr std::int32_t default_cell_side = DEFAULT_CELL_SIDE;
#else
inline constexpr std::int32_t default_cell_side = 20;
#endif // DEFAULT_CELL_SIDE
#undef DEFAULT_CELL_SIDE

#ifdef DEFAULT_LOOP_BOARD
inline constexpr std::int32_t default_loop_boards = DEFAULT_LOOP_BOARD;
#else
inline constexpr std::int32_t default_loop_boards = 0;
#endif // DEFAULT_LOOP_BOARD
#undef DEFAULT_LOOP_BOARD

#ifdef DEFAULT_FULLSCREEN
inline constexpr std::int32_t default_fullscreen = DEFAULT_FULLSCREEN;
#else
inline constexpr std::int32_t default_fullscreen = 0;
#endif // DEFAULT_FULLSCREEN
#undef DEFAULT_FULLSCREEN


} // namespace tt_program

#endif // TT_DEFAULT_SETTINGS_HPP
