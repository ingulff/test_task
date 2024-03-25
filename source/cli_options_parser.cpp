/**
 *  ᛝ
 */

#include <charconv>
#include <string>
#include <system_error>
#include <utility>

#include <cstdint>

#include "cli_options_parser.hpp"
#include "error/error.hpp"

namespace tt_program
{

std::pair<error::parser_status, board_size> parse_cli_options(int argc, char ** argv)
{
	error::parser_status status = error::parser_status::invalid;
	board_size size;

	if(argc == 1)
	{
		status = error::parser_status::need_use_default;
	}
	else if(argc == 3)
	{
		std::string_view value = argv[2];
		std::int32_t board_side;
		auto [ptr, ec] = std::from_chars(value.data(), value.data() + value.size(), board_side);
		if( std::string_view(argv[1]) != "-s" || ec != std::errc())
		{
			status = error::parser_status::invalid;
		}
		else
		{
			size.width = board_side;
			size.height = board_side;
			status = error::parser_status::ok;
		}
	}
	else if(argc == 5)
	{
		std::string_view flag_1 = argv[1];
		std::string_view flag_2 = argv[3];
		std::string_view value_1 = argv[2];
		std::string_view value_2 = argv[4];
		std::int32_t int_value_1;
		std::int32_t int_value_2;
		auto [ptr_1, ec_1] = std::from_chars(value_1.data(), value_1.data() + value_1.size(), int_value_1);
		auto [ptr_2, ec_2] = std::from_chars(value_2.data(), value_2.data() + value_2.size(), int_value_2);

		if( (flag_1 != "-w" && flag_1 != "-h")
			|| (flag_2 != "-w" && flag_2 != "-h")
			|| ec_1 != std::errc()
			|| ec_2 != std::errc() )
		{
			status = error::parser_status::invalid;
		}
		else
		{
			size.width = (flag_1 == "-w") ? int_value_1 : int_value_2;
			size.height = (flag_1 == "-w") ? int_value_2 : int_value_1;
			status = error::parser_status::ok;
		}
	}

	return { status, size };
}


bool is_valid_options(error::parser_status status)
{
	return (status == error::parser_status::ok) || (status == error::parser_status::need_use_default);
}


bool need_use_default_settings(error::parser_status status)
{
	return (status == error::parser_status::need_use_default);
}

} // namespace tt_program
