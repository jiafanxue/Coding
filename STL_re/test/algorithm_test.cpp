#include <algorithm>
#include <iterator>
#include <iostream>
#include <vector>
#include <string>
#include "../library/xstl_algorithm.h"

int main(int argc, char const *argv[])
{
	std::vector<int> v{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	// fill
	xstl::algo::fill(v.begin(), v.end(), 1);
	// fill_n
	xstl::algo::fill_n(std::back_inserter(v), 5, -1);
	// copy
	xstl::algo::copy(v.cbegin(), v.cend(), std::ostream_iterator<int>(std::cout, " "));
	std::cout << std::endl;
	// copy_n
	std::string in = "123456";
	std::string out;
	xstl::algo::copy_n(in.cbegin(), 4, std::back_inserter(out));
	std::cout << out << std::endl;
	// copy_backward
	std::vector<int> from_vector;
	from_vector.reserve(10);
	for(int i = 1; i <= 10; ++i) {
		from_vector.push_back(i);
	}
	std::vector<int> to_vector(15);
	xstl::algo::copy_backward(from_vector.cbegin(), from_vector.cend(), to_vector.end());
	xstl::algo::copy(to_vector.cbegin(), to_vector.cend(), std::ostream_iterator<int>(std::cout, " "));
	std::cout << std::endl;

	// unique
	std::vector<int> vec{1, 1, 2, 3, 3, 3, 4};
    /*auto last = */xstl::algo::unique(vec.begin(), vec.end());
   	xstl::algo::copy(vec.cbegin(), vec.cend(), std::ostream_iterator<int>(std::cout, " "));
	std::cout << std::endl;

	std::string s = "wanna go       to     space?";
	std::cout << s << std::endl;
	auto send = xstl::algo::unique(s.begin(), s.end(), [](char l, char r){
		return std::isspace(l) && std::isspace(r) && l == r;
	});
	std::cout << std::string(s.begin(), send) << std::endl;
	return 0;
}