#ifndef TT_OPTIONS_CONFIG_HPP
#define TT_OPTIONS_CONFIG_HPP

#include <array>
#include <iterator>
#include <string>
#include <vector>

#include "settings/options_value.hpp"

namespace tt_program::settings
{

static constexpr std::string_view help_message = R"(Usage:

  program [options]

  Example:

  test -f Test.tst -m words -v mother
  test -f Test.tst -m checksum
  test -h

  Options:

  -f <path-to-file>     = Specifies a path to the resource file.
  -m <mode name>        = Specifies operation to be performer with file specified -f option.
  -v <value>            = Specifies value for some operation of -m option.
  -h                    = Show help message)";


constexpr std::array<std::string_view, 1> help_option_flags{ "-h" };
constexpr std::array<std::string_view, 1> filepath_option_flags{ "-f" };
constexpr std::array<std::string_view, 1> mode_option_flags{ "-m" };
constexpr std::array<std::string_view, 1> additional_option_flags{ "-v" };

constexpr std::array<std::string_view, 1> checksum_mode_values{ "checksum" };
constexpr std::array<std::string_view, 1> count_mode_values{ "words" };

} // namespace tt_program::settings

#endif // TT_OPTIONS_CONFIG_HPP
