#include <iostream>
#include <fstream>
#include <boost/format.hpp>
using namespace std;
using namespace boost;

int main(int argc, char const *argv[])
{
	cout << "Please input filename: ";
	string filename;
	cin >> filename;
	fstream s(filename, ios::out | ios::trunc);

	if (!s.is_open()) {
		cout << "failed to open " << filename << endl;
	}
	else {
		format fmt("E:/WorkFor2017/ML/test_image/att_faces/s%d/%d.pgm;%d\n");
		for(int i = 1; i <= 41; ++i) {
			for(int j = 1; j <= 10; ++j) {
				fmt %i %j %(i-1);
				s << fmt;
			}
		}
	}
	s.close();
	return 0;
}
