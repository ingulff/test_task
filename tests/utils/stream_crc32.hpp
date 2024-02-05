#ifndef TT_STREAM_CRC32_HPP
#define TT_STREAM_CRC32_HPP

#include <istream>
#include <memory>
#include <string>

namespace tt_tests
{

class stream_crc32_interface
{
public:

    std::uint32_t calc_crc32(std::istream & stream);

    std::uint32_t calc_crc32(const std::string & filename);

private:

    virtual std::int32_t calc_crc32_impl(std::istream & stream) = 0;
};

using stream_crc32_interface_t = std::shared_ptr<stream_crc32_interface>;

stream_crc32_interface_t make_stream_crc32();

} // namespace tt_tests

#endif // TT_STREAM_CRC32_HPP

