#ifndef TT_FILESYSEM_HELPERS_HPP
#define TT_FILESYSEM_HELPERS_HPP

#include <filesystem>
#include <string>


namespace tt_program::utils
{

void throw_if_no_exist(const std::string & path);
void throw_if_no_exist(const std::filesystem::path & entry);

void throw_if_no_directory(const std::string & dir_path);
void throw_if_no_directory(const std::filesystem::path & dir_path);

void throw_if_no_regular_file(const std::string & path);
void throw_if_no_regular_file(const std::filesystem::path & file );

}

#endif // TT_FILESYSEM_HELPERS_HPP