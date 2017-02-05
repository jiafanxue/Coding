#include <iostream>
#include <iterator>
#include <initializer_list>
#include "../library/xstl_allocator.h"
#include "../library/xstl_vector.h"
using namespace std;

int main(int argc, char const *argv[])
{
	xstl::vector<int> vec1 = { 1, 2, 3 };
	for (unsigned i = 0; i < vec1.size(); ++i) {
		cout << vec1[i] << " "; 
	}
	cout << endl;

	xstl::vector<int> vec2(vec1.begin(), vec1.end());
	for (unsigned i = 0; i < vec2.size(); ++i) {
		cout << vec2[i] << " ";
	}
	cout << endl;

	xstl::vector<int> vec3(vec1);
	for (unsigned i = 0; i < vec3.size(); ++i) {
		cout << vec3[i] << " ";
	}
	cout << endl;

	xstl::vector<int> vec4(3, 0);
	for (unsigned i = 0; i < vec4.size(); ++i) {
		cout << vec4[i] << " ";
	}
	cout << endl;

	xstl::vector<int> vec5;
	vec5 = vec1;
	for (unsigned i = 0; i < vec5.size(); ++i) {
		cout << vec5[i] << " ";
	}
	cout << endl;

	xstl::vector<int> vec6;
	vec6.assign(5, 1);
	for (unsigned i = 0; i < vec6.size(); ++i) {
		cout << vec6[i] << " ";
	}
	cout << endl;

	cout << vec1.at(1) << endl;
	cout << vec1[1] << endl;
	cout << vec1.front() << endl;
	cout << vec1.back() << endl;
	int* p = vec1.data();
	*p = 10;
	++p;
	*p = 20;
	*++p = 100;

	for (xstl::vector<int>::iterator i = vec1.begin(); i != vec1.end(); ++i) {
		cout << *i << " ";
	}
	cout << endl;

	for (xstl::vector<int>::const_iterator i = vec1.cbegin(); i != vec1.cend(); ++i) {
		cout << *i << " ";
	}
	cout << endl;

	for (xstl::vector<int>::reverse_iterator i = vec1.rbegin(); i != vec1.rend(); ++i) {
		cout << *i << " ";
	}
	cout << endl;

	for (xstl::vector<int>::const_reverse_iterator i = vec1.crbegin(); i != vec1.crend(); ++i) {
		cout << *i << " ";
	}
	cout << endl;

	if (!vec1.empty()) {
		cout << "vec1 is not empty" << endl;
	}
	else {
		cout << "vec1 is empty" << endl;
	}

	cout << vec1.size() << endl;

	cout << vec1.max_size() << endl;

	cout << vec1.capacity() << endl;

	vec1.clear();
	if (vec1.empty()) {
		cout << "After clear() the vec1 is empty" << endl;
	}

	xstl::vector<int>::iterator i = vec1.begin();
	vec1.insert(i, 100);
	cout << vec1[0] << endl;

	vec1.erase(vec1.begin());

	if (vec1.empty()) {
		cout << "After erase() the vec1 is empty" << endl;
	}

	vec1.push_back(100);
	cout << vec1[0] << endl;

	vec1.pop_back();
	cout << vec1.size() << endl;

	vec2.resize(5);
	for (unsigned i = 0; i < vec2.size(); ++i) {
		cout << vec2[i] << " ";
	}
	cout << endl;

	cout << "Before swap ---" << endl;
	for (unsigned i = 0; i < vec4.size(); ++i) {
		cout << vec4[i] << " ";
	}
	cout << endl;
	cout << "After swap ---" << endl;
	vec4.swap(vec2);
	for (unsigned i = 0; i < vec4.size(); ++i) {
		cout << vec4[i] << " ";
	}
	cout << endl;


	return 0;
}