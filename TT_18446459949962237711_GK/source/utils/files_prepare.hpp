#ifndef TT_FILE_PREPARE_HPP
#define TT_FILE_PREPARE_HPP

#include <filesystem>
#include <string>

#include <boost/beast/core/string.hpp>

namespace tt_program::utils
{

// Return a reasonable mime type based on the extension of a file.
boost::beast::string_view mime_type(boost::beast::string_view path);


// Append an HTTP rel-path to a local filesystem path.
// The returned path is normalized for the platform.
std::string path_cat(boost::beast::string_view base, boost::beast::string_view path);

} // tt_program::utils

#endif //TT_FILE_PREPARE_HPP
