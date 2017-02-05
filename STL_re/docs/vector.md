# Vector

## Introduction
This is the first version of the vector, can only achieve a simple operation.

## Member Type
The type of the Allocator:

	vector::value_type
	vector::allocator_type
	vector::size_type
	vector::difference_type
	vector::reference
	vector::const_reference
	vector::pointer
	vector::const_pointer
	vector::iterator
	vector::const_iterator
	vector::reverse_iterator
	vector::const_reverse_iterator

## Member Function
The interface function of the Allocator:

	Member functions:
		vector();										-- Constructs the vector
		~vector();										-- Destructs the vector
		vector<T>& operator=(const vector<T>& other); 	-- Assigned values to container
		void assign(size_type count, const T& value);	-- Assigned values to container
		allocator_type get_allocator() const 			-- Returns the allocator associated with the container

	Element access:
		reference at(size_type pos);					-- Returns a reference to the element at specified 
														   	loaction pos, with bounds checking
		const_reference at(size_type pos);
		reference operaotr[](size_type pos);			-- Returns a reference to the element at specified
															loaction pos, with bounds checking
		const_reference operaotr[](size_type pos);		

		reference front();								-- Returns reference to the first element in the container.
		const_reference front();
		reference back();								-- Returns reference to the last element in the container
		const_reference back();
		T* data();										-- Returns pointer to the underlying array serving as 
		const T* data() const;								element storage.

	Iterators:
		iterator begin() noexcept
		const_iterator begin() const noexcept
		iterator end() noexcept
		const_iterator end() const noexcept
	
	Capacity:
		bool empty() const noexcept
		size_type size() const noexcept
		size_type max_size() const noexcept
		void reserve() 
		size_type capacity() const noexcept
		void shrink_to_fit()							-- It is a non-binding request to reduce capacity() to size().

	Modifiers:
		void clear()
		insert()
		iterator emplace(const_iterator pos, Arg&&...args)
		erase()
		void push_bacK(..)
		void emplace_back(...)
		void pop_back()
		void resize(size_type count, T value = T())
		void swap(vector& other)