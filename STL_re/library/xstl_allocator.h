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
	inline T* _allocate(std::ptrdiff_t size, T*)
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
	inline void _deallocate(T* buffer, std::size_t /* n */)
	{
		::operator delete(buffer);
	}

	template <typename T1, typename T2>
	inline void _construct(T1* p, const T2& val)
	{
		new ((void *)p) T1(val);
	}

	template <typename T>
	inline void _destroy(T* ptr)
	{
		ptr->~T();
	}

	// simple_alloc
	template <typename T, typename Alloc>
	class simple_alloc
	{
	public:
		static T* allocate(std::size_t n)
		{
			// return 0 == n ? 0 : (T*) Alloc::allocate(n * sizeof(T));
			// return 0 == n ? 0 : (T*) _allocate(n * sizeof(T));
			return _allocate(static_cast<std::ptrdiff_t>(n * sizeof(T)),
				static_cast<T*>(0));
		}

		static T* allocate(void)
		{
			// return (T*)Alloc::allocate(sizeof(T));
			// return (T*)_allocate(sizeof(T));
			return _allocate(static_cast<std::ptrdiff_t>(sizeof(T)),
				static_cast<T*>(0));
		}

		static void deallocate(T* p, std::size_t n)
		{
			if(0 != n) {
				// Alloc::deallocate(p, n * sizeof(T));
				_deallocate(p, n * sizeof(T));
			}
		}

		static void deallocate(T* p)
		{
			// Alloc::deallocate(p, sizeof(T));
			_deallocate(p, sizeof(T));
		}
	};


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

		// template <class U> struct rebind {
		// 	typedef allocator<U> other;
		// };
		template <typename U>
		using rebind			= allocator<U>;
		// using is_always_equal 	= std::true_type;

	public:
		// Member functions
		/*
			costructor && destructor
		*/
		allocator() {}
		allocator(const allocator& other) {}
		template <class U> allocator(const allocator<U>& other) {}
		~allocator() {}

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
			/*
				In sgi stl only return &x;
			*/
		}

		/*
			allocate : allocates uninitialized storage
		*/
		pointer allocate(size_type n, const void * hint = 0)
		{
			return _allocate(static_cast<difference_type>(n),
				static_cast<pointer>(0));
			/*
				return n != 0 ? static_cast<pointer>(_Alloc::allocte(n * sizeof(T*))) : 0;
			*/
		}

		/*
			T* allocate(std::size_t n)
			{
				return _allocate(static_cast<difference_type>(n),
					static_cast<pointer>(0));
			}
		*/

		/*
			deallocate : deallocates storage
		*/
		void deallocate(T* p, std::size_t n)
		{
			_deallocate(p, n * sizeof(T));
		}

		/*
			max_size : returns the largest supported allocation size
		*/
		size_type max_size() const
		{
			return static_cast<size_type>(UINT_MAX / sizeof(T));
			/*
				return size_t(-1) / sizeof(T);
			*/
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

	template<>
	class allocator<void>
	{
	public:
		// Member types
		using value_type 		= void;
		using pointer	 		= void*;
		using const_pointer 	= const void*;
		using size_type  		= std::size_t;
		using difference_type   = std::ptrdiff_t;
		// using propagate_on_container_move_assignment = std::true_type;
		
		// template <class U> struct rebind {
		// 	typedef allocator<U> other;
		// };
		template <typename U>
		using rebind			= allocator<U>;
		
		// using is_always_equal 	= std::true_type; 
	};

	template <typename T1, typename T2>
	bool operator==(const allocator<T1>& lhs, const allocator<T2>& rhs)
	{
		return true;
	}

	template <typename T1, typename T2>
	bool operator!=(const allocator<T1>& lhs, const allocator<T2>& rhs)
	{
		return false;
	}
}

#endif // LIBRARY_XSTL_ALLOCATOR_H__