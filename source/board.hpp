#ifndef TT_BOARD_HPP
#define TT_BOARD_HPP

#include <iterator>
#include <memory>

#include <cstdint>

#include "utils/point.hpp"

//#include "utils/index_helpers.hpp"

namespace tt_program
{

class board_t
{

public:
	class cell_t;

	class iterator
	{
	friend board_t;
	public:
		using iterator_category = std::forward_iterator_tag;
		using difference_type = std::ptrdiff_t;
		using value_type = board_t::cell_t;
		using pointer = value_type *;
		using reference = value_type &;

	public:
		iterator(std::uint8_t * block, std::uint8_t mask);

	public:
		value_type operator*() const;
		value_type operator->() const;

	public:
		iterator& operator++();
		iterator operator++(int);

		template<typename Iterator, typename OtherIteratorType>
		friend bool operator== (const Iterator & a, const OtherIteratorType& b);

		template<typename Iterator, typename OtherIteratorType>
		friend bool operator!= (const Iterator & a, const OtherIteratorType& b);

	private:
		std::uint8_t * m_block;
		std::uint8_t m_mask;
	};

	class alive_iterator
	{
	friend board_t;
	public:
		using iterator_category = std::forward_iterator_tag;
		using difference_type = std::ptrdiff_t;
		using value_type = board_t::cell_t;
		using pointer = value_type *;
		using reference = value_type &;

	public:
		alive_iterator(std::uint8_t * block, std::uint8_t * end_block, std::uint8_t mask, std::uint8_t end_mask);

		value_type operator*() const;
		value_type operator->() const;

		alive_iterator& operator++();
		alive_iterator operator++(int);

		template<typename Iterator, typename OtherIteratorType>
		friend bool operator== (const Iterator & a, const OtherIteratorType& b);

		template<typename Iterator, typename OtherIteratorType>
		friend bool operator!= (const Iterator & a, const OtherIteratorType& b);

	private:
		std::uint8_t * m_block;
		std::uint8_t * m_end_block;
		std::uint8_t m_mask;
		std::uint8_t m_end_mask;
	};

public:
	class cell_t
	{
	
	friend board_t;
	friend board_t::iterator;
	friend board_t::alive_iterator;
	
	private:
		cell_t(std::uint8_t & block, std::uint8_t mask);

	public:
		cell_t & operator=(bool is_alive);

		bool is_alive() const noexcept;

	private:
		std::uint8_t & m_block;
		std::uint8_t m_mask;
	};


public:
	board_t();

	board_t(std::int32_t width, std::int32_t height);

public:

	cell_t operator[](const tt_program::utilss::point_t & coordinates);

	//cell_t operator[](const std::int32_t x, const std::int32_t y);

public:
	void swap(board_t & other);

public:
	std::int32_t size() const noexcept;

public:
	void clear();

public:
	board_t::iterator begin();
	
	board_t::iterator end();

	board_t::alive_iterator alive_begin();

	board_t::alive_iterator alive_end();

public:
	tt_program::utilss::point_t coordinates(const board_t::iterator & it) const;

	tt_program::utilss::point_t coordinates(const board_t::alive_iterator & it) const;

	tt_program::utilss::point_t coordinates(const board_t::cell_t & cell) const;

public:
	tt_program::utilss::point_t coordinates(const std::uint8_t const * block, std::uint8_t mask) const;


private:

	std::int32_t coordinates_to_block_index(const tt_program::utilss::point_t & point);

	std::uint8_t coordinates_to_cell_mask(const tt_program::utilss::point_t & point);

private:
	std::int32_t m_width;
	std::int32_t m_height;
	std::unique_ptr<std::uint8_t []> m_buffer;
};


template<typename Iterator, typename OtherIteratorType>
bool operator== (const Iterator & a, const OtherIteratorType& b)
{
	return (a.m_block == b.m_block) && (a.m_mask == b.m_mask);
}

template<typename Iterator, typename OtherIteratorType>
bool operator!= (const Iterator & a, const OtherIteratorType& b)
{
	return !(a == b);
}


} // namespace tt_program

#endif // TT_BOARD_HPP
