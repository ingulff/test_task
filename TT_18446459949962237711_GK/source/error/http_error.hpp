#ifndef TT_HTTP_ERROR_HPP
#define TT_HTTP_ERROR_HPP

#include <cstdint>
#include <type_traits>

namespace error
{

enum class http_errc : std::int16_t
{
    unknown = 1,
    successs = 200,
    moved_permanently = 301,
    bad_request = 400,
    unautorized = 401, // if you change this enum also change to_http_error
    not_found = 404,
};


int to_int(http_errc errc);


template<typename Integer, std::enable_if_t<std::is_integral_v<Integer>, bool> = true>
http_errc to_http_errc(Integer error_code)
{
    http_errc errc;
    switch (error_code)
    {
        case 200:
            errc = http_errc::successs;
            break;

        case 301:
            errc = http_errc::moved_permanently;
            break;

        case 400:
            errc = http_errc::bad_request;
            break;

        case 401:
            errc = http_errc::unautorized;
            break;

        case 404:
            errc = http_errc::not_found;
            break;
    
        default:
            errc = http_errc::unknown;
            break;
    }

    return errc;
}

template<typename NonInteger, std::enable_if_t<!std::is_integral_v<NonInteger>, bool> = true>
inline http_errc to_http_errc(NonInteger error_code)
{
    return http_errc::unknown;
}

} // namespace error

#endif // TT_HTTP_ERROR_HPP
