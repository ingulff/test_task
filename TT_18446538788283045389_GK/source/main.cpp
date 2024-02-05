#include <exception>
#include <ios>
#include <iostream>
#include <memory>

#include "command_worker.hpp"
#include "error.hpp"
#include "settings/cli_options.hpp"
#include "settings/cli_options_parser.hpp"
#include "settings/options_validator.hpp"
#include "utils/default_initialization_helpers.hpp"
#include "utils/io_helpers.hpp"

int main(int argc, char **argv)
{
	tt_program::settings::options_t options;
	
	tt_program::settings::cli_options_parser options_parser;

	auto validator = tt_program::settings::make_unique_option_validator();

    tt_program::command_worker worker{ std::cout };

	int res = error::to_int(error::errc::ok);

	try
	{
		tt_program::utils::config_default_parser(options_parser, options);
		options_parser.parse(argc, argv);

		tt_program::utils::config_default_validator(validator);
		if( validator->is_options_valid(options) == true )
		{
			auto task = tt_program::task::make_task(options);

			worker.process(task);
		}
		else
		{
			tt_program::utils::print_message(std::cerr, "Invalid optional configuration\n");
			tt_program::utils::print_message(std::cout, tt_program::settings::help_message);

			res = error::to_int(error::errc::invalid_config);
		}
		
	}
	catch(std::invalid_argument ex)
	{
		tt_program::utils::print_message(std::cerr, ex.what());
		tt_program::utils::print_message(std::cout, tt_program::settings::help_message);

		res = error::to_int(error::errc::invalid_argument);
	}
	catch(std::ios_base::failure ex)
	{
		tt_program::utils::print_message(std::cerr, ex.what());
		tt_program::utils::print_message(std::cout, tt_program::settings::help_message);

		res = error::to_int(error::errc::file_operation_abort);
	}
	catch(...)
	{
		tt_program::utils::print_message(std::cerr, "Unknown exception caught\n");

		res = error::to_int(error::errc::unknown);
	}

	return res;
}