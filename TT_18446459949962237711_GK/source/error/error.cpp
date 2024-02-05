/**
 *  ᛝ
 */

#include "error.hpp"
#include "http_error.hpp"

namespace error
{

int to_int(errc error_code) noexcept
{
	return static_cast<int>(error_code);
}

int to_int(http_errc error_code)
{
    return static_cast<int>(error_code);
}

} // namespace error