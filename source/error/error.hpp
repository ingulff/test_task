#ifndef TT_ERROR_HPP
#define TT_ERROR_HPP

namespace error
{

enum class errc
{
	ok                   = 0x0,
	unknown              = 0x1,
	invalid_argument     = 0x2,
	invalid_config       = 0x3,
	cant_send_http       = 0x4, // @todo delete this
	file_operation_abort = 0x5,
};

int to_int(errc error_code) noexcept;

} //namespace error

#endif // TT_ERROR_HPP