#ifndef TT_HANDLE_RESPONSE_HPP
#define TT_HANDLE_RESPONSE_HPP

#include <sstream>

#include "error/http_error.hpp"
#include "utils/file_generators.hpp"
#include "utils/stream_crc32.hpp"

namespace tt_tests::utils
{

inline error::http_errc expected_errc;
inline bool is_valid_file = false;
inline bool is_reseived = false;


} // tt_tests::utils

void handle_response_body(std::string response, std::uint32_t errc)
{
    std::istringstream stream{std::move(response), std::ios::binary};
        
    auto handler = tt_tests::make_stream_crc32();
    auto hash = handler->calc_crc32(stream);

    REQUIRE(tt_tests::utils::expected_errc == error::to_http_errc(errc));
    REQUIRE(tt_tests::utils::is_valid_file == tt_tests::upload_files_hashs.count(hash));
    tt_tests::utils::is_reseived = true;
}

void handle_response_error(std::string, std::uint32_t errc)
{
    REQUIRE(tt_tests::utils::expected_errc == error::to_http_errc(errc));
    tt_tests::utils::is_reseived = true;
}

#endif // TT_HANDLE_RESPONSE_HPP
