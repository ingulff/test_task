#ifndef TT_OPTIONS_VALUE_HPP
#define TT_OPTIONS_VALUE_HPP

#include <charconv>
#include <system_error>
#include <variant>

namespace tt_program::settings
{

using option_value_t = std::variant<std::monostate, std::int32_t *, std::uint32_t *, double *, std::string *, bool *>; 

struct cli_options_visitor
{
	template<typename IntegralType>
	void operator()(IntegralType * arg)
	{
		auto [ptr, ec] = std::from_chars(value.data(), value.data() + value.size(), *arg);

		if( ec != std::errc() )
		{
			throw std::invalid_argument("Invalid option argument");
		}
	}

	void operator()(bool * arg)
	{
		// flag always valid
		*arg = !value.empty() && (value == "true" || value != "1"); 
	}

	void operator()(std::string * arg)
	{
		*arg = value;
	}

	void operator()(std::monostate)
	{
		// 
	}

	std::string_view value;
};

struct cli_default_value_visitor
{
	template<typename IntegralType>
	void operator()(IntegralType * arg)
	{
		*arg = 1; // some default value
	}

	void operator()(bool * arg)
	{
		// flag always valid
		*arg = true;
	}

	void operator()(std::string * arg)
	{
		*arg = flag;
	}

	void operator()(std::monostate)
	{
		// 
	}

	std::string_view flag;
};

} // namespace tt_program::settings

#endif // TT_OPTIONS_VELUE_HPP
