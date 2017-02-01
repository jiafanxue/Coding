#ifndef LIBRARY_XSTL_ALLOCATOR_H__
#define LIBRARY_XSTL_ALLOCATOR_H__

#include <climits>			// for UINT_MAX
#include <cstddef>			// for std::ptrdiff_t, std::size_t
#include <cstdlib>			// for exit()
#include <iostream>			// for std::cerr, std::endl;
#include <new>				// for placement new
#include <type_traits>		// for std::true_type

namespace xstl
{
	template <typename T>
	inline T* _allocate(ptrdiff_t size, T*)
	{
		std::set_new_handler(0);
		T* tmp = (T*)(::operator new((static_cast<size_t>(size * sizeof(T)))));
		if(tmp == 0) {
			std::cerr << "Out of memory" << std::endl;
			exit(1);
		}
		return tmp;
	}

	template <typename T>
	inline void _deallocate(T* buffer, std::size_t n)
	{
		for(std::size_t i = 0; i < n; ++i) {
			::operator delete(buffer + i);
		}
	}

	template <typename T1, typename T2>
	inline void _construct(T1* p, const T2& val)
	{
		new ((void *)p)T1(val);
	}

	template <typename T>
	inline void _destroy(T* ptr)
	{
		ptr->~T();
	}

	template <typename T>
	class allocator
	{
	public:
		// Member types
		using value_type 		= T;
		using pointer	 		= T*;
		using const_pointer 	= const T*;
		using reference     	= T&;
		using const_reference	= const T&;
		using size_type  		= std::size_t;
		using difference_type   = std::ptrdiff_t;
		// using propagate_on_container_move_assignment = std::true_type;
		// using rebind 			= template <class U> struct rebind { typedef allocator<U> other;};
		// using is_always_equal 	= std::true_type;

	public:
		// Member functions
		// allocator();
		// allocator(const allocator& other);
		// template <class U>
		// allocator(const allocator<U>& other);
		// ~allocator();

		/*
			address : obtains the address of an object, even if operator& is overloaded
		*/
		pointer address(reference x) const
		{
			return static_cast<reference>(&x);
		}

		const_pointer address(const_reference x) const
		{
			return static_cast<const_reference>(&x);
		}

		/*
			allocate : allocates uninitialized storage
		*/
		pointer allocate(size_type n, const void * hint = 0)
		{
			return _allocate(static_cast<difference_type>(n),
				static_cast<pointer>(0));
		}


		// T* allocate(std::size_t n)
		// {
		// 	return _allocate(static_cast<difference_type>(n),
		// 		static_cast<pointer>(0));
		// }

		/*
			deallocate : deallocates storage
		*/
		void deallocate(T* p, std::size_t n)
		{
			_deallocate(p, n);
		}

		/*
			max_size : returns the largest supported allocation size
		*/
		size_type max_size() const
		{
			return static_cast<size_type>(UINT_MAX / sizeof(T));
		}

		/*
			construct : constructs an object in allocated storage
		*/
		void construct(pointer p, const_reference val)
		{
			_construct(p, val);
		}

		/*
			destroy : destructs an object in allocated storage
		*/
		void destroy(pointer p)
		{
			_destroy(p);
		}
	};
}


#endif // LIBRARY_XSTL_ALLOCATOR_H__