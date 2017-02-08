#ifndef LIBRARY_XSTL_STRING_H__
#define LIBRARY_XSTL_STRING_H__

#include <cstdlib>				// for std::size_t, std::ptrdiff_t
#include <iterator>				// for std::reverse_iterator 
#include <functional>

#ifndef _STL_THROW_ERROR
#define _STL_THROW_ERROR

#define _stl_throw_length_error(error) { std::cerr << "Length error" << error << std::endl; exit(1); }
#define _stl_throw_out_of_range(error) { std::cerr << "Out of range " << error << std::endl; exit(1); }

#endif // _STL_THROW_ERROR

namespace xstl
{
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

		using _Alloc_type = simple_alloc<T, Alloc>;

		T* m_allocate(size_t _n) { return _Alloc_type::allocate(_n); }

		void m_deallocate(T* _p, size_t _n)
		{
			if(_p) {
				_Alloc_type::deallocate(_p, _n);
			}
		}

		void m_allocate_block(size_t _n)
		{
			if(_n <= max_size()) {
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
		using const_pointer	= const value_type*;
		using reference	= value_type&;
		using const_reference = const value_type&;
		using size_type	= std::size_t;
		using difference_type = std::ptrdiff_t;
		using iterator = value_type*;
		using const_iterator = const value_type*;
		
		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;

		using _Base = _String_base<CharT, Alloc>;

	public:
		typedef typename _Base::allocator_type allocator_type;
		allocator_type get_allocator() const { return _Base::get_allocator(); }

	protected:
		using _Base::m_allocate;
		using _Base::m_deallocate;
		using _Base::m_allocate_block
		using _Base::m_deallocate_block;
		using _Base::m_throw_out_of_range;
		using _Base::m_throw_length_error;

		using _Base::m_start;
		using _Base::m_finish;
		using _Base::m_end_of_storage;

	public:
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
			if(pos > other.size()) {
				m_throw_out_of_range();
			}
			else {
				m_range_initialize(other.beign() + pos, 
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

	private:
		void m_construct_null(CharT* _p)
		{
			constuct(_p);
			try {
				*_p = static_cast<CharT>(0);
			}
			catch(...) {
				destroy(_p);
			}
		}

		void m_terminate_string()
		{
			try {
				m_construct_null(m_finish);
			}
			catch(...) {
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
			if(&str != this) {
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

		basic_string& assign(size_type count, CharT ch);

		basic_string& assign(const basic_string& str)
		{
			return assign(str.begin(), str.end());
		}

		basic_string& assign(const basic_string& str,
			size_type pos,
			size_type count);
		{
			if(pos > str.size()) {
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

	};

	template <typename CharT, typename Traits, typename Alloc>
	basic_string<CharT, Traits, Alloc>& 
	basic_string<CharT, Traits, Alloc>::assign(size_type count, CharT ch)
	{
		if(count < size()) {
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
	basic_string<CharT, Traits, Alloc>::assign(const CharT* first, const CharT* last)
	{
		const difference_type n = last - first;
		if(static_cast<size_type>(n) <= size()) {
			Traits::copy(m_start, first, n);
			erase(m_start + n, m_finish);
		}
		else {
			Traits::copy(m_start, first, size());
			append(first + size(), last);
		}
	}

}

#endif // LIBRARY_XSTL_STRING_H__