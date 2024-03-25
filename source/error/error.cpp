/**
 *  ᛝ
 */

#include "error/error.hpp"

namespace error
{

enum errc to_errc(status_code status) noexcept
{
	errc error;
	switch(status)
	{
	case status_code::active: // ?? (maybe it error)
	case status_code::stopped:
		error = errc::ok;
		break;
	case status_code::invalid:
		error = errc::invalid_initialization;
		break;
	default:
		error = errc::unknown;
	}

	return error;
}


enum errc to_errc(parser_status status) noexcept
{
	errc error;
	switch(status)
	{
	case parser_status::need_use_default:
	case parser_status::ok:
		error = errc::ok;
		break;
	case parser_status::invalid:
		error = errc::invalid_cli_parsing;
		break;
	default:
		error = errc::unknown;	 
	}

	return error;
}


int to_int(errc error_code) noexcept
{
	return static_cast<int>(error_code);
}

} // namespace error
