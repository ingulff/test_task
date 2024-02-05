#ifndef TT_OPTION_VALIDATOR_HPP
#define TT_OPTION_VALIDATOR_HPP

#include <memory>

#include "settings/cli_options.hpp"

namespace tt_program::settings
{

class option_validator_interface
{

public:
	virtual bool is_options_valid(const tt_program::settings::options_t & options) const = 0;

	virtual void clear() noexcept = 0;

	virtual void add_option(std::string mode, std::string mode_flag, std::vector<std::string> expected_flags)= 0;
};


using opt_validat_uniqu_ptr = std::unique_ptr<option_validator_interface>; 

opt_validat_uniqu_ptr make_unique_option_validator();

} // namespace tt_program::settings

#endif // TT_OPTION_VALIDATOR_HPP
