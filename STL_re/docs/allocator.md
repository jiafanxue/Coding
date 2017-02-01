## Introduction
This is the first version of the allocator, can only achieve a simple operation.

## Function && Member type

The type of the Allocator:
	allocator::value_type
	allocator::pointer
	allocator::const_pointer
	allocator::reference
	allocator::const_reference
	allocator::size_type
	allocator::difference_type
	allocator::rebind   		--- template <class U> struct rebind { typedef allocator<U> other; };

The interface function of the Allocator:
	allocator()										-- Default constructor
	allocaotr(const allocator&)						-- Copy constructor
	template<class U> allocator(const allocator<U>& other );			-- template construcotr
	~allocator()									-- Destructor

	pointer address(reference x) const 				-- Return the address of object. e.g. a.address(x) <=> &x 
	const_pointer address(const_reference x) const  -- Return the const address of object 
	pointer allocate(size_type n, const void* = 0)  -- Allocate memory, enough to store the n size of T objects. The
	 												   second parameter is used prompting
	void deallocate(pointer p, size_type n)			-- Release memory
	size_type max_size() const 						-- Return the max size of the memory
	void construct(pointer p, const T& x)			-- Equal 'new((void*)p) T(x)'
	void destroy(pointer p)							-- Equal 'p->~T()'