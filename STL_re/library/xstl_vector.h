#ifndef LIBRARY_XSTL_VECTOR_H__
#define LIBRARY_XSTL_VECTOR_H__

#include <algorithm>	// for std::fill
#include <iterator>		// for std::reverse_iterator, __STL_CLASS_PARTIAL_SPECIALIZATION
#include <memeory>		// for std::allocator_traits<Allocator>::pointer, std::uninitialized_fill_n
#include "xstl_allocator.h"

#ifndef _STL_THROW_RANGE_ERRORS
#define _STL_THROW_RANGE_ERRORS
	
#define _stl_throw_range_errors(error) /
	cerr << "Out of Range from " + error; /
	exit(1);
#endif // _STL_THROW_RANGE_ERRORS

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
			return m_data_allocator::allocator(_n);
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
		using _base    					= vector_base<T, Allocator>;

	public:
		// Member Types
		using value_type 				= T;
		using size_type					= std::size_t;
		using difference_type			= std::ptrdiff_t;
		using reference					= value_type&;
		using const_reference			= const value_type&;
		using pointer 					= value_type*;
		using const_pointer				= const value_type*;
		using iterator					= value_type*;
		using const_iterator			= const value_type*;

		typedef typename _base::allocator_type allocator_type;

	#ifdef __STL_CLASS_PARTIAL_SPECIALIZATION
		using reverse_iterator 			= std::reverse_iterator<iterator>;
		using const_reverse_iterator	= std::reverse_iterator<const_iterator>;
	#else // __STL_CLASS_PARTIAL_SPECIALIZATION
		using reverse_iterator 			= std::reverse_iterator<iterator, value_type, reference, difference_type>;
		using const_reverse_iterator	= std::reverse_iterator<const_iterator, value_type, reference, difference_type>;
	#endif // __STL_CLASS_PARTIAL_SPECIALIZATION

	/*
	protected:
		using _base::m_start;
		using _base::m_finish;
		using _base::m_end_of_storage;
		using _base::m_allocate;
		using _base::m_deallocate;
	*/

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

		// destructor
		~vector()
		{
			destroy(m_start, m_finish);
		}

	public:
		// vector<T, Allocator>& operator=(const vector<T, Allocator>& x);
		// Since C++11
		vector<T, Allocator>& operator=(vector<T, Allocator>&& x);
		void assign(size_type count, const T& value) { m_fill_assign(count, value); }
		void m_fill_assign(size_type count, const T& value);
		allocator_type get_allocator() const { return _base::get_allocator(); }

	public:
		// Element access
		// 下标检查
	#ifdef _STL_THROW_RANGE_ERRORS
		void m_range_check(size_type n) const
		{
			if(!(n < this->size())) {
				_stl_throw_range_errors("vector");
			}
		}
		
		reference at(size_type pos)
		{ 
			m_range_check(pos); 
			return (*this)[n]; 
		}
		
		const_reference at(size_type pos) const
		{
			m_range_check(pos);
			return (*this)[n];
		}
	#endif // _STL_THROW_RANGE_ERRORS

		reference operator[](size_type pos) { return *(begin() + pos); }
		const_reference operator[](size_type pos) { return *(begin() + pos); }

		reference front() { return *begin(); }
		const_reference front() const { return *begin(); }

		reference back() { return *(end() - 1); }
		const_reference back() const { return *(end() - 1); }

		T* data() /*noexcept*/ { return &front(); }
		const T* data() const /*noexcept*/ { return &front(); }

	public:
		// Iterators
		iterator begin() /*noexcept*/ { return m_start; }
		const_iterator begin() const /*noexcept*/ { return static_cast<const_iterator>(m_start); }
		const_iterator cbegin() const /*noexcept*/ { return static_cast<const_iterator>(m_start); }
		iterator end() /*noexcept*/ { return m_finish; }
		const_iterator end() const /*noexcept*/ { return static_cast<const_iterator>(m_finish); }
		const_iterator cend() const /*noexcept*/ { return static_cast<const_iterator>(m_finish); }
		
		reverse_iterator rbegin() /*noexcept*/ { return reverse_iterator(begin()); }
		const_reverse_iterator rbegin() const /*noexcept*/ { return const_reverse_iterator(begin()); }
		const_reverse_iterator crbegin() const /*noexcept*/ { return const_reverse_iterator(begin()); }
		reverse_iterator rend() /*noexcept*/ { return reverse_iterator(end()); }
		const_reverse_iterator rend() const /*noexcept*/ { return const_reverse_iterator(end()); }
		const_reverse_iterator crend() const /*noexcept*/ { return const_reverse_iterator(end()); }

	public:
		// Capacity
		bool empty() const /*noexcept*/
		{
			return begin() == end();
		}

		size_type size() const /*noexcept*/
		{
			// return std::distance(begin(), end());
			return static_cast<size_type>(end(), begin());
		}

		size_type max_size() const /*noexcept*/
		{
			return static_cast<size_type>(-1) / sizeof(T);
		}

		void reserve(size_type new_cap)
		{
			if(capacity() < new_cap) {
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
			return static_cast<size_type>(m_end_of_storage - begin();)
		}

		/*void shrink_to_fit() {}*/


	protected:
		template <typename ForwardIterator>
		iterator m_allocate_and_copy(size_type _n, ForwardIterator _first, ForwardIterator _last)
		{
			iterator _result = m_allocate(_n);
			try {
				uninitialized_copy(_first, _last, result);
				return _result;
			}
			catch(...) {
				m_deallocate(_result, _n);
			}
		}
	};

	template <typename T, typename Allocator>
	void vector<T, Allocator>::m_fill_assign(size_t count, const T& value)
	{
		if(count > capacity()) {
			vector<T, Allocator> tmp(count, value, get_allocator());
			tmp.swap(*this);
		}
		else if(count > size()) {
			std::fill(begin(), end(), value);
			m_finish = std::uninitialized_fill_n(m_start, count - size(), value);
		}
		else {	// 如果失败，删除全部元素
			erase(std::fill_n(begin(), count, value), end());
		}
	}

}

#endif // LIBRARY_XSTL_VECTOR_H__