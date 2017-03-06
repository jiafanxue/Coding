#include <algorithm>
#include <iterator>
#include <iostream>
#include <vector>
#include <string>
#include "../library/xstl_algorithm.h"

struct Sum
{
	Sum() : sum{0} { }
	void operator()(int n) { sum += n; }
	int sum;
};

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
    xstl::algo::unique(vec.begin(), vec.end());
   	xstl::algo::copy(vec.cbegin(), vec.cend(), std::ostream_iterator<int>(std::cout, " "));
	std::cout << std::endl;

	std::string s = "wanna go       to     space?";
	std::cout << s << std::endl;
	auto send = xstl::algo::unique(s.begin(), s.end(), [](char l, char r){
		return std::isspace(l) && std::isspace(r) && l == r;
	});
	std::cout << std::string(s.begin(), send) << std::endl;

	// unique_copy
	std::string s1 = "The    string    with many   spaces!";
	std::cout << "before: " << s1 << std::endl;
	std::string s2;
	std::string s3;
	xstl::algo::unique_copy(s1.begin(), s1.end(), std::back_inserter(s2));
	std::cout << "s2: " << s2 << std::endl;
	xstl::algo::unique_copy(s1.begin(), s1.end(), std::back_inserter(s3),
                     [](char c1, char c2){ return c1 == ' ' && c2 == ' '; });
	std::cout << "after: " << s3 << std::endl;

	// for_each
	std::vector<int> nums{3, 4, 2, 8, 15, 267};
	auto print = [](const int& i) { std::cout << " " << i; };

	std::cout << "before:";
	std::for_each(nums.begin(), nums.end(), print);
	std::cout << std::endl;

	std::for_each(nums.begin(), nums.end(), [](int& n) { n++; });

	Sum sss = std::for_each(nums.begin(), nums.end(), Sum());

	std::cout << "after: ";
	std::for_each(nums.begin(), nums.end(), print);
	std::cout << std::endl;
	std::cout << "sum:" << sss.sum << std::endl;

	return 0;
}