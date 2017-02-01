#include <iostream>
#include <string>
#include <memory>
#include "../library/xstl_allocator.h"

int main(int argc, char const *argv[])
{
	xstl::allocator<int> a1;
	int* a = a1.allocate(10);

	a[9] = 7;

	std::cout << a[9] << std::endl;

	a1.deallocate(a, 10);

	std::allocator<std::string> a2;

	// decltype(a1)::rebind<std::string>::other a2_1;

	// std::allocator_traits<decltype(a1)>::rebind_alloc<std::string> a2_2;

	std::string* s = a2.allocate(2);

	a2.construct(s, "foo");
	a2.construct(s + 1, "bar");

	std::cout << s[0] << " " << s[1] << std::endl;

	a2.destroy(s);
	a2.destroy(s + 1);
	a2.deallocate(s, 2);

	return 0;
}