#ifndef TT_FILEPATHS_HPP
#define TT_FILEPATHS_HPP

#include <set>
#include <string>
#include <system_error>
#include <vector>
#include <unordered_set>

#include <cstdint>

namespace tt_tests
{

#ifdef UPLOAD_SOURCE_DIR
constexpr std::string_view upload_resource_dir = UPLOAD_SOURCE_DIR;
#else
constexpr std::string_view upload_resource_dir = "./resources/upload";
#endif // UPLOAD_SOURCE_DIR

#ifdef DOWNLOAD_SOURCE_DIR
constexpr std::string_view download_resourse_dir = DOWNLOAD_SOURCE_DIR;
#else
constexpr std::string_view download_resourse_dir = "./resources/download";
#endif // DOWNLOAD_SOURCE_DIR

#ifdef CONFIG_FILE_PATH
constexpr std::string_view config_filepath = CONFIG_FILE_PATH;
#else
constexpr std::string_view config_filepath = "./configs/ids.txt"
#endif // CONFIG_FILE_PATH


inline std::vector<std::string> upload_target_filepaths;

inline std::vector<std::string> upload_filepaths;

inline std::vector<std::string> download_filepaths;

inline std::unordered_set<std::uint32_t> upload_files_hashs;

inline std::set<std::uint64_t> auth_tokens;
inline std::set<std::uint64_t> fake_auth_tokens;


void generate_uploads_targets_filepaths(const std::string_view & dir_path);
void generate_upload_filepaths(const std::string_view & dir_path);
void generate_download_filepaths(const std::string_view & dir_path);
void generate_upload_file_hashs(const std::string_view & dir_path);
void generate_config_gates(const std::string_view & filepath);
void generate_tests_tokens(const std::string_view & filepath);

std::error_code clear_downloading_directory();

} // namespace tt_tests

#endif // TT_FILEPATHS_HPP
