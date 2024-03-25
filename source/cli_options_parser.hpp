#ifndef TT_CLI_OPTIONS_PARSER
#define TT_CLI_OPTIONS_PARSER

#include <utility>

#include <cstdint>

#include "error/error.hpp"

namespace tt_program
{

struct board_size
{
	std::int32_t width;
	std::int32_t height;

};


std::pair<error::parser_status, board_size> parse_cli_options(int argc, char ** argv);

bool is_valid_options(error::parser_status status);

bool need_use_default_settings(error::parser_status status);

} // namespace tt_program

#endif // TT_SDL_RENDERER_PTR_HPP