#ifndef TT_ERROR_HPP
#define TT_ERROR_HPP

#include <cstdint>

namespace error
{

enum class status_code : std::int8_t
{
	active = 0x0000,
	invalid = 0x0001,
	stopped = 0x0002,
	paused = 0x0003,
};


enum class parser_status : std::int8_t
{
	ok = 0x0000,
	invalid = 0x0001,
	need_use_default = 0x0002,
};

enum class errc
{
	ok = 0x00000000,
	unknown = 0x00000001,
	invalid_initialization = 0x00000002,
	invalid_cli_parsing = 0x00000003,
};

enum error::errc to_errc(status_code status) noexcept;

enum error::errc to_errc(parser_status status) noexcept;

int to_int(errc error_code) noexcept;

} // namespace error

#endif // TT_ERROR_HPP
