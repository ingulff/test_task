/**
 *  ᛝ
 */

#include <algorithm>
#include <iterator>
#include <string>
#include <unordered_map>

#include "settings/options_config.hpp"
#include "settings/options_validator.hpp"

namespace tt_program::settings
{

class options_validator final : public option_validator_interface
{

public:
	options_validator() = default;

	options_validator(options_validator & other) = default;

	options_validator & operator=(options_validator & other) = default;

	options_validator(options_validator && other) = default;

	options_validator & operator=(options_validator && other) = default;

	~options_validator() = default;

public:
	bool is_options_valid(const tt_program::settings::options_t & options) const override final
	{
		if( options.need_show_help_msg == true )
		{
			return true;
		}

		if( options.filepath.empty() == true )
		{
			return false;
		}

		// -m option values
		const auto  & mode = options.mode;
		if( m_value_option_to_flag.count(mode) == 0 )
		{
			return false;
		}

		// -v option values
		const auto & mode_value = options.mode_value;
		if( mode_value.empty() == false )
		{

			return is_additional_option_applicable(mode);
		}

		return true;
	}

public:

	void add_option(std::string mode, std::string mode_flag, std::vector<std::string> expected_flags) override final
	{
		m_value_to_expected_mode.emplace( std::make_pair(mode, std::move(expected_flags)) );
		m_value_option_to_flag.emplace( std::make_pair(std::move(mode), std::move(mode_flag)) );
	}

	void clear() noexcept override final
	{
		m_value_to_expected_mode.clear();
		m_value_option_to_flag.clear();
	}

private:
	bool is_additional_option_applicable(const std::string & mode) const
	{
		bool res = false;
		try
		{
			for(const auto & additional_option_flag : additional_option_flags)
			{
				// -v, etc
				const auto & expected_options = m_value_to_expected_mode.at(mode);
				if( const auto pos = std::find(std::cbegin(expected_options), std::cend(expected_options), additional_option_flag);
					pos != std::cend(expected_options) )
				{
					res = true;
					break;
				}
			}
		}
		catch(...)
		{
			res = false;
		}	

		return res;
	}

private:

	std::unordered_map<std::string, std::vector<std::string>> m_value_to_expected_mode;
	std::unordered_map<std::string, std::string>              m_value_option_to_flag;

};


std::unique_ptr<option_validator_interface> make_unique_option_validator()
{
	return std::make_unique<options_validator>();
}

} // namespace tt_program::settings
