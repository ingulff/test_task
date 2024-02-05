#ifndef TT_IO_HELPERS_HPP
#define TT_IO_HELPERS_HPP

#include <ostream>
#include <string>

namespace tt_program::utils
{

inline void print_message(std::ostream & out_stream, const std::string_view help_msg)
{
	out_stream << help_msg << std::endl;
}

class dummy_streambuf final : public std::streambuf
{
public:
	int overflow(int c) { return c; }
};

inline dummy_streambuf dummy_buffer;
inline auto dummy_osteam = std::ostream(&dummy_buffer);

} // namespace tt_program::utils

#endif // TT_IO_HELPERS_HPP
