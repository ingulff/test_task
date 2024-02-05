#ifndef TT_TASK_HPP
#define TT_TASK_HPP

#include <memory>
#include <string>
#include <variant>

#include "settings/cli_options.hpp"
namespace tt_program::task
{

struct print_help_msg_t
{};

struct calculate_checksum_t
{
	std::string filepath;
};

struct count_numbers_of_value_t
{
	std::string filepath;
	std::string pattern;
};

using task_variant = std::variant<std::monostate, print_help_msg_t, calculate_checksum_t, count_numbers_of_value_t>;

class task_t
{

public:
	task_t();

	explicit task_t(task_variant task)
		: m_task(std::move(task))
	{}

public:

	task_variant m_task;
};


/// @warning options should be validated before creating a task
task_t make_task(tt_program::settings::options_t & options);

} // namespace tt_program::task

#endif // TT_TASK_HPP
