/**
 *  ᛝ
 */


#include <filesystem>
#include <fstream>
#include <map>
#include <random>
#include <set>
#include <stdexcept>
#include <string>
#include <system_error>
#include <vector>
#include <unordered_set>

#include <cstdint>

#include "utils/file_generators.hpp"
#include "stream_crc32.hpp"
#include "utils/filesystem_helpers.hpp"
#include "utils/files_prepare.hpp"
namespace tt_tests
{

static void append_uploads_targets_files(std::vector<std::string> & paths, const std::filesystem::path & path, const std::string_view & root_path)
{
    for( const auto & elem : std::filesystem::directory_iterator(path) )
    {
        if(std::filesystem::is_regular_file(elem))
        {
            std::string target_path = {"/"};
            target_path += 
            paths.emplace_back(elem.path().string().substr(root_path.size()));
            continue;
        }

        append_uploads_targets_files(paths, elem, root_path);
    }
}

static void append_uploads_targets_files(std::vector<std::string> & paths, const std::string_view & resource_dirs, const std::string_view & root_path)
{
    std::filesystem::path directory{resource_dirs};

    append_uploads_targets_files(paths, directory, root_path);
}

static void append_system_files(std::vector<std::string> & paths, const std::filesystem::path & path)
{
    for( const auto & elem : std::filesystem::directory_iterator(path) )
    {
        if(std::filesystem::is_regular_file(elem))
        {
            paths.emplace_back(elem.path().string());
            continue;
        }

        append_system_files(paths, elem);
    }
}

static void append_system_files(std::vector<std::string> & paths, const std::string_view & resource_dirs)
{
    std::filesystem::path directory{resource_dirs};

    append_system_files(paths, directory);
}


static void create_directory(const std::filesystem::path path)
{
    std::filesystem::create_directory(path);
}

static void create_directory(std::string_view path)
{
    std::filesystem::path entry{path};
    tt_tests::create_directory(entry);
}


void generate_uploads_targets_filepaths(const std::string_view & dir_path)
{
    upload_target_filepaths.clear();

    std::filesystem::path entry{dir_path};

    tt_program::utils::throw_if_no_exist(entry);
    tt_program::utils::throw_if_no_directory(entry);

    append_uploads_targets_files(upload_target_filepaths, entry, dir_path);
}

void generate_upload_filepaths(const std::string_view & dir_path)
{
    upload_filepaths.clear();
    
    std::filesystem::path entry{dir_path};

    tt_program::utils::throw_if_no_exist(entry);
    tt_program::utils::throw_if_no_directory(entry);

    append_system_files(upload_filepaths, entry);
}

void generate_download_filepaths(const std::string_view & dir_path)
{
    download_filepaths.clear();
    
    if(  upload_filepaths.empty() == true )
    {
        generate_upload_filepaths(upload_resource_dir);
    }

    std::filesystem::path entry{dir_path};

    tt_tests::create_directory(entry);

    tt_program::utils::throw_if_no_exist(entry);
    tt_program::utils::throw_if_no_directory(entry);

    append_system_files(download_filepaths, entry);
}


static void insert_fil_hash(std::unordered_set<std::uint32_t> & hash_set, std::int32_t hash)
{
    hash_set.insert(hash);
}


static void generate_file_hash(const std::filesystem::path entry)
{
    tt_program::utils::throw_if_no_exist(entry);
    tt_program::utils::throw_if_no_regular_file(entry);

    auto hasher = make_stream_crc32();
    auto hash = hasher->calc_crc32(entry.string());
    insert_fil_hash(upload_files_hashs, hash);
}

static void generate_file_hash(const std::string & file_path)
{
    std::filesystem::path file{file_path};
    generate_file_hash(file);
}

void generate_upload_file_hashs(const std::string_view & dir_path)
{
    tt_program::utils::throw_if_no_exist(dir_path);

    if(upload_filepaths.empty() == true)
    {
        generate_upload_filepaths(dir_path);
    }

    for(const auto & path : upload_filepaths )
    {
        generate_file_hash(path);
    }
}


std::error_code clear_downloading_directory()
{
    std::filesystem::path dir{download_resourse_dir};
    std::error_code error;
    std::filesystem::remove_all(dir, error);

    return error;
}

static constexpr std::int64_t token_gate_size = 6000; // gate is segment - {left..right}
static constexpr std::int64_t token_test_count = 1000;

static std::map<std::uint64_t, std::uint64_t> gates;

void generate_config_gates(const std::string_view & filepath)
{
    std::filesystem::path entry(filepath);
    bool exist = std::filesystem::exists(entry);
    bool is_reg_file = std::filesystem::is_regular_file(entry);
    if(exist && is_reg_file)
    {
        return;
    }
    
    // Seed with a real random value, if available
    std::random_device r;

    // Choose a random mean between 1 and 6
    std::default_random_engine e1(r());
    std::uniform_int_distribution<std::uint64_t> uniform_dist(1, 700'000ull);
    for(uint64_t mean = uniform_dist(e1); 
        auth_tokens.size() < token_gate_size; 
        mean = uniform_dist(e1))
    {
        auth_tokens.insert(mean);
    }
    
    bool is_right_bound = false;
    std::uint64_t left, right;
    for(auto bound : auth_tokens)
    {
        if(is_right_bound == true)
        {
            right = bound;
            gates.emplace(std::make_pair(right, left)); // sort segments by right bouns
        }
        else
        {
            left = bound;
        }

        is_right_bound = !is_right_bound;
    }

    auth_tokens.clear();

    std::ofstream file( entry );
    if( file.is_open() == false )
    {
        std::string error_msg { "Can't open file: " };
        error_msg += filepath;

        file.close();
        throw std::ios_base::failure(error_msg);
    }
    for(auto & [right, left]: gates)
    {
        file << left << " " << right << std::endl;
    }

    file.close();
}


void generate_tests_tokens(const std::string_view & filepath)
{    
    std::filesystem::path entry(filepath);
    bool exist = std::filesystem::exists(entry);
    bool is_reg_file = std::filesystem::is_regular_file(entry);
    if(exist && is_reg_file && gates.empty())
    {
        std::ifstream file(entry);
        bool is_right_bound = false;
        for(std::int64_t left, right, bound; file >> bound;)
        {
            if(is_right_bound)
            {
                right = bound;
                gates.emplace(std::make_pair(right, left));  // sort segments by right bouns
            }
            else
            {
                left = bound;
            }

            is_right_bound = !is_right_bound;
        }
    }

    // Seed with a real random value, if available
    std::random_device r;

    // Choose a random mean between 1 and 6
    std::default_random_engine e1(r());
    std::uniform_int_distribution<std::uint64_t> uniform_dist(1, 900'000ull);

    for(uint64_t mean = uniform_dist(e1); 
        auth_tokens.size() < token_test_count; 
        mean = uniform_dist(e1))
    {
        auto pos = gates.lower_bound(mean);
        if(pos != std::cend(gates) && ( mean <= pos->first && mean >= pos->second)) // sort segments by right bouns
        {
            auth_tokens.insert(mean);
        }
        else if(fake_auth_tokens.size() < token_test_count)
        {
            fake_auth_tokens.insert(mean);
        }
    }

    gates.clear();
}

} // namespace tt_tests

