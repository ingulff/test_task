/**
 *  ᛝ
 */

#include <stdexcept>
#include <string_view>

#include <cstdint>

#include "settings.hpp"

namespace tt_program
{

#ifdef HOST_IP
static constexpr std::string_view host_ip = HOST_IP;
#else
static constexpr std::string_view host_ip = "127.0.0.1";
#endif // HOST_IP

#ifdef HOST_PORT
static constexpr std::uint16_t port = HOST_PORT;
#else
static constexpr std::uint16_t port = 1234;
#endif // HOST_PORT

#ifdef THREADS_COUNT
static constexpr std::uint16_t threads_number = THREADS_COUNT;
#else
static constexpr std::uint16_t threads_number = 4;
#endif // THREADS_COUNT

connect_t make_settings(int argc, char **argv)
{
    if(argc < 3)
    {
        throw std::logic_error("ERROR: invalid arguments");
    }

    connect_t settings;
    settings.host = host_ip;
    settings.port = port;
    settings.threads_count = threads_number;
    settings.upload_path = argv[1];
    settings.auth_tokens_path = argv[2];

    return settings;
}

} // namespace tt_program


