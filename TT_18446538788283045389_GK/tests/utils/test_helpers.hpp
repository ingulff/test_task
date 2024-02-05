#ifndef TT_TEST_HELPERS_HPP
#define TT_TEST_HELPERS_HPP

#include <array>
#include <fstream>
#include <ios>
#include <memory>

#include <cstdint>

#include "settings/cli_options_parser.hpp"
#include "settings/cli_options.hpp"
#include "settings/options_validator.hpp"

#include "utils/utils.hpp"

namespace tt_tests
{

inline void clear_options(tt_program::settings::options_t & options)
{
	options.need_show_help_msg = false;
	options.filepath.clear();
	options.mode.clear();
	options.mode_value.clear();
}


inline void initialize_parser_with_options(tt_program::settings::cli_options_parser & parser, tt_program::settings::options_t & options)
{
	utils::config_default_parser(parser, options);
}


inline void initialize_validator_with_default_config(std::unique_ptr<tt_program::settings::option_validator_interface> & validator)
{
	utils::config_default_validator(validator);
}


inline std::uint32_t cals_words_in_file(std::string filepath)
{
	std::uint32_t sum = 0x0;

	std::ifstream file(filepath);

	if( file.is_open() == true )
	{
		for( std::string line; file >> line; )
		{
			++sum;
		}
	}

	return sum;
}


inline std::uint32_t cals_words_in_file(std::string filepath, std::string pattern)
{
	std::uint32_t sum = 0x0;

	std::ifstream file(filepath);

	if( file.is_open() == true )
	{
		for( std::string line; file >> line; )
		{
			if(line == pattern)
			{
				++sum;
			}
		}
	}

	return sum;
}


inline std::uint32_t calc_checksum_of_file(std::string filepath)
{
	std::uint32_t checksum = 0x0;

	std::ifstream file(filepath, std::ios::binary | std::ios::ate);

	if( file.is_open() )
	{
		std::uint32_t file_size = file.tellg();

		file.seekg(0, std::ios::beg);

		const std::uint32_t buff_size = tt_program::utils::default_buffer_size;
		std::vector<char> buffer(buff_size, 0x0);

		std::uint32_t read_size = 0x0;

		while( read_size < file_size )
		{	try
			{
				std::uint32_t next_chunk_size = std::min(buff_size, file_size - read_size);
				file.read(buffer.data(), next_chunk_size);

				std::uint32_t * int_buffer = reinterpret_cast<std::uint32_t *>( buffer.data() );
				std::uint32_t int_buf_size = next_chunk_size / sizeof(std::uint32_t);
				int_buf_size = (int_buf_size > 1) ? int_buf_size : 1;


				for(std::uint32_t i = 0x0; i < int_buf_size; ++i )
				{
					checksum += int_buffer[i];
				}

				for(auto elem : buffer)
				{
					elem = 0x0;
				}

				read_size += next_chunk_size;
			}
			catch(...)
			{
				checksum = 0x0;
				break;
			}
		}
	}

	return checksum;
}

} // namespace tt_tests


#endif // TT_TEST_HELPERS_HPP
