#ifndef TT_SETTINGS_HPP
#define TT_SETTINGS_HPP

#include <string>

#include <cstdint>

namespace tt_program
{

struct connect_t
{
std::string host;
std::uint16_t port;
std::uint16_t threads_count;
std::string upload_path;
std::string auth_tokens_path;
};

connect_t make_settings(int argc, char **argv);

} // namespace tt_program

#endif // TT_SETTINGS_HPP
