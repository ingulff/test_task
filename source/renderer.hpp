#ifndef TT_SDL_RENDERER_HPP
#define TT_SDL_RENDERER_HPP

#include <memory>

#include "board.hpp"
#include "error/error.hpp"
#include "settings.hpp"

namespace tt_program
{

class renderer
{
public:
	renderer();

	renderer(renderer & other) = delete;
	renderer & operator=(renderer & other) = delete;

	renderer(renderer && other);
	renderer & operator=(renderer && other);

	~renderer();

public:
	enum error::status_code initialize(tt_program::settings_t & settings);

public:
	std::int32_t x_offset() const;

	std::int32_t y_offset() const;

public:
	void update(tt_program::board_t & board);

public:
	void fullscreen_handle();

private:
	class renderer_impl;
	std::unique_ptr<renderer_impl> m_impl;

	error::status_code m_status;
};

} // namespace tt_program

#endif // TT_SDL_RENDERER_HPP
