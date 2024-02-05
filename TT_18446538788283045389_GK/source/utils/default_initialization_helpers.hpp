#ifndef TT_DEFAULT_INITIALIZATION_HELPERS_HPP
#define TT_DEFAULT_INITIALIZATION_HELPERS_HPP

#include <iterator>

#include "settings/cli_options.hpp"
#include "settings/cli_options_parser.hpp"
#include "settings/options_config.hpp"
#include "settings/options_validator.hpp"

namespace tt_program::utils
{

#ifdef WORKER_BUFFER_SIZE

constexpr std::uint32_t default_buffer_size = WORKER_BUFFER_SIZE;

#else

// to make testing easier 
constexpr std::uint32_t default_buffer_size = 1024;

#endif // WORKER_BUFFER_SIZE

inline void config_default_parser(tt_program::settings::cli_options_parser & parser, tt_program::settings::options_t & options)
{
	using option_type = tt_program::settings::cli_options_parser::option_type;

	using tt_program::settings::help_option_flags;
	using tt_program::settings::filepath_option_flags;
	using tt_program::settings::mode_option_flags;
	using tt_program::settings::additional_option_flags;

	parser.add_option( { std::cbegin(help_option_flags), std::cend(help_option_flags) }, &(options.need_show_help_msg), option_type::non_parametric );
	parser.add_option( { std::cbegin(filepath_option_flags), std::cend(filepath_option_flags) }, &(options.filepath), option_type::parametric );
	parser.add_option( { std::cbegin(mode_option_flags), std::cend(mode_option_flags) }, &(options.mode), option_type::parametric);
	parser.add_option( { std::cbegin(additional_option_flags), std::cend(additional_option_flags) }, &(options.mode_value), option_type::parametric);
}


inline void config_default_validator(tt_program::settings::opt_validat_uniqu_ptr & validator)
{
	using tt_program::settings::help_option_flags;
	using tt_program::settings::filepath_option_flags;
	using tt_program::settings::mode_option_flags;
	using tt_program::settings::additional_option_flags;
	using tt_program::settings::checksum_mode_values;
	using tt_program::settings::count_mode_values;

	for(const auto & flag : help_option_flags)
	{
		validator->add_option( std::string(flag), std::string(flag), {} );
	}

	for(const auto & flag : filepath_option_flags)
	{
		validator->add_option( std::string(flag), std::string(flag), {} );
	}

	for(const auto & mode : checksum_mode_values)
	{
		for(const auto & flag : mode_option_flags)
		{
			validator->add_option( std::string(mode), std::string(flag), {} );
		}
	}

	for(const auto & mode : count_mode_values)
	{
		for(const auto & flag : mode_option_flags)
		{
			validator->add_option( std::string(mode), std::string(flag), { std::cbegin(additional_option_flags), std::cend(additional_option_flags) } );
		}
	}
}

} // namespace tt_program::utils

#endif // TT_DEFAULT_INITIALIZATION_HELPERS_HPP