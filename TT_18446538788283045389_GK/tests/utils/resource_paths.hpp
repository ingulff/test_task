#ifndef TT_RESOURCE_PATHS_HPP
#define TT_RESOURCE_PATHS_HPP

#include <string>

namespace tt_tests
{

#ifndef RESOURCE_DIR

#define RESOURCE_DIR "."

#endif // RESOURCE_DIR

constexpr std::string_view nonexistent_file     = RESOURCE_DIR"/nonexistent.txt";
constexpr std::string_view empty_file           = RESOURCE_DIR"/empty.txt";
constexpr std::string_view one_value_file       = RESOURCE_DIR"/test_1.txt";
constexpr std::string_view large_file           = RESOURCE_DIR"/test_2.txt";
constexpr std::string_view main_file            = RESOURCE_DIR"/test_3.txt";
constexpr std::string_view boundary_case_file   = RESOURCE_DIR"/test_4.txt";
constexpr std::string_view boundary_case_file_1 = RESOURCE_DIR"/test_4.txt";

}

#endif // TT_RESOURCE_PATHS_HPP
