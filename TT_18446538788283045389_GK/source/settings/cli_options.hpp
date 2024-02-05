#ifndef TT_CLI_OPTIONS
#define TT_CLI_OPTIONS

#include <string>

namespace tt_program::settings
{

struct options_t
{
	bool need_show_help_msg = false;
	std::string filepath;
	std::string mode;
	std::string mode_value;
};

} // tt_program::settings

#endif // TT_CLI_OPTIONS