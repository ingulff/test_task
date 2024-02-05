/**
 *  ᛝ
 */

#include "error.hpp"

namespace error
{

int to_int(errc error_code) noexcept
{
	return static_cast<int>(error_code);
}

} // namespace error
