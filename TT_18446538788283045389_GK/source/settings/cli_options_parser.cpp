/**
 *  ᛝ
 */
#include <exception>
#include <stdexcept>
#include <unordered_map>

#include "settings/cli_options_parser.hpp"

namespace tt_program::settings
{

class cli_options_parser::cli_parser_impl
{
public:
	cli_parser_impl() = default;

	~cli_parser_impl() = default;

	cli_parser_impl(cli_parser_impl & other) = delete;

	cli_parser_impl & operator=(cli_parser_impl & other) = delete;

	cli_parser_impl(cli_parser_impl && other) = default;

	cli_parser_impl & operator=(cli_parser_impl && other) = default;

public:

	void parse(int argc, char ** argv)
	{
		if(argc < 2)
		{
			throw std::invalid_argument("Too few parameters to run");
		}

		for(int i = 1; i < argc; ++i)
		{
			std::string flag = argv[i];
			std::string value;

			if( is_parametric_option(flag) )
			{
				if( auto equal_pos = flag.find('=');  equal_pos != std::string::npos )
				{
					value = flag.substr(equal_pos + 1);
					flag = flag.substr(0, equal_pos);
				}
				else if(argc > i + 1)
				{
					value = argv[++i];
				}

				m_params_visitor.value = value;
				std::visit(m_params_visitor, m_parametric_options[flag]);
			}
			else if( is_non_parametric_option(flag) )
			{
				
				m_defaule_visitor.flag = flag;
				std::visit(m_defaule_visitor, m_non_parametric_options[flag]);
			}
			else
			{
				std::string error_msg{"Unknown option: "};
				error_msg += flag;

				throw std::invalid_argument(error_msg);
			}

		}
	}

	void add_option(const std::vector<std::string_view> & flags, option_value_t value, option_type parametric_policy) noexcept
	{
		for(auto & flag : flags)
		{
			if(parametric_policy == option_type::parametric)
			{
				m_parametric_options.emplace(std::make_pair(std::string(flag), value));
			}
			else if(parametric_policy == option_type::non_parametric)
			{
				m_non_parametric_options.emplace(std::make_pair(std::string(flag), value));
			}

		}
	}

	void clear() noexcept
	{
		m_parametric_options.clear();
		m_non_parametric_options.clear();
	}

private:
	bool is_parametric_option(const std::string & flag)
	{
		return m_parametric_options.count(flag) != 0;
	}

	bool is_non_parametric_option(const std::string & flag)
	{
		return m_non_parametric_options.count(flag) != 0;
	}

private:
	std::unordered_map<std::string, option_value_t> m_parametric_options;
	std::unordered_map<std::string, option_value_t> m_non_parametric_options;
	
	cli_options_visitor                             m_params_visitor;
	cli_default_value_visitor						m_defaule_visitor;
};


cli_options_parser::cli_options_parser()
	: m_parser_impl()
{
	m_parser_impl = std::make_unique<cli_parser_impl>();
}

cli_options_parser::~cli_options_parser() = default;

cli_options_parser::cli_options_parser(cli_options_parser && other) = default;

cli_options_parser & cli_options_parser::operator= (cli_options_parser && other) = default;


void cli_options_parser::parse(int argc, char ** argv)
{
	m_parser_impl->parse(argc, argv);
}


void cli_options_parser::add_option(const std::vector<std::string_view> & flags, option_value_t value, option_type parametric_policy) noexcept
{
	m_parser_impl->add_option(flags, value, parametric_policy);
}

void cli_options_parser::clear() noexcept
{
	m_parser_impl->clear();
}

} // namespace tt_program::settings
