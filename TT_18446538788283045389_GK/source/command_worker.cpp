/**
 *  ᛝ
 */

#include <fstream>
#include <ios>
#include <ostream>
#include <string>
#include <vector>

#include <cstdint>
#include <cstring>

#include "command_worker.hpp"
#include "task.hpp"
#include "settings/options_config.hpp"
#include "utils/default_initialization_helpers.hpp"
#include "utils/io_helpers.hpp"

namespace tt_program
{

class command_worker::command_worker_impl
{
public:
	command_worker_impl()
		: m_filepath()
		, m_out_stream(tt_program::utils::dummy_osteam)
		, m_file()
	{
		m_out_stream.flush(); 
	}

	command_worker_impl(command_worker_impl & other) = delete;

	command_worker_impl & operator=(command_worker_impl & other) = delete;

	command_worker_impl(command_worker_impl && other) = default;

	command_worker_impl & operator=(command_worker_impl && other) = default;

	~command_worker_impl() = default;

	command_worker_impl(std::ostream & out_stream)
		: m_filepath()
		, m_out_stream(out_stream)
		, m_file()
	{
		m_out_stream.flush();
	}

public:
	
	void process(const tt_program::task::task_t & task)
	{
		process_impl(task);
	}

private:
	void process_impl(const tt_program::task::task_t & task)
	{
		std::visit([&task, this](const auto & command)
		{
			this->process_impl(command);
		}, task.m_task);
	}

	void process_impl(const tt_program::task::print_help_msg_t &)
	{
		tt_program::utils::print_message(m_out_stream, tt_program::settings::help_message);
	}

	void process_impl(const tt_program::task::calculate_checksum_t & command)
	{
		m_filepath = command.filepath;

		std::uint32_t checksum = calc_file_checksum();

		tt_program::utils::print_message(m_out_stream, std::to_string(checksum));
	}

	void process_impl(const tt_program::task::count_numbers_of_value_t & command)
	{
		m_filepath = command.filepath;
		std::string pattern = command.pattern;

		std::int32_t sum = 0x0;
		
		if( pattern.empty() == false )
		{
			sum = calc_some_value_in_file(pattern);
		}
		else
		{
			sum = calc_some_value_in_file();
		}

		tt_program::utils::print_message(m_out_stream, std::to_string(sum));
	}

	void process_impl(std::monostate)
	{

	}

private:
	std::uint32_t calc_file_checksum()
	{
		std::uint32_t checksum = 0x0;

		open_file(std::ios::binary | std::ios::ate);

		std::uint32_t file_size = m_file.tellg();

		m_file.seekg(0, std::ios::beg);

		std::uint32_t max_chunk_size = tt_program::utils::default_buffer_size;
		std::vector<char> buffer(max_chunk_size, 0x0);

		for( std::uint32_t read_file_size = 0x0, next_chunk_size = max_chunk_size; read_file_size < file_size; )
		{
			next_chunk_size = std::min(file_size - read_file_size, max_chunk_size);
			read_file_chunk(buffer, next_chunk_size);

			std::uint32_t * int_buffer = reinterpret_cast<std::uint32_t *>( buffer.data() );
			std::uint32_t int_buf_size = next_chunk_size / sizeof(std::uint32_t);
			int_buf_size = (int_buf_size > 1) ? int_buf_size : 1;

			for(std::uint32_t i = 0x0; i < int_buf_size; ++i )
			{
				checksum += int_buffer[i];
			}

			std::memset(buffer.data(), 0x0, next_chunk_size);

			read_file_size += next_chunk_size;
		}

		close_file();

		return checksum;
	}

	std::uint32_t calc_some_value_in_file(const std::string_view pattern)
	{
		std::uint32_t sum = 0x0;

		open_file(std::ios::in | std::ios::ate);

		std::uint32_t file_size = m_file.tellg();

		m_file.seekg(0, std::ios::beg);

		std::uint32_t max_chunk_size = tt_program::utils::default_buffer_size;

		std::vector<char> buffer(max_chunk_size, 0x0);

		auto pattern_pos = std::cbegin(pattern);
		std::uint32_t equal_sum = 0x0;
		for( std::uint32_t read_file_size = 0x0, next_chunk_size = max_chunk_size; read_file_size < file_size; )
		{
			next_chunk_size = std::min(file_size - read_file_size, max_chunk_size);
			read_file_chunk(buffer, next_chunk_size);

			for(std::uint32_t buf_pos = 0; buf_pos < next_chunk_size; ++buf_pos)
			{
				if( buffer[buf_pos] == '\0' )
				{
					break;

				}

				if( std::isspace(static_cast<unsigned char>(buffer[buf_pos])) > 0 )  // not space
				{
					if( equal_sum == pattern.size() )
					{
						++sum;
					}

					equal_sum = 0x0;
					pattern_pos = std::cbegin(pattern);
				}
				else
				{
					if( std::cend(pattern) != pattern_pos )
					{
						if( buffer[buf_pos] == *pattern_pos )
						{
							++equal_sum;
							++pattern_pos;
						}
						else
						{
							equal_sum = 0x0;
							pattern_pos = std::cend(pattern); // stop comparison for this word
						}

					}
					else
					{
						equal_sum = 0x0;
						pattern_pos = std::cend(pattern);
					}
				}
			}

			std::memset(buffer.data(), 0x0, next_chunk_size);

			read_file_size += next_chunk_size;
		}

		if( equal_sum == pattern.size() )
		{
			++sum;

			equal_sum = 0x0;
			pattern_pos = std::cbegin(pattern);
		}

		close_file();

		return sum;
	}

	std::uint32_t calc_some_value_in_file()
	{
		std::uint32_t sum = 0x0;

		open_file(std::ios::in | std::ios::ate);

		std::uint32_t file_size = m_file.tellg();

		m_file.seekg(0, std::ios::beg);

		std::uint32_t max_chunk_size = tt_program::utils::default_buffer_size;
		
		std::vector<char> buffer(max_chunk_size, 0x0);
		
		bool inside_in_value = false; // word, sentence, etc
		for( std::uint32_t read_file_size = 0x0, next_chunk_size = max_chunk_size; read_file_size < file_size; )
		{
			next_chunk_size = std::min(file_size - read_file_size, max_chunk_size);
			read_file_chunk(buffer, next_chunk_size);

			for( std::uint32_t pos = 0x0; pos < next_chunk_size; ++pos )
			{
				if( std::isspace(static_cast<unsigned char>(buffer[pos])) > 0 )
				{
					if( inside_in_value == true )
					{
						++sum;
						inside_in_value = false;
					}
				}
				else
				{
					inside_in_value = true;
				}
			}

			std::memset(buffer.data(), 0x0, next_chunk_size);

			read_file_size += next_chunk_size;
		}

		if( inside_in_value == true )
		{
			++sum;
		}

		close_file();

		return sum;
	}

private:
	void open_file(std::ios_base::openmode mode)
	{
		if( m_filepath.empty() == true )
		{
			throw std::ios_base::failure("Error: empty fileanme");
		}

		m_file.open(m_filepath, mode);
		m_file.exceptions(std::ifstream::badbit);

		if( m_file.is_open() == false )
		{
			std::string error_msg { "Can't open file: " };
			error_msg += m_filepath;

			close_file();
			throw std::ios_base::failure(error_msg);
		}
	}

	void close_file()
	{
		if( m_file.is_open() )
		{
			m_file.close();
		}
	}

	void read_file_chunk(std::vector<char> & buffer, std::uint32_t file_chunk_size)
	{
		m_file.read(buffer.data(), file_chunk_size);
	}

private:
	std::string m_filepath;

	std::ostream & m_out_stream;

	std::ifstream m_file;
};


command_worker::command_worker()
	: m_worker_impl()
{
	m_worker_impl = std::make_unique<command_worker_impl>();
}

command_worker::~command_worker() = default;

command_worker::command_worker(command_worker && other) = default;

command_worker & command_worker::operator=(command_worker && other) = default;

command_worker::command_worker(std::ostream & out_stream)
	: m_worker_impl()
{
	m_worker_impl = std::make_unique<command_worker_impl>(out_stream);
}


void command_worker::process(const tt_program::task::task_t & task)
{
	m_worker_impl->process(task);
}

} // namespace tt_program

