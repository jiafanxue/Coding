#ifndef LIBRARY_XSTL_VECTOR_H__
#define LIBRARY_XSTL_VECTOR_H__

#include <initializer_list> 	// for std::initializer_list
#include <iostream>				// for std::cerr
#include <algorithm>			// for std::fill, std::copy, std::copy_backward, std::equal
#include <iterator>				// for std::reverse_iterator
#include <memory>				// for std::allocator_traits<Allocator>::pointer, std::uninitialized_fill_n
#include "xstl_allocator.h"

#ifndef _STL_THROW_RANGE_ERROR
#define _STL_THROW_RANGE_ERROR

#define _stl_throw_range_errors(error) { std::cerr << "Out of range " << error << std::endl; exit(1); }

#endif // _STL_THROW_RANGE_ERROR

namespace xstl
{
	template <typename T, typename Alloc>
	class vector_base
	{
	public:
		using allocator_type = Alloc;
		allocator_type get_allocator() const
		{
			return allocator_type();
		}

	public:
		vector_base(const Alloc&)
			: m_start(0), m_finish(0), m_end_of_storage(0)
		{
			// empty
		}

		vector_base(std::size_t n, const Alloc&)
			: m_start(0), m_finish(0), m_end_of_storage(0)
		{
			m_start = m_allocate(n);
			m_finish = m_start;
			m_end_of_storage = m_start + n;
		}

		~vector_base()
		{
			m_deallocate(m_start, m_end_of_storage - m_start);
		}

	protected:
		T* m_start;						// 表示目前使用空间的头
		T* m_finish;					// 表示目前使用空间的尾
		T* m_end_of_storage;			// 表示目前可用空间的尾

		using m_data_allocator = xstl::simple_alloc<T, Alloc>;

		T* m_allocate(std::size_t _n)
		{
			return m_data_allocator::allocate(_n);
		}

		void m_deallocate(T* _p, std::size_t _n)
		{
			m_data_allocator::deallocate(_p, _n);
		}
	};


	template <typename T, typename Allocator = xstl::allocator<T>>
	class vector : protected vector_base<T, Allocator>
	{
	private:
		using _base = vector_base<T, Allocator>;

	public:
		// Member Types
		using value_type = T;
		using size_type = std::size_t;
		using difference_type = std::ptrdiff_t;
		using reference = value_type&;
		using const_reference = const value_type&;
		using pointer = value_type*;
		using const_pointer = const value_type*;
		using iterator = value_type*;
		using const_iterator = const value_type*;

		typedef typename _base::allocator_type allocator_type;

		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;

	protected:
		using _base::m_start;
		using _base::m_finish;
		using _base::m_end_of_storage;
		using _base::m_allocate;
		using _base::m_deallocate;


	public:
		// Member functions

		// Constructor 
		// Until C++14
		explicit vector(const allocator_type& alloc = allocator_type())
			: _base(alloc)
		{
			// empty
		}

		// C++14 实现
		/*
		vector() : vector(allocator_type()) {}
		explicit vector(allocator_type& alloc = allocator_type())
		: _base(alloc)
		{
		// empty
		}
		*/

		/*
		例如实现：vector<string> vec(10, 'c')
		*/
		vector(size_type count, const T& value,
			const allocator_type& alloc = allocator_type())
			: _base(count, alloc)
		{
			m_finish = std::uninitialized_fill_n(m_start, count, value);
		}

		// C++11
		// explicit vecotr(size_type count)
		// Since C++14
		explicit vector(size_type count, const allocator_type& alloc = allocator_type())
			: _base(count, alloc)
		{
			m_finish = std::uninitialized_fill_n(m_start, count, T());
		}

		vector(const T* _first, const T* _last,
			const allocator_type& alloc = allocator_type())
			: _base(_last - _first, alloc)
		{
			m_finish = std::uninitialized_copy(_first, _last, m_start);
		}

		vector(const vector<T, Allocator>& x)
			: _base(x.size(), x.get_allocator())
		{
			m_finish = std::uninitialized_copy(x.begin(), x.end(), m_start);
		}

		vector(std::initializer_list<T> init,
			const allocator_type& alloc = allocator_type())
			: _base(init.end() - init.begin(), alloc)
		{
			m_finish = std::uninitialized_copy(init.begin(), init.end(), m_start);
		}

		// destructor
		~vector()
		{
			destroy(m_start, m_finish);
		}

		void destroy(iterator pos);
		void destroy(iterator _first, iterator _last);

	public:
		vector<T, Allocator>& operator=(const vector<T, Allocator>& x);
		// Since C++11
		// vector<T, Allocator>& operator=(vector<T, Allocator>&& x);
		void assign(size_type count, const T& value) { m_fill_assign(count, value); }
		void m_fill_assign(size_type count, const T& value);
		allocator_type get_allocator() const { return _base::get_allocator(); }

	public:
		// Element access
		// 下标检查
#ifndef _STL_THROW_RANGE_ERRORS
#define _STL_THROW_RANGE_ERRORS
		void m_range_check(size_type n) const
		{
			if (!(n < this->size())) {
				_stl_throw_range_errors("vector");
			}
		}
#endif // _STL_THROW_RANGE_ERRORS

		reference at(size_type pos)
		{
			m_range_check(pos);
			return (*this)[pos];
		}

		const_reference at(size_type pos) const
		{
			m_range_check(pos);
			return (*this)[pos];
		}


		reference operator[](size_type pos) { return *(begin() + pos); }
		const_reference operator[](size_type pos) const { return *(begin() + pos); }

		reference front() { return *begin(); }
		const_reference front() const { return *begin(); }

		reference back() { return *(end() - 1); }
		const_reference back() const { return *(end() - 1); }

		T* data() /*noexcept*/ { return &front(); }
		const T* data() const /*noexcept*/ { return &front(); }

	public:
		// Iterators
		iterator begin() /*noexcept*/ { return m_start; }
		const_iterator begin() const /*noexcept*/ { return m_start; }
		const_iterator cbegin() const /*noexcept*/ { return static_cast<const_iterator>(m_start); }
		iterator end() /*noexcept*/ { return m_finish; }
		const_iterator end() const /*noexcept*/ { return m_finish; }
		const_iterator cend() const /*noexcept*/ { return static_cast<const_iterator>(m_finish); }

		reverse_iterator rbegin() /*noexcept*/ { return reverse_iterator(end()); }
		const_reverse_iterator rbegin() const /*noexcept*/ { return const_reverse_iterator(end()); }
		const_reverse_iterator crbegin() const /*noexcept*/ { return const_reverse_iterator(end()); }
		reverse_iterator rend() /*noexcept*/ { return reverse_iterator(begin()); }
		const_reverse_iterator rend() const /*noexcept*/ { return const_reverse_iterator(begin()); }
		const_reverse_iterator crend() const /*noexcept*/ { return const_reverse_iterator(begin()); }

	public:
		// Capacity
		bool empty() const /*noexcept*/
		{
			return begin() == end();
		}

		size_type size() const /*noexcept*/
		{
			// return std::distance(begin(), end());
			return static_cast<size_type>(end() - begin());
		}

		size_type max_size() const /*noexcept*/
		{
			return static_cast<size_type>(-1) / sizeof(T);
		}

		void reserve(size_type new_cap)
		{
			if (capacity() < new_cap) {
				const size_type old_size = size();
				iterator tmp = m_allocate_and_copy(new_cap, m_start, m_finish);
				destroy(m_start, m_finish);
				m_deallocate(m_start, m_end_of_storage - m_start);
				m_start = tmp;
				m_finish = tmp + old_size;
				m_end_of_storage = m_start + new_cap;
			}
		}

		size_type capacity() const
		{
			return static_cast<size_type>(m_end_of_storage - begin());
		}

		/*void shrink_to_fit() {}*/

	public:
		// Until C++11
		iterator erase(iterator pos)
		{
			if (pos + 1 != end()) {
				std::copy(pos + 1, m_finish, pos);
			}
			--m_finish;
			destroy(m_finish);
			return pos;
		}

		// Since C++11
		iterator erase(const_iterator pos)
		{
			if (pos + 1 != end()) {
				std::copy(pos + 1, m_finish, pos);
			}
			--m_finish;
			destroy(m_finish);
			return pos;
		}

		iterator erase(iterator first, iterator last)
		{
			iterator i = std::copy(last, m_finish, first);
			destroy(i, m_finish);
			m_finish = m_finish - (last - first);
			return first;
		}

		iterator erase(const_iterator first, const_iterator last)
		{
			iterator i = std::copy(last, m_finish, first);
			destroy(i, m_finish);
			m_finish = m_finish - (last - first);
			return first;
		}

		void clear()
		{
			erase(begin(), end());
		}

		iterator insert(iterator pos, const T& value)
		{
			size_type n = pos - begin();
			if (m_finish != m_end_of_storage && pos == end()) {
				construct(m_finish, value);
				++m_finish;
			}
			else {
				m_insert_aux(pos, value);
			}
			return begin() + n;
		}

		// Since C++11
		iterator insert(const_iterator pos, const T& value)
		{
			size_type n = pos - begin();
			if (m_finish != m_end_of_storage && pos == end()) {
				construct(m_finish, value);
				++m_finish;
			}
			else {
				m_insert_aux(pos, value);
			}
			return begin() + n;
		}

		iterator insert(const_iterator pos)
		{
			size_type n = pos - begin();
			if (m_finish != m_end_of_storage && pos == end()) {
				construct(m_finish);
				++m_finish;
			}
			else {
				m_insert_aux(pos);
			}
			return begin() + n;
		}

		void insert(iterator pos, size_type count, const T& value)
		{
			m_fill_insert(pos, count, value);
		}

		void m_fill_insert(iterator pos, size_type count, const T& value);

		void push_back(const T& value)
		{
			if (m_finish != m_end_of_storage) {
				construct(m_finish, value);
				++m_finish;
			}
			else {
				m_insert_aux(end(), value);
			}
		}

		void push_back()
		{
			if (m_finish != m_end_of_storage) {
				construct(m_finish);
				++m_finish;
			}
			else {
				m_insert_aux(end());
			}
		}

		/*
		void push_back(T&& value)
		{
		if(m_finish != m_end_of_storage) {
		construct(m_finish, value);
		++m_finish;
		}
		else {
		m_insert_aux(end(), value);
		}
		}
		*/

		void pop_back()
		{
			--m_finish;
			destroy(m_finish);
		}

		void resize(size_type new_size)
		{
			resize(new_size, T());
		}

		void resize(size_type new_size, const value_type& value) /* const T& value*/
		{
			if (new_size < size()) {
				erase(begin() + new_size, end());
			}
			else {
				insert(end(), new_size - size(), value);
			}
		}

		void swap(vector<T, Allocator>& x)
		{
			// Specializetion
			// std::swap for vector
			std::swap(m_start, x.m_start);
			std::swap(m_finish, x.m_finish);
			std::swap(m_end_of_storage, x.m_end_of_storage);
		}

	protected:
		template <typename ForwardIterator>
		iterator m_allocate_and_copy(size_type _n, ForwardIterator _first, ForwardIterator _last)
		{
			iterator _result = m_allocate(_n);
			try {
				std::uninitialized_copy(_first, _last, _result);
				return _result;
			}
			catch (...) {
				m_deallocate(_result, _n);
			}
			return NULL;
		}

		void m_insert_aux(iterator pos, const T& value);
		void m_insert_aux(iterator pos);
		void construct(pointer _p, const T& _val) { new(_p)T(_val); }
	};

	template <typename T, typename Allocator>
	void vector<T, Allocator>::destroy(iterator pos)
	{
		pos->~T();
	}

	template <typename T, typename Allocator>
	void vector<T, Allocator>::destroy(iterator first, iterator last)
	{
		for (; first != last; ++first) {
			first->~T();
		}
	}

	template <typename T, typename Allocator>
	vector<T, Allocator>& vector<T, Allocator>::operator=(const vector<T, Allocator>& x)
	{
		if (&x != this) {
			const size_type xlen = x.size();
			if (xlen > capacity()) {
				iterator tmp = m_allocate_and_copy(xlen, x.begin(), x.end());
				destroy(m_start, m_finish);
				m_deallocate(m_start, m_end_of_storage - m_start);
				m_start = tmp;
				m_end_of_storage = m_start + xlen;
			}
			else if (size() >= xlen) {
				iterator i = std::copy(x.begin(), x.end(), begin());
				destroy(i, m_finish);
			}
			else {
				std::copy(x.begin(), x.begin() + size(), m_start);
				std::uninitialized_copy(x.begin() + size(), x.end(), m_finish);
			}
			m_finish = m_start + xlen;
		}
		return *this;
	}

	template <typename T, typename Allocator>
	void vector<T, Allocator>::m_fill_assign(size_type count, const T& value)
	{
		if (count > capacity()) {
			vector<T, Allocator> tmp(count, value, get_allocator());
			tmp.swap(*this);
		}
		else if (count > size()) {
			std::fill(begin(), end(), value);
			m_finish = std::uninitialized_fill_n(m_start, count - size(), value);
		}
		else {	// 如果失败，删除全部元素
			erase(std::fill_n(begin(), count, value), end());
		}
	}

	template <typename T, typename Allocator>
	void vector<T, Allocator>::m_insert_aux(iterator pos, const T& value)
	{
		if (m_finish != m_end_of_storage) {
			construct(m_finish, *(m_finish - 1));
			++m_finish;
			T value_copy = value;
			std::copy_backward(pos, m_finish - 2, m_finish - 1);
			*pos = value_copy;
		}
		else {
			const size_type old_size = size();
			const size_type len = old_size != 0 ? 2 * old_size : 1;
			iterator new_start = m_allocate(len);
			iterator new_finish = new_start;
			try {
				new_finish = std::uninitialized_copy(m_start, pos, new_start);
				construct(new_finish, value);
				++new_finish;
				new_finish = std::uninitialized_copy(pos, m_finish, new_finish);
			}
			catch (...) {
				destroy(new_start, new_finish);
				m_deallocate(new_start, len);
			}
			destroy(begin(), end());
			m_deallocate(m_start, m_end_of_storage - m_start);
			m_start = new_start;
			m_finish = new_finish;
			m_end_of_storage = new_start + len;
		}
	}

	template <typename T, typename Allocator>
	void vector<T, Allocator>::m_insert_aux(iterator pos)
	{
		if (m_finish != m_end_of_storage) {
			construct(m_finish, *(m_finish - 1));
			++m_finish;
			std::copy_backward(pos, m_finish - 2, m_finish - 1);
			*pos = T();
		}
		else {
			const size_type old_size = size();
			const size_type len = old_size != 0 ? 2 * old_size : 1;
			iterator new_start = m_allocate(len);
			iterator new_finish = m_start;
			try {
				new_finish = std::uninitialized_copy(m_start, pos, new_start);
				construct(new_finish);
				++new_finish;
				new_finish = std::uninitialized_copy(pos, m_finish, new_finish);
			}
			catch (...) {
				destroy(new_start, new_finish);
				m_deallocate(new_start, len);
			}
			destroy(begin(), end());
			m_deallocate(m_start, m_end_of_storage - m_start);
			m_start = new_start;
			m_finish = new_finish;
			m_end_of_storage = new_start + len;
		}
	}

	template <typename T, typename Allocator>
	void vector<T, Allocator>::m_fill_insert(iterator pos, size_type count, const T& value)
	{
		if (count != 0) {
			if (static_cast<size_type>(m_end_of_storage - m_finish) >= 0) {
				T value_copy = value;
				const size_type elems_after = m_finish - pos;
				iterator old_finish = m_finish;
				if (elems_after > count) {
					std::uninitialized_copy(m_finish - count, m_finish, m_finish);
					m_finish += count;
					std::copy_backward(pos, old_finish - count, old_finish);
					std::fill(pos, pos + count, value_copy);
				}
				else {
					std::uninitialized_fill_n(m_finish, count - elems_after, value_copy);
					m_finish += count - elems_after;
					std::uninitialized_copy(pos, old_finish, m_finish);
					m_finish += elems_after;
					std::fill(pos, old_finish, value_copy);
				}
			}
			else {
				const size_type old_size = size();
				const size_type len = old_size + std::max(old_size, count);
				iterator new_start = m_allocate(len);
				iterator new_finish = m_start;
				try {
					new_finish = std::uninitialized_copy(m_start, pos, new_start);
					new_finish = std::uninitialized_fill_n(new_finish, count, value);
					new_finish = std::uninitialized_copy(pos, m_finish, new_finish);
				}
				catch (...) {
					destroy(new_start, new_finish);
					m_deallocate(new_start, len);
				}
				destroy(m_start, m_finish);
				m_deallocate(m_start, m_end_of_storage - m_start);
				m_start = new_start;
				m_finish = new_finish;
				m_end_of_storage = new_start + len;
			}
		}
	}


	// Non-member functions
	template <typename T, typename Allocator>
	inline bool operator==(const vector<T, Allocator>& lhs,
		const vector<T, Allocator>& rhs)
	{
		return lhs.size() == rhs.size() &&
			std::equal(lhs.begin(), lhs.end(), rhs.begin());
	}

	template <typename T, typename Allocator>
	inline bool operator!=(const vector<T, Allocator>& lhs,
		const vector<T, Allocator>& rhs)
	{
		return !(lhs == rhs);
	}

	template <typename T, typename Allocator>
	inline bool operator<(const vector<T, Allocator>& lhs,
		const vector<T, Allocator>& rhs)
	{
		return lexicographical_compare(lhs.begin(), lhs.end(),
			rhs.begin(), rhs.end());
	}

	template <typename T, typename Allocator>
	inline bool operator<=(const vector<T, Allocator>& lhs,
		const vector<T, Allocator>& rhs)
	{
		return !(lhs > rhs);
	}

	template <typename T, typename Allocator>
	inline bool operator>(const vector<T, Allocator>& lhs,
		const vector<T, Allocator>& rhs)
	{
		return lhs < rhs;
	}

	template <typename T, typename Allocator>
	inline bool operator>=(const vector<T, Allocator>& lhs,
		const vector<T, Allocator>& rhs)
	{
		return !(lhs < rhs);
	}

}

#endif // LIBRARY_XSTL_VECTOR_H__