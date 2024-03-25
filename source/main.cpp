/**
 *  ᛝ
 */

#include <cstdlib>

#include "application.hpp"
#include "cli_options_parser.hpp"

int main(int argc, char ** argv)
{
	auto [status, board_settings] = tt_program::parse_cli_options(argc, argv);
	if( ! tt_program::is_valid_options(status) )
	{

		return error::to_int( error::to_errc(status) );
	}

	tt_program::application_t application;
	if( tt_program::need_use_default_settings( status ) )
	{
		application.initialize();
	}
	else
	{
		application.initialize(board_settings.width, board_settings.height );
	}

	return application.exec();
}