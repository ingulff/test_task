#ifndef TT_VALIDATION_HELPERS_HPP
#define TT_VALIDATION_HELPERS_HPP

#include "settings/options_config.hpp"

namespace tt_program::utils
{

inline bool is_checksum_option(const std::string_view option)
{
	using tt_program::settings::checksum_mode_values;

	auto pos = std::find( std::cbegin(checksum_mode_values), std::cend(checksum_mode_values), option );

	return pos != std::cend(checksum_mode_values);
}

inline bool is_countable_option(const std::string_view option)
{
	using tt_program::settings::count_mode_values;

	auto pos = std::find( std::cbegin(count_mode_values), std::cend(count_mode_values), option );

	return pos != std::cend(count_mode_values);
}

inline bool is_valid_task(const tt_program::task::task_t & task)
{
    return !std::holds_alternative<std::monostate>(task.m_task);
}


} // namespace tt_program::utils

#endif // TT_VALIDATION_HELPERS_HPP
