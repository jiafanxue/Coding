#ifndef LIBRARY_XSTL_STRING_H__
#define LIBRARY_XSTL_STRING_H__

#include <algorithm>			// for std::search, std::find_if, std::find_end
#include <cstdlib>				// for std::size_t, std::ptrdiff_t
#include <iterator>				// for std::reverse_iterator 
#include <functional>			// for std::binary_function
#include <memory>				// for std::allocator_traits<Allocator>::pointer, std::uninitialized_fill_n
#include <stddef.h>
#include <ostream>				// for std::basic_ostream, std::basic_istream
//#include <string>
#include "xstl_allocator.h"

#ifndef _STL_THROW_ERROR
#define _STL_THROW_ERROR

#define _stl_throw_length_error(error) { std::cerr << "Length error" << error << std::endl; exit(1); }
#define _stl_throw_out_of_range(error) { std::cerr << "Out of range " << error << std::endl; exit(1); }

#endif // _STL_THROW_ERROR

namespace xstl
{
	template <typename T>
	struct _Eq_traits :
		public std::unary_function<T, bool>
	{
		bool operator()(const T& lhs, const T& rhs) const
		{
			if (lhs == rhs) {
				return true;
			}
			return false;
		}
	};

	template <typename Traits>
	struct _Not_within_traits :
		public std::unary_function<typename Traits::char_type, bool>
	{
		typedef const typename Traits::char_type* _Pointer;
		const _Pointer m_first;
		const _Pointer m_last;

		_Not_within_traits(_Pointer _first, _Pointer _last) : m_first(_first), m_last(_last) { }

		bool operator()(const typename Traits::char_type& _x) const
		{
			return std::find_if(m_first, m_last,
				std::bind1st(_Eq_traits<Traits>(), _x)) == m_last;
		}
	};

	template <typename T, typename Alloc>
	class _String_base
	{
	public:
		using allocator_type = Alloc;
		allocator_type get_allocator() const { return allocator_type(); }

	protected:
		_String_base(const allocator_type&)
			: m_start(0), m_finish(0), m_end_of_storage(0)
		{
			// empty
		}

		_String_base(const allocator_type&, size_t _n)
			: m_start(0), m_finish(0), m_end_of_storage(0)
		{
			m_allocate_block(_n);
		}

	protected:
		T* m_start;
		T* m_finish;
		T* m_end_of_storage;

		using _Alloc_type = xstl::simple_alloc<T, Alloc>;

		T* m_allocate(size_t _n) { return _Alloc_type::allocate(_n); }

		void m_deallocate(T* _p, size_t _n)
		{
			if (_p) {
				_Alloc_type::deallocate(_p, _n);
			}
		}

		void m_allocate_block(size_t _n)
		{
			if (_n <= max_size()) {
				m_start = m_allocate(_n);
				m_finish = m_start;
				m_end_of_storage = m_start + _n;
			}
			else {
				m_throw_length_error();
			}
		}

		void m_deallocate_block()
		{
			m_deallocate(m_start, m_end_of_storage - m_start);
		}

		size_t max_size() const { return (static_cast<size_t>(-1) / sizeof(T)) - 1; }

		void m_throw_length_error() const;
		void m_throw_out_of_range() const;
	};

#ifdef _STL_THROW_ERROR

	template <typename T, typename Alloc>
	void _String_base<T, Alloc>::m_throw_length_error() const
	{
		_stl_throw_length_error("basic_string");
	}

	template <typename T, typename Alloc>
	void _String_base<T, Alloc>::m_throw_out_of_range() const
	{
		_stl_throw_out_of_range("basic_string");
	}

#endif 



	template <typename CharT, typename Traits, typename Alloc>
	class basic_string : private _String_base<CharT, Alloc>
	{
	public:
		// Member Types
		using traits_type = Traits;
		using value_type = CharT;
		using pointer = value_type*;
		using const_pointer = const value_type*;
		using reference = value_type&;
		using const_reference = const value_type&;
		using size_type = std::size_t;
		using difference_type = std::ptrdiff_t;
		using iterator = value_type*;
		using const_iterator = const value_type*;

		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;

		using _Base = _String_base<CharT, Alloc>;

		static const std::size_t npos = -1;

	public:
		typedef typename _Base::allocator_type allocator_type;
		allocator_type get_allocator() const { return _Base::get_allocator(); }

	protected:
		using _Base::m_allocate;
		using _Base::m_deallocate;
		using _Base::m_allocate_block;
		using _Base::m_deallocate_block;
		using _Base::m_throw_out_of_range;
		using _Base::m_throw_length_error;

		using _Base::m_start;
		using _Base::m_finish;
		using _Base::m_end_of_storage;

	public:
		struct _Reserve_t {};

		basic_string(_Reserve_t, size_t count,
			const allocator_type& alloc = allocator_type())
			: _Base(alloc, count + 1) {
			m_terminate_string();
		}

		// Constructor && Destructor
		explicit basic_string(const allocator_type& alloc = allocator_type())
			: _Base(alloc, 8)
		{
			m_terminate_string();
		}

		// Since C++14
		/*
		basic_string() : basic_string(allocator_type()) {}
		explicit basic_string(const allocator_type& alloc);
		*/

		basic_string(size_type count, CharT ch,
			const allocator_type& alloc = allocator_type())
			: _Base(alloc, count + 1)
		{
			m_finish = std::uninitialized_fill_n(m_start, count, ch);
			m_terminate_string();
		}

		basic_string(const basic_string& other, size_type pos,
			size_type count = npos,
			const allocator_type& alloc = allocator_type())
			: _Base(alloc)
		{
			if (pos > other.size()) {
				m_throw_out_of_range();
			}
			else {
				m_range_initialize(other.begin() + pos,
					other.begin() + pos + std::min(count, other.size() - pos));
			}
		}

		basic_string(const CharT* s, size_type count,
			const allocator_type& alloc = allocator_type())
			: _Base(alloc)
		{
			m_range_initialize(s, s + count);
		}

		basic_string(const CharT* s,
			const allocator_type& alloc = allocator_type())
			: _Base(alloc)
		{
			m_range_initialize(s, s + Traits::length(s));
		}

		// Since C++11
		/*
		basic_string(const basic_string& other,
		const allocator_type& alloc = allocator_type())
		{

		}
		*/

		basic_string(const CharT* first, const CharT* last,
			const allocator_type& alloc = allocator_type())
			: _Base(alloc)
		{
			m_range_initialize(first, last);
		}

		~basic_string()
		{
			destroy(m_start, m_finish + 1);
		}

		void destroy(iterator pos);
		void destroy(iterator _first, iterator _last);
		void constuct(CharT* &p)
		{
			p = new CharT();
		}

	private:
		void m_construct_null(CharT* _p)
		{
			constuct(_p);
			try {
				*_p = static_cast<CharT>(0);
			}
			catch (...) {
				destroy(_p);
			}
		}

		void m_terminate_string()
		{
			try {
				m_construct_null(m_finish);
			}
			catch (...) {
				destroy(m_start, m_finish);
			}
		}

		void m_range_initialize(const CharT* _first, const CharT* _last)
		{
			difference_type _n = _last - _first;
			m_allocate_block(_n + 1);
			m_finish = std::uninitialized_copy(_first, _last, m_start);
			m_terminate_string();
		}

	public:
		basic_string& operator=(const basic_string& str)
		{
			if (&str != this) {
				assign(str.begin(), str.end());
			}
			return *this;
		}

		/* Since C++11 */
		// basic_string& operator=(basic_string&& str)
		// {

		// }

		basic_string& operator=(const CharT* s)
		{
			return assign(s, s + Traits::length(s));
		}

		basic_string& operator=(CharT ch)
		{
			return assign(static_cast<size_type>(1), ch);
		}

		/* Since C++11 */
		// basic_string& operator=(std::initializer_list<CharT> ilist)
		// {

		// }

		reference operator[](size_type pos)
		{
			return *(m_start + pos);
		}

		const_reference operator[](size_type pos) const
		{
			return *(m_start + pos);
		}

		basic_string& operator+=(const basic_string& str)
		{
			return append(str);
		}

		basic_string& operator+=(CharT ch)
		{
			push_back(ch);
			return *this;
		}

		basic_string& operator+=(const CharT* s)
		{
			return append(s);
		}

		// Since C++11
		basic_string& operator+=(std::initializer_list<CharT> ilist)
		{
			return append(ilist.begin(), ilist.end());
		}
		// Since C++17
		// basic_string& operator+=(std::basic_string_view<CharT, Traits> sv);

	public:
		basic_string& assign(size_type count, CharT ch);

		basic_string& assign(const basic_string& str)
		{
			return assign(str.begin(), str.end());
		}

		basic_string& assign(const basic_string& str,
			size_type pos,
			size_type count)
		{
			if (pos > str.size()) {
				m_throw_out_of_range();
			}
			return assign(str.begin() + pos,
				str.begin() + pos + std::min(count, str.size() - pos));
		}

		// Since C++14
		/*
		basic_string& assign(const basic_string& str,
		size_type pos,
		size_type count = npos)
		{

		}
		*/

		basic_string& assign(const CharT& s,
			size_type count)
		{
			return assign(s, s + count);
		}

		basic_string& assign(const CharT* s)
		{
			return assign(s, s + Traits::length(s));
		}

		basic_string& assign(const CharT* first, const CharT* last);

	public:
		// Element access
		reference at(size_type pos)
		{
			if (pos >= size()) {
				m_throw_out_of_range();
			}
			return *(m_start + pos);
		}

		const_reference at(size_type pos) const
		{
			if (pos >= size()) {
				m_throw_out_of_range();
			}
			return *(m_start + pos);
		}

	public:
		// Since C++11
		// is equivalent to operator[](0)
		CharT& front()
		{
			return *m_start;
		}

		const CharT& front() const
		{
			return *m_start;
		}

		CharT& back()
		{
			return *(m_finish - 1);
		}

		const CharT& back() const
		{
			return *(m_finish - 1);
		}

	public:
		/* Since C++11 data() and c_str() perform the same function. */
		const CharT* data() const { return m_start; }
		// Since C++17
		CharT* data() { return m_start; }

		const CharT* c_str() const { return m_start; }

		// operator std::basic_string_view<CharT, Traits>() const;

	public:
		// Iterators
		iterator begin() /*noexcept*/ { return m_start; }
		const_iterator begin() const /*noexcept*/ { return m_start; }
		const_iterator cbegin() const /*noexcept*/ { return static_cast<const_iterator>(m_start); }
		iterator end() /*noexcept*/ { return m_finish; }
		const_iterator end() const /*noexcept*/ { return m_finish; }
		const_iterator cend() const /*noexcept*/ { return static_cast<const_iterator>(m_finish); }

		reverse_iterator rbegin() /*noexcept*/ { return static_cast<reverse_iterator>(m_finish); }
		const_reverse_iterator rbegin() const /*noexcept*/ { return static_cast<const_reverse_iterator>(m_finish); }
		const_reverse_iterator crbegin() const /*noexcept*/ { return static_cast<const_reverse_iterator>(m_finish); }
		reverse_iterator rend() /*noexcept*/ { return static_cast<reverse_iterator>(m_start); }
		const_reverse_iterator rend() const /*noexcept*/ { return static_cast<const_reverse_iterator>(m_start); }
		const_reverse_iterator crend() const /*noexcept*/ { return static_cast<const_reverse_iterator>(m_start); }

	public:
		// Capacity
		bool empty() const
		{
			// return begin() == end();
			return m_start == m_finish;
		}

		size_type size() const
		{
			// return std::distance(end() - begin());
			return static_cast<size_type>(m_finish - m_start);
		}

		size_type length() const
		{
			return size();
		}

		size_type max_size() const
		{
			return static_cast<size_type>(-1) / sizeof(CharT);
		}

		void reserve(size_type new_cap = 0);

		size_type capacity() const
		{
			// return static_cast<size_type>(m_end_of_storage - begin());
			return static_cast<size_type>(m_end_of_storage - begin()) - 1;
		}

	public:
		void clear()
		{
			if (!empty()) {
				Traits::assign(*m_start, m_null());
				destroy(m_start + 1, m_finish + 1);
				m_finish = m_start;
			}
		}

	public:
		//  s.insert(0, 1, 'E');
		basic_string& insert(size_type index, size_type count, CharT ch)
		{
			if (index > size()) {
				m_throw_out_of_range();
			}
			if (size() > max_size() - count) {
				m_throw_length_error();
			}
			insert(m_start + index, count, ch);
			return *this;
		}

		basic_string& insert(size_type index, const CharT* s)
		{
			if (index > size()) {
				m_throw_out_of_range();
			}
			size_type len = Traits::length(s);
			if (size() > max_size() - len) {
				m_throw_length_error;
			}
			insert(m_start + index, s, s + len);
			return *this;
		}

		basic_string& insert(size_type index, const CharT* s, size_type count)
		{
			if (index > size()){
				m_throw_out_of_range();
			}
			if (size() > max_size() - count) {
				m_throw_length_error();
			}
			insert(m_start + index, s, s + count);
			return *this;
		}

		basic_string& insert(size_type index, const basic_string& str)
		{
			if (index > size()) {
				m_throw_out_of_range();
			}
			if (size() > max_size() - str.size()) {
				m_throw_length_error();
			}
			insert(m_start + index, str.begin(), str.end());
			return *this;
		}

		// Until C++14
		basic_string& insert(size_type index, const basic_string& str,
			size_type index_str, size_type count)
		{
			if (index > size() || index_str > str.size()) {
				m_throw_out_of_range();
			}
			size_type len = min(count, str.size() - index_str);
			if (size() > max_size() - len) {
				m_throw_length_error();
			}
			insert(m_start + index,
				str.begin() + index_str,
				str.begin() + index_str + len);
			return *this;
		}

		// Since C++14
		/*
		basic_string& insert(size_type index, const basic_string& str,
		size_type index_str, size_type count = npos)
		{
		if(index > size() || index_str > str.size()) {
		m_throw_out_of_range();
		}
		size_type len = min(count, str.size() - index_str);
		if(size() > max_size() - len) {
		m_throw_length_error();
		}
		insert(m_start + index,
		str.begin() + index_str,
		str.begin() + index_str + len);
		return *this;
		}
		*/

		// Until C++11
		iterator insert(iterator pos, CharT ch)
		{
			if (pos == m_finish) {
				push_back(ch);
				return m_finish - 1;
			}
			else {
				m_insert_aux(pos, ch);
			}
		}

		// Since C++11
		// iterator insert(const_iterator pos, CharT ch);

		// Until C++11
		// void insert(iterator pos, size_t count, CharT ch);
		void insert(iterator pos, size_type count, CharT ch);
		// Since C++11
		// iterator insert(const_iterator pos, size_type count, CharT ch);

		void insert(iterator pos, const CharT* first, const CharT* last);

		// iterator insert(const_iterator pos, std::initializer_list<CharT> ilist);

	private:
		iterator m_insert_aux(iterator, CharT);


	public:
		basic_string& erase(size_type index = 0, size_type count = npos)
		{
			if (index > size()) {
				m_throw_out_of_range();
			}
			erase(m_start + index, m_start + index + min(count, size() - index));
			return *this;
		}

		// Until C++11
		iterator erase(iterator position)
		{
			Traits::move(position, position + 1, m_finish - position);
			destroy(m_finish);
			--m_finish;
			return position;
		}

		// Since C++11
		// iterator erase(const_iterator position);

		// Until C++11
		iterator erase(iterator first, iterator last)
		{
			if (first != last) {
				Traits::move(first, last, (m_finish - last) + 1);
				const iterator new_finish = m_finish - (last - first);
				destroy(new_finish + 1, m_finish + 1);
				m_finish = new_finish;
			}
		}

		// Since C++11
		// iterator erase(const_iterator first, const_iterator last); 

		void push_back(CharT ch)
		{
			if (m_finish + 1 == m_end_of_storage) {
				reserve(size() + max(size(), static_cast<size_type>(1)));
			}
			m_construct_null(m_finish + 1);
			Traits::assign(*m_finish, ch);
			++m_finish;
		}

		void pop_back()
		{
			Traits::assign(*(m_finish - 1), m_null());
			destroy(m_finish);
			--m_finish;
		}

		// append
		basic_string& append(size_type count, CharT ch);

		basic_string& append(const basic_string& str)
		{
			return append(str.begin(), str.end());
		}

		basic_string& append(const basic_string& str,
			size_type pos,
			size_type count)
		{
			if (pos > str.size()) {
				m_throw_out_of_range();
			}
			return append(str.begin() + pos,
				str.begin() + pos + std::min(count, size() - pos));
		}

		// Since C++14
		// basic_string& append(const basic_string& str,
		// 	size_type pos,
		// 	size_type count = npos);

		basic_string& append(const CharT* s, size_type count)
		{
			return append(s, s + count);
		}

		basic_string& append(const CharT* s)
		{
			return append(s, s + Traits::length(s));
		}

		basic_string& append(const CharT* first, const CharT* last);

	private:
		static CharT m_null()
		{
			return (CharT)0;
		}


	public:
		// compare
		int compare(const basic_string& str) const
		{
			return m_compare(m_start, m_finish, str.m_start, str.m_finish);
		}

		int compare(size_type pos1, size_type count1,
			const basic_string& str) const
		{
			if (pos1 > size()) {
				m_throw_out_of_range();
			}
			return m_compare(m_start + pos1,
				m_finish + pos1 + std::min(count1, size() - pos1),
				str.m_start, str.m_finish);
		}

		int compare(const CharT* s) const
		{
			return m_compare(m_start, m_finish, s, s + Traits::length(s));
		}

		int compare(size_type pos1, size_type count1,
			const CharT* s) const
		{
			if (pos1 > size()) {
				m_throw_out_of_range();
			}
			return m_compare(m_start + pos1,
				m_finish + pos1 + std::min(count1, size() - pos1),
				s, s + Traits::length(s));
		}

		int compare(size_type pos1, size_type count1,
			const CharT* s, size_type count2) const
		{
			if (pos1 > size()) {
				m_throw_out_of_range();
			}
			return m_compare(m_start + pos1,
				m_start + pos1 + std::min(count1, size() - pos1),
				s, s + count2);
		}

	public:
		static int m_compare(const CharT* first1, const CharT* last1,
			const CharT* first2, const CharT* last2)
		{
			const difference_type n1 = last1 - first1;
			const difference_type n2 = last2 - first2;
			const int cmp = Traits::compare(first1, first2, std::min(n1, n2));
			return cmp != 0 ? cmp : (n1 < n2 ? -1 : (n1 > n2 ? 1 : 0));
		}

	public:
		// replace
		basic_string& replace(size_type pos, size_type count,
			const basic_string& str)
		{
			if (pos > size()) {
				m_throw_out_of_range();
			}
			const size_type len = std::min(count, size() - pos);
			if (size() - len >= max_size() - str.size()) {
				m_throw_length_error();
			}
			return replace(m_start + pos, m_start + pos + len,
				str.begin(), str.end());
		}

		basic_string& replace(const_iterator first, const_iterator last,
			const basic_string& str)
		{
			return replace(first, last, str.begin(), str.end());
		}

		// Until C++14
		basic_string& replace(size_type pos1, size_type count1,
			const basic_string& str,
			size_type pos2, size_type count2)
		{
			if (pos1 > size() || pos2 > str.size()) {
				m_throw_out_of_range();
			}
			const size_type len1 = std::min(count1, size() - pos1);
			const size_type len2 = std::min(count2, str.size() - pos2);
			if (size() - len1 >= max_size() - len2) {
				m_throw_length_error();
			}
			return replace(m_start + pos1, m_start + pos1 + len1,
				str.begin() + pos2, str.begin() + pos2 + len2);
		}

		basic_string& replace(iterator first1, iterator last1,
			const iterator first2, const iterator last2);

		basic_string& replace(size_type pos, size_type count,
			const CharT* cstr, size_type count2)
		{
			if (pos > size()) {
				m_throw_out_of_range();
			}
			const size_type len = std::min(count, size() - pos);
			if (count2 > max_size() || size() - len >= max_size() - count2) {
				m_throw_length_error();
			}
			return replace(m_start + pos, m_start + pos + len,
				cstr, cstr + count2);
		}

		basic_string& replace(const_iterator first, const_iterator last,
			const CharT* cstr, size_type count2)
		{
			return replace(first, last, cstr, cstr + count2);
		}

		basic_string& replace(size_type pos, size_type count,
			const CharT* cstr)
		{
			if (pos > size()) {
				m_throw_out_of_range();
			}
			const size_type len = std::min(count, size() - pos);
			const size_type n2 = Traits::length(cstr);
			if (n2 > max_size() || size() - len >= max_size() - n2) {
				m_throw_length_error();
			}
			return replace(m_start + pos, m_start + pos + len,
				cstr, cstr + Traits::length(cstr));
		}

		// basic_string& replace(iterator first, iterator last, const CharT* cstr)
		// {
		// 	return replace(first2, last, cstr, cstr + Traits::length(cstr));
		// }

		basic_string& replace(const_iterator first, const_iterator last,
			const CharT* cstr)
		{
			return replace(first, last, cstr, cstr + Traits::length(cstr));
		}

		basic_string& replace(size_type pos1, size_type count1,
			size_type count2, CharT ch)
		{
			if (pos1 > size()) {
				m_throw_out_of_range();
			}
			const size_type len = std::min(count1, size() - pos1);
			if (count2 > max_size() || size() - len >= max_size() - count2) {
				m_throw_length_error();
			}
			return replace(m_start + pos1, m_start + pos1 + len,
				count2, ch);
		}

		basic_string& replace(const_iterator first, const_iterator last,
			size_type count2, CharT ch);

	private:
		template <class _InputIterator>
		void m_copy(_InputIterator _first, _InputIterator _last, iterator _result)
		{
			for (; _first != _last; ++_first, ++_last) {
				Traits::assign(*_result, *_first);
			}
		}

		void m_copy(const CharT* _first, const CharT* _last, CharT* _result)
		{
			Traits::copy(_result, _first, _last - _first);
		}

	public:
		// substr
		basic_string substr(size_type pos = 0, size_type count = npos) const
		{
			if (pos > size()) {
				m_throw_out_of_range();
			}
			return basic_string(m_start + pos,
				m_start + pos + std::min(count, size() - pos));
		}

		// copy
		size_type copy(CharT* dest, size_type count, size_type pos = 0) const
		{
			if (pos > size()) {
				m_throw_out_of_range();
			}
			const size_type len = std::min(count, size() - pos);
			Traits::copy(dest, m_start + pos, count);
			return len;
		}

		// resize
		void resize(size_type count)
		{
			return resize(count, m_null());
		}

		void resize(size_type count, CharT ch)
		{
			if (count <= size()) {
				erase(begin() + count, end());
			}
			else {
				append(count - size(), ch);
			}
		}

		// swap
		void swap(basic_string& other)
		{
			std::swap(m_start, other.m_start);
			std::swap(m_finish, other.m_finish);
			std::swap(m_end_of_storage, other.m_end_of_storage);
		}

	public:
		// Search
		size_type find(const basic_string& str, size_type pos = 0) const
		{
			return find(str.begin(), pos, str.size());
		}

		size_type find(const CharT* s, size_type pos, size_type count) const;

		size_type find(const CharT* s, size_type pos = 0) const
		{
			return find(s, pos, Traits::length(s));
		}

		size_type find(CharT ch, size_type pos = 0) const;

	public:
		size_type rfind(const basic_string& str, size_type pos = npos) const
		{
			return rfind(str.begin(), pos, str.size());
		}

		size_type rfind(const CharT* s, size_type pos, size_type count) const;

		size_type rfind(const CharT* s, size_type pos = npos) const
		{
			return rfind(s, pos, Traits::length(s));
		}

		size_type rfind(CharT ch, size_type pos = npos) const;

	public:
		size_type find_first_of(const basic_string& str, size_type pos = 0) const
		{
			return find_first_of(str.begin(), pos, str.size());
		}

		size_type find_first_of(const CharT* S, size_type pos, size_type count) const;

		size_type find_first_of(const CharT* s, size_type pos = 0) const
		{
			return find_first_of(s, pos, Traits::length(s));
		}

		size_type find_first_of(CharT ch, size_type pos = 0) const
		{
			return find(ch, pos);
		}

	public:
		size_type find_first_not_of(const basic_string& str, size_type pos = 0) const
		{
			return find_first_not_of(str.begin(), pos, str.size());
		}

		size_type find_first_not_of(const CharT* s, size_type pos, size_type count) const;

		size_type find_first_not_of(const CharT* s, size_type pos = 0) const
		{
			return find_first_not_of(s, pos, Traits::length(s));
		}

		size_type find_first_not_of(CharT ch, size_type pos = 0) const;

	public:
		size_type find_last_of(const basic_string& str, size_type pos = 0) const
		{
			return find_last_of(str.begin(), pos, str.size());
		}

		size_type find_last_of(const CharT* s, size_type pos, size_type count) const;

		size_type find_last_of(const CharT* s, size_type pos = npos)
		{
			return find_last_of(s, pos, Traits::length(s));
		}

		size_type find_last_of(CharT ch, size_type pos = npos) const
		{
			return rfind(ch, pos);
		}

	public:
		size_type find_last_not_of(const basic_string& str, size_type pos = 0) const
		{
			return find_last_not_of(str.begin(), pos, str.size());
		}

		size_type find_last_not_of(const CharT* s, size_type pos, size_type count) const;

		size_type find_last_not_of(const CharT* s, size_type pos = npos)
		{
			return find_last_not_of(s, pos, Traits::length(s));
		}

		size_type find_last_not_of(CharT ch, size_type pos = npos) const;

	};

	template <typename CharT, typename Traits, typename Alloc>
	void basic_string<CharT, Traits, Alloc>::destroy(iterator pos)
	{
		pos->~CharT();
	}

	template <typename CharT, typename Traits, typename Alloc>
	void basic_string<CharT, Traits, Alloc>::destroy(iterator first, iterator last)
	{
		for (; first != last; ++first) {
			first->~CharT();
		}
	}

	template <typename CharT, typename Traits, typename Alloc>
	basic_string<CharT, Traits, Alloc>&
		basic_string<CharT, Traits, Alloc>::assign(size_type count, CharT ch)
	{
		if (count < size()) {
			Traits::assign(m_start, count, ch);
			erase(m_start + count, m_finish);
		}
		else {
			Traits::assign(m_start, size(), count);
			append(count - size(), ch);
		}
		return *this;
	}

	template <typename CharT, typename Traits, typename Alloc>
	basic_string<CharT, Traits, Alloc>&
		basic_string<CharT, Traits, Alloc>::assign(const CharT* first,
		const CharT* last)
	{
		const difference_type n = last - first;
		if (static_cast<size_type>(n) <= size()) {
			Traits::copy(m_start, first, n);
			erase(m_start + n, m_finish);
		}
		else {
			Traits::copy(m_start, first, size());
			append(first + size(), last);
		}
	}

	template <typename CharT, typename Traits, typename Alloc>
	basic_string<CharT, Traits, Alloc>&
		basic_string<CharT, Traits, Alloc>::append(size_type count, CharT ch)
	{
		if (count > max_size() || size() > max_size() - count) {
			m_throw_length_error();
		}

		if (size() + count > capacity()) {
			reserve(size() + max(size(), count));
		}

		if (count > 0) {
			std::uninitialized_fill_n(m_finish + 1, count - 1, ch);
			try {
				m_construct_null(m_finish + count);
			}
			catch (...) {
				destroy(m_finish + 1, m_finish + count);
			}
			Traits::assign(*m_finish, ch);
			m_finish += count;
		}
		return *this;
	}

	template <typename CharT, typename Traits, typename Alloc>
	basic_string<CharT, Traits, Alloc>&
		basic_string<CharT, Traits, Alloc>::append(const CharT* first,
		const CharT* last)
	{
		if (first != last) {
			const size_type old_size = size();
			std::ptrdiff_t n = last - first;
			if (n > max_size() || old_size > max_size() - n) {
				m_throw_length_error();
			}
			if (old_size + n > capacity()) {
				const size_type len = old_size + std::max(old_size, (size_t)n) + 1;
				pointer new_start = m_allocate(len);
				pointer new_finish = new_start;
				try {
					new_finish = std::uninitialized_copy(m_start, m_finish, new_start);
					new_finish = std::uninitialized_copy(first, last, new_finish);
					m_construct_null(new_finish);
				}
				catch (...) {
					destroy(new_start, new_finish);
					m_deallocate(new_start, len);
				}
				destroy(m_start, m_finish + 1);
				m_deallocate_block();
				m_start = new_start;
				m_finish = new_finish;
				m_end_of_storage = m_start + len;
			}
		}
		return *this;
	}

	template <typename CharT, typename Traits, typename Alloc>
	basic_string<CharT, Traits, Alloc>&
		basic_string<CharT, Traits, Alloc>::replace(
		const_iterator first,
		const_iterator last,
		size_type count2, CharT ch)
	{
		const size_type len = static_cast<size_type>(last - first);
		if (len >= count2) {
			Traits::assign(first, count2, ch);
			erase(first + count2, last);
		}
		else {
			Traits::assign(first, len, ch);
			insert(last, count2 - len, ch);
		}
		return *this;
	}

	template <typename CharT, typename Traits, typename Alloc>
	basic_string<CharT, Traits, Alloc>&
		basic_string<CharT, Traits, Alloc>::replace(
		iterator first1, iterator last1,
		const iterator first2, const iterator last2)
	{
		const difference_type n = last1 - first2;
		const difference_type len = last2 - first2;
		if (len >= n) {
			m_copy(first2, last2, first1);
			erase(first1 + n, last1);
		}
		else {
			const CharT* m = first2 + len;
			m_copy(first2, m, first1);
			insert(last1, m, last2);
		}
		return *this;
	}

	template <typename CharT, typename Traits, typename Alloc>
	typename basic_string<CharT, Traits, Alloc>::size_type
		basic_string<CharT, Traits, Alloc>::find(const CharT* s,
		size_type pos, size_type count) const
	{
		if (pos + count > size()) {
			return npos;
		}
		else {
			const const_iterator result =
				std::search(
				m_start + pos, m_finish,
				s, s + count);
			return result != m_finish ? result - begin() : npos;
		}
	}

	template <typename CharT, typename Traits, typename Alloc>
	typename basic_string<CharT, Traits, Alloc>::size_type
		basic_string<CharT, Traits, Alloc>::find(CharT ch,
		size_type pos) const
	{
		if (pos >= size()) {
			return npos;
		}
		else {
			const const_iterator result =
				find_if(
				m_start + pos, m_start,
				bind2nd(_Eq_traits<Traits>(), ch));
			return result != m_finish ? result - begin() : npos;
		}
	}

	template <typename CharT, typename Traits, typename Alloc>
	typename basic_string<CharT, Traits, Alloc>::size_type
		basic_string<CharT, Traits, Alloc>::rfind(const CharT* s,
		size_type pos, size_type count) const
	{
		const size_t len = size();

		if (count > len) {
			return npos;
		}
		else if (count == 0) {
			return std::min(len, pos);
		}
		else {
			const const_iterator last = begin() + std::min(len - count, pos) + count;
			const const_iterator result =
				std::find_end(begin(), last,
				s, s + count, _Eq_traits<Traits>());
			return result != last ? result - begin() : npos;
		}
	}

	template <typename CharT, typename Traits, typename Alloc>
	typename basic_string<CharT, Traits, Alloc>::size_type
		basic_string<CharT, Traits, Alloc>::rfind(CharT ch,
		size_type pos) const
	{
		const size_type len = size();

		if (len < 1) {
			return npos;
		}
		else {
			const const_iterator last = begin() + std::min(len - 1, pos) + 1;
			const_reverse_iterator rresult =
				std::find_if(const_reverse_iterator(last), rend(),
				std::bind2nd(_Eq_traits<Traits>(), ch));

			return rresult != rend() ? (rresult.base() - 1) - begin() : npos;
		}
	}

	template <typename CharT, typename Traits, typename Alloc>
	typename basic_string<CharT, Traits, Alloc>::size_type
		basic_string<CharT, Traits, Alloc>::find_first_of(const CharT* s,
		size_type pos, size_type count) const
	{
		if (pos >= size()) {
			return npos;
		}
		else {
			const_iterator result = std::find_first_of(begin() + pos, end(),
				s, s + count,
				_Eq_traits<Traits>());
			return result != m_finish ? result - begin() : npos;
		}
	}

	template <typename CharT, typename Traits, typename Alloc>
	typename basic_string<CharT, Traits, Alloc>::size_type
		basic_string<CharT, Traits, Alloc>::find_first_not_of(const CharT* s, size_type pos, size_type count) const
	{
		if (pos > size()) {
			return npos;
		}
		else {
			const_iterator result = std::find_if(m_start + pos, m_finish,
				_Not_within_traits<Traits>(s, s + count));
			return result != m_finish ? result - m_start : npos;
		}
	}

	template <typename CharT, typename Traits, typename Alloc>
	typename basic_string<CharT, Traits, Alloc>::size_type
		basic_string<CharT, Traits, Alloc>::find_first_not_of(CharT ch, size_type pos) const
	{
		if (pos > size()) {
			return npos;
		}
		else {
			const_iterator result =
				std::find_if(begin() + pos, end(),
				std::not1(std::bind2nd(_Eq_traits<Traits>(), ch)));
			return result != m_finish ? result - begin() : npos;
		}
	}

	template <typename CharT, typename Traits, typename Alloc>
	typename basic_string<CharT, Traits, Alloc>::size_type
		basic_string<CharT, Traits, Alloc>::find_last_of(const CharT* s, size_type pos, size_type count) const
	{
		const size_type len = size();

		if (len < 1) {
			return npos;
		}
		else {
			const const_iterator last = m_start + std::min(len - 1, pos) + 1;
			const const_reverse_iterator rresult =
				std::find_first_of(const_reverse_iterator(last), rend(),
				s, s + count,
				_Eq_traits<Traits>());
			return rresult != rend() ? (rresult.base() - 1) - m_start : npos;
		}
	}

	template <typename CharT, typename Traits, typename Alloc>
	typename basic_string<CharT, Traits, Alloc>::size_type
		basic_string<CharT, Traits, Alloc>::find_last_not_of(const CharT* s, size_type pos, size_type count) const
	{
		const size_type len = size();

		if (len < 1) {
			return npos;
		}
		else {
			const const_iterator last = begin() + std::min(len - 1, pos) + 1;
			const const_reverse_iterator rresult =
				std::find_if(const_reverse_iterator(last), rend(),
				_Not_within_traits<Traits>(s, s + count));
			return rresult != rend() ? (rresult.base() - 1) - m_start : npos;
		}
	}


	template <typename CharT, typename Traits, typename Alloc>
	typename basic_string<CharT, Traits, Alloc>::size_type
		basic_string<CharT, Traits, Alloc>::find_last_not_of(CharT ch, size_type pos) const
	{
		const size_type len = size();

		if (len < 1) {
			return npos;
		}
		else {
			const const_iterator last = begin() + std::min(len - 1, pos) + 1;
			const_reverse_iterator rresult =
				std::find_if(const_reverse_iterator(last), rend(),
				std::not1(std::bind2nd(_Eq_traits<Traits>(), ch)));
			return rresult != rend() ? (rresult.base() - 1) - begin() : npos;
		}
	}

	// Non-member functions
	// Operator+
	template <typename CharT, typename Traits, typename Alloc>
	inline basic_string<CharT, Traits, Alloc>
		operator+(const basic_string<CharT, Traits, Alloc>& lhs,
		const basic_string<CharT, Traits, Alloc>& rhs)
	{
		typedef basic_string<CharT, Traits, Alloc> _Str;
		typedef typename _Str::_Reserve_t _Reserve_t;
		_Reserve_t _reserve;
		_Str _result(_reserve, lhs.size() + rhs.size(), lhs.get_allocator());
		_result.append(lhs);
		_result.append(rhs);
		return _result;
	}

	template <typename CharT, typename Traits, typename Alloc>
	inline basic_string<CharT, Traits, Alloc>
		operator+(const CharT* lhs,
		const basic_string<CharT, Traits, Alloc>& rhs)
	{
		typedef basic_string<CharT, Traits, Alloc> _Str;
		typedef typename _Str::_Reserve_t _Reserve_t;
		_Reserve_t _reserve;
		const std::size_t n = Traits::length(lhs);
		_Str _result(_reserve, n + rhs.size());
		_result.append(lhs, lhs + n);
		_result.append(rhs);
		return _result;
	}

	template <typename CharT, typename Traits, typename Alloc>
	inline basic_string<CharT, Traits, Alloc>
		operator+(CharT lhs,
		const basic_string<CharT, Traits, Alloc>& rhs)
	{
		typedef basic_string<CharT, Traits, Alloc> _Str;
		typedef typename _Str::_Reserve_t _Reserve_t;
		_Reserve_t _reserve;
		_Str _result(_reserve, 1 + rhs.size());
		_result.push_back(lhs);
		_result.append(rhs);
		return _result;
	}

	template <typename CharT, typename Traits, typename Alloc>
	inline basic_string<CharT, Traits, Alloc>
		operator+(const basic_string<CharT, Traits, Alloc>& lhs,
		const CharT* rhs)
	{
		typedef basic_string<CharT, Traits, Alloc> _Str;
		typedef typename _Str::_Reserve_t _Reserve_t;
		_Reserve_t _reserve;
		const size_t n = Traits::length(rhs);
		_Str _result(_reserve, lhs.size() + n, lhs.get_allocator());
		_result.append(lhs);
		_result.append(rhs, rhs + n);
		return _result;
	}

	template <typename CharT, typename Traits, typename Alloc>
	inline basic_string<CharT, Traits, Alloc>
		operator+(const basic_string<CharT, Traits, Alloc>& lhs,
		CharT rhs)
	{
		typedef basic_string<CharT, Traits, Alloc> _Str;
		typedef typename _Str::_Reserve_t _Reserve_t;
		_Reserve_t _reserve;
		_Str _result(_reserve, lhs.size() + 1);
		_result.append(lhs);
		_result.push_back(rhs);
		return _result;
	}

	template <class _CharT, class _Traits, class _Alloc>
	std::basic_ostream<_CharT, _Traits>&
		operator<<(std::basic_ostream<_CharT, _Traits>& _os,
		const basic_string<_CharT, _Traits, _Alloc>& _s)
	{
		typename std::basic_ostream<_CharT, _Traits>::sentry _sentry(_os);
		bool _ok = false;

		if (_sentry) {
			_ok = true;
			std::size_t _n = _s.size();
			std::size_t _pad_len = 0;
			const bool _left = (_os.flags() & std::ios::left) != 0;
			const std::size_t _w = _os.width(0);
			std::basic_streambuf<_CharT, _Traits>* _buf = _os.rdbuf();

			if (_w != 0 && _n < _w)
				_pad_len = _w - _n;

			if (!_left)
				_ok = _sgi_string_fill(_os, _buf, _pad_len);

			_ok = _ok &&
				_buf->sputn(_s.data(), std::streamsize(_n)) == std::streamsize(_n);

			if (_left)
				_ok = _ok && _sgi_string_fill(_os, _buf, _pad_len);
		}

		if (!_ok)
			_os.setstate(std::ios_base::failbit);

		return _os;
	}

	template <class _CharT, class _Traits, class _Alloc>
	std::basic_istream<_CharT, _Traits>&
		operator>>(std::basic_istream<_CharT, _Traits>& _is,
		basic_string<_CharT, _Traits, _Alloc>& _s)
	{
		typename std::basic_istream<_CharT, _Traits>::sentry _sentry(_is);

		if (_sentry) {
			std::basic_streambuf<_CharT, _Traits>* _buf = _is.rdbuf();
			const std::ctype<_CharT>& _ctype = std::use_facet<std::ctype<_CharT> >(_is.getloc());

			_s.clear();
			size_t _n = _is.width(0);
			if (_n == 0)
				_n = static_cast<size_t>(-1);
			else
				_s.reserve(_n);


			while (_n-- > 0) {
				typename _Traits::int_type _c1 = _buf->sbumpc();
				if (_Traits::eq_int_type(_c1, _Traits::eof())) {
					_is.setstate(std::ios_base::eofbit);
					break;
				}
				else {
					_CharT _c = _Traits::to_char_type(_c1);

					if (_ctype.is(std::ctype<_CharT>::space, _c)) {
						if (_Traits::eq_int_type(_buf->sputbackc(_c), _Traits::eof()))
							_is.setstate(std::ios_base::failbit);
						break;
					}
					else
						_s.push_back(_c);
				}
			}

			// If we have read no characters, then set failbit.
			if (_s.size() == 0)
				_is.setstate(std::ios_base::failbit);
		}
		else
			_is.setstate(std::ios_base::failbit);

		return _is;
	}

	template <class _CharT, class _Traits>
	inline bool
		_sgi_string_fill(std::basic_ostream<_CharT, _Traits>& _os,
		std::basic_streambuf<_CharT, _Traits>* _buf,
		std::size_t _n)
	{
		_CharT _f = _os.fill();
		size_t _i;
		bool _ok = true;

		for (_i = 0; _i < _n; _i++)
			_ok = _ok && !_Traits::eq_int_type(_buf->sputc(_f), _Traits::eof());
		return _ok;
	}

#ifndef XSTL_STRING
#define XSTL_STRING

	typedef basic_string<char, std::char_traits<char>, xstl::allocator<char>> string;
	typedef basic_string<wchar_t, std::char_traits<wchar_t>, xstl::allocator<wchar_t>> wstring;

#endif // XSTL_STRING
}

#endif // LIBRARY_XSTL_STRING_H__