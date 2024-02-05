#ifndef TT_CLI_OPTION_PARSER_HPP
#define TT_CLI_OPTION_PARSER_HPP

#include <memory>
#include <string>
#include <vector>

#include <cstdint>

#include "settings/options_value.hpp"

namespace tt_program::settings
{

class cli_options_parser
{
public:	
	enum class option_type
	{
		non_parametric = 0x0,
		parametric     = 0x1,
	};

	cli_options_parser();

	~cli_options_parser();

	cli_options_parser(cli_options_parser & other) = delete;

	cli_options_parser & operator= (cli_options_parser & other) = delete;

	cli_options_parser(cli_options_parser && other);

	cli_options_parser & operator= (cli_options_parser && other);

public:
	void parse(int argc, char ** argv);

	void add_option(const std::vector<std::string_view> & flags, option_value_t value, option_type parametric_policy) noexcept;

	void clear() noexcept;

private:
	class cli_parser_impl;
	std::unique_ptr<cli_parser_impl> m_parser_impl;
};

} // namespace tt_program::settings

#endif // TT_CLI_OPTION_PARSER_HPP
