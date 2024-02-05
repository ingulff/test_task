/**
 *  ᛝ
 */

#include <filesystem>
#include <string>

#include "filesystem_helpers.hpp"

namespace tt_program::utils
{

void throw_if_no_exist(const std::filesystem::path & entry)
{
    if(std::filesystem::exists(entry) == false)
    {
        std::string msg{"ERROR: file or directory "};
        msg += entry.string();
        msg += " not exist\n";
        throw std::logic_error(msg);
    }
}

void throw_if_no_exist(const std::string & path)
{
    std::filesystem::path entry{path};

    throw_if_no_exist(entry);
}


void throw_if_no_directory(const std::filesystem::path & dir_path)
{
    if(std::filesystem::is_directory(dir_path) == false)
    {
        std::string msg{"ERROR: source_dir path: "};
        msg += dir_path.string();
        msg += " not directory\n";
        throw std::logic_error(msg);
    }
}

void throw_if_no_directory(const std::string & dir_path)
{
    std::filesystem::path entry{dir_path};

    throw_if_no_directory(entry);
}


void throw_if_no_regular_file(const std::filesystem::path & file )
{
    if(std::filesystem::is_regular_file(file) == false)
    {
        std::string msg("ERROR: ");
        msg += file.string();
        msg += " not regular file\n";
        throw std::logic_error(msg);
    }
}

void throw_if_no_regular_file(const std::string & path)
{
    std::filesystem::path file{path};
    throw_if_no_regular_file(file);
}

} // namespace tt_program::utils

