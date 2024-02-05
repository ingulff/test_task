#ifndef TT_WORKER_HPP
#define TT_WORKER_HPP

#include <memory>

#include "task.hpp"

namespace tt_program
{

class command_worker
{
public:
	command_worker();

	~command_worker();

	command_worker(command_worker & other) = delete;

	command_worker & operator=(command_worker & other) = delete;

	command_worker(command_worker && other);

	command_worker & operator=(command_worker && other);

	command_worker(std::ostream & out_stream);

public:
	void process(const tt_program::task::task_t & task);

private:
	class command_worker_impl;
	std::unique_ptr<command_worker_impl> m_worker_impl;
};

} // namespace tt_program

#endif // TT_WORKER_HPP
