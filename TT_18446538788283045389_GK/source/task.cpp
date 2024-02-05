/**
 *  ᛝ
 */

#include "task.hpp"
#include "utils/validation_helpers.hpp"

namespace tt_program::task
{

task_t::task_t()
	: m_task()
{}

task_t make_task(tt_program::settings::options_t & options)
{
	namespace utils = tt_program::utils;

	if( options.need_show_help_msg == true )
	{
		return task_t{ { print_help_msg_t{} } };
	}

	if( utils::is_checksum_option(options.mode) )
	{
		calculate_checksum_t task;
		task.filepath = std::move(options.filepath);

		return task_t { task };
	}

	if( utils::is_countable_option(options.mode) )
	{
		count_numbers_of_value_t task;
		task.filepath = std::move(options.filepath);

		if( options.mode_value.empty() == false )
		{
			task.pattern = std::move(options.mode_value);
		}

		return task_t { task };
	}

	return {};
}

} // namespace tt_program::task
