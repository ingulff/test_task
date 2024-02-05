/**
 *  ᛝ
 */

#include "stream_crc32.hpp"

#include <istream>
#include <fstream>
#include <ios>
#include <memory>
#include <utility>
#include <vector>

#include <cstdint>
#include <cstring>

#include <3rdparty/Crc32/Crc32.h>

namespace tt_tests
{

class stream_crc32 : public stream_crc32_interface
{
private:
    std::int32_t calc_crc32_impl(std::istream & stream) override final
    {
        constexpr std::uint32_t default_buffer_size = 1024;
        std::uint32_t crc_sum = 0x0;
        try
        {
            stream.seekg(0, std::ios::end);

            std::uint32_t file_size = stream.tellg();

		    stream.seekg(0, std::ios::beg);

            std::uint32_t max_chunk_size = default_buffer_size;
		    std::vector<char> buffer(max_chunk_size, 0x0);

            for( std::uint32_t read_file_size = 0x0, next_chunk_size = max_chunk_size; read_file_size < file_size; )
            {
                next_chunk_size = std::min(file_size - read_file_size, max_chunk_size);
                stream.read(buffer.data(), next_chunk_size);

                crc_sum = crc32_fast(buffer.data(), next_chunk_size, crc_sum);

                std::memset(buffer.data(), 0x0, next_chunk_size);

                read_file_size += next_chunk_size;
            }
        }
        catch(...)
        {
            return 0x0;
        }

        return crc_sum;
    }
};


std::uint32_t stream_crc32_interface::calc_crc32(std::istream & stream)
{
    return calc_crc32_impl(stream);
}

std::uint32_t stream_crc32_interface::calc_crc32(const std::string & filename)
{
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    file.exceptions(std::ifstream::badbit);
    
    return calc_crc32_impl(file);
}


stream_crc32_interface_t make_stream_crc32()
{
    return std::make_shared<stream_crc32>();
}

} // namespace tt_tests



