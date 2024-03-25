/**
 *  ᛝ
 */

#include "board.hpp"
#include "utils/point.hpp"


namespace tt_program
{

// cell_t
board_t::cell_t::cell_t(std::uint8_t & block, std::uint8_t mask)
	: m_block(block)
	, m_mask(mask)
{}


board_t::cell_t & board_t::cell_t::operator=(bool is_alive)
{
	m_block = (is_alive) ? (m_block | m_mask) : (m_block & ~m_mask);

	return *this;
}


bool board_t::cell_t::is_alive() const noexcept
{
	return !!(m_block & m_mask);
}


// iterator

board_t::iterator::iterator(std::uint8_t * block, std::uint8_t mask)
	: m_block(block)
	, m_mask(mask)
{}


board_t::iterator::value_type board_t::iterator::operator*() const
{
	return board_t::cell_t{*m_block, m_mask};
}

board_t::iterator::value_type board_t::iterator::operator->() const
{
	return board_t::cell_t{*m_block, m_mask};
}


board_t::iterator & board_t::iterator::operator++()
{
	std::uint8_t max_mask = static_cast<std::uint8_t>(1 << 7);
	if(m_mask == max_mask)
	{
		m_mask = 1;
		++m_block;
	}
	else
	{
		m_mask <<= 1;
	}
	
	
	return *this;
}

board_t::iterator board_t::iterator::operator++(int)
{
	iterator tmp = *this;
	++(*this);

	return tmp;
}

// alive_iterator

board_t::alive_iterator::alive_iterator(std::uint8_t * block, std::uint8_t * end_block, std::uint8_t mask, std::uint8_t end_mask)
	: m_block(block)
	, m_end_block(end_block)
	, m_mask(mask)
	, m_end_mask(end_mask)
{}


board_t::alive_iterator::alive_iterator::value_type board_t::alive_iterator::operator*() const
{
	return board_t::cell_t{*m_block, m_mask};
}

board_t::alive_iterator::alive_iterator::value_type board_t::alive_iterator::operator->() const
{
	return board_t::cell_t{*m_block, m_mask};
}


board_t::alive_iterator & board_t::alive_iterator::operator++()
{
	m_mask <<= 1;
	for( m_block; m_block != m_end_block; ++m_block)
	{
		if( !(*m_block) )
		{
			continue;
		}

		for( m_mask ; m_mask > 0; m_mask <<= 1 )
		{
			if( !!(*m_block & m_mask) )
			{
				return *this;
			}
		}

		m_mask = 1;
	}

	for( m_mask ; m_mask < m_end_mask; m_mask <<= 1 )
	{
		if( !!(*m_block & m_mask) )
		{
			break;
		}
	}

	return *this;
}

board_t::alive_iterator board_t::alive_iterator::operator++(int)
{
	alive_iterator tmp = *this;
	++(*this);

	return tmp;
}


// board_t
board_t::board_t()
	: m_width(0)
	, m_height(0)
	, m_buffer(nullptr)
{}

board_t::board_t(std::int32_t width, std::int32_t height)
	: m_width(width)
	, m_height(height)
	, m_buffer(nullptr)
{
	m_buffer = std::make_unique<std::uint8_t[]>(size());
	clear();
}


board_t::cell_t board_t::operator[](const tt_program::utilss::point_t & coordinates)
{
	cell_t cell{ m_buffer[coordinates_to_block_index(coordinates)], coordinates_to_cell_mask(coordinates) };

	return cell;
}


void board_t::swap(board_t & other)
{
	std::swap(m_buffer, other.m_buffer);
	std::swap(m_width, other.m_width);
	std::swap(m_height, other.m_height);
}


std::int32_t board_t::size() const noexcept
{
	return m_width * m_height / 8 + 1;
}


void board_t::clear()
{
	for(std::int32_t i = 0, b_size = size(); i < b_size; ++i)
	{
		m_buffer[i] = 0;
	}
}


board_t::iterator board_t::begin()
{
	return board_t::iterator(&(m_buffer[0]), 1 );
}
	
board_t::iterator board_t::end()
{
	auto end_blocK_index = size()-1;
	auto end_mask = coordinates_to_cell_mask( {0, m_height} );

	return board_t::iterator( &(m_buffer[end_blocK_index]), end_mask);
}

board_t::alive_iterator board_t::alive_begin()
{
	auto end_blocK_index = size()-1;
	auto end_mask = coordinates_to_cell_mask( {0, m_height} ); 

	for(std::int32_t i = 0; i < end_blocK_index; i++)
	{
		if( m_buffer[i] )
		{
			for(std::uint8_t mask = 1; mask > 0; mask <<= 1)
			{
				if( !!(m_buffer[i] & mask) )
				{
					return board_t::alive_iterator( &(m_buffer[i]), &(m_buffer[end_blocK_index]), mask, end_mask );
				}
			}
		}
	}


	for(std::uint8_t mask = 1; mask < end_mask; mask <<= 1)
	{
		if( !!(m_buffer[end_blocK_index] & mask) )
		{
			return board_t::alive_iterator( &(m_buffer[end_blocK_index]), &(m_buffer[end_blocK_index]), mask, end_mask );
		}
	}

	return board_t::alive_iterator( &(m_buffer[end_blocK_index]), &(m_buffer[end_blocK_index]), end_mask, end_mask );
}

board_t::alive_iterator board_t::alive_end()
{
	auto end_blocK_index = size()-1;
	auto end_mask = coordinates_to_cell_mask( {0, m_height} );

	return board_t::alive_iterator( &(m_buffer[end_blocK_index]),  &(m_buffer[end_blocK_index]), end_mask, end_mask );
}


tt_program::utilss::point_t board_t::coordinates(const board_t::iterator & it) const
{
	return coordinates(it.m_block, it.m_mask);
}

tt_program::utilss::point_t board_t::coordinates(const board_t::alive_iterator & it) const
{
	return coordinates(it.m_block, it.m_mask);
}

tt_program::utilss::point_t board_t::coordinates(const board_t::cell_t & cell) const
{
	return coordinates(&(cell.m_block), cell.m_mask);
}

tt_program::utilss::point_t board_t::coordinates(const std::uint8_t const * block, std::uint8_t mask) const
{
	std::int32_t y_distance = block - &(m_buffer[0]);
	std::int32_t x_distanle = 0;
	while(mask > 1)
	{
		++x_distanle;
		mask >>= 1;
	}
	auto coordinates_combination = y_distance * 8 + x_distanle;
	std::int32_t y = coordinates_combination / m_width;
	std::int32_t x = coordinates_combination % m_width;

	return { x, y };
}


// if you change this function also you need check coordinates_by_index works correctly
std::int32_t board_t::coordinates_to_block_index(const tt_program::utilss::point_t & coordinates)
{
	return ( coordinates.y * m_width + coordinates.x ) / 8; 
}

std::uint8_t board_t::coordinates_to_cell_mask(const tt_program::utilss::point_t & coordinates)
{
	return static_cast<std::uint8_t>(1 << (( coordinates.y * m_width + coordinates.x ) % 8));
}


} // namespace tt_program

