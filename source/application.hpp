#ifndef TT_APPLICATION_HPP
#define TT_APPLICATION_HPP

#include <memory>

#include <cstdint>

#include "error/error.hpp"

namespace tt_program
{

class application_t
{

public:
	application_t();

	application_t(application_t & other) = delete;
	application_t & operator=(application_t & other) = delete;

	application_t(application_t && other);
	application_t & operator=(application_t && other);

	~application_t();

public:
	int initialize();

	int initialize(std::int32_t board_width, std::int32_t board_height);

public:
	int exec();

private:
	class application_impl;
	std::unique_ptr<application_impl> m_impl;
};

} // namespace tt_program


#endif // TT_APPLICATION_HPP
