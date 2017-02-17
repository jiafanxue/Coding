#include <iostream>
#include <iterator>
#include <cassert>
#include <initializer_list>
#include "../library/xstl_allocator.h"
#include "../library/xstl_string.h"
using namespace std;

int main(int argc, char const *argv[])
{
	{
		xstl::string s;
		assert(s.empty() && (s.length() == 0) && (s.size() == 0));
	}

	{
		xstl::string s(4, '=');
		cout << s << endl;
	}

	{
		xstl::string const other("Exemplary");
		// string::string(string const& other, size_type pos, size_type count)
		xstl::string s(other, 0, other.length() - 1);
		cout << s << endl; // "Exemplar"
	}

	{
		// string::string(charT const* s, size_type count)
		xstl::string s("C-style string", 7);
		cout << s << endl; // "C-style"
	}

	{
		// string::string(charT const* s)
		xstl::string s("C-style\0string");
		cout << s << endl; // "C-style"
	}

	{
		char mutable_c_str[] = "another C-style string";
		// string::string(InputIt first, InputIt last)
		xstl::string s(std::begin(mutable_c_str) + 8, std::end(mutable_c_str) - 1);
		cout << s << endl; // "C-style string"
	}

	{
		xstl::string const other("Exemplar");
		xstl::string s(other);
		cout << s << endl; // "Exemplar"
	}

	{
		// string::string(string&& str)
		//xstl::string s(xstl::string("C++ by ") + xstl::string("example"));
		//cout << s << endl; // "C++ by example"
	}

	{
		// string(std::initializer_list<charT> ilist)
		//xstl::string s({ 'C', '-', 's', 't', 'y', 'l', 'e' });
		//cout << s << endl; // "C-style"
	}

	return 0;
}