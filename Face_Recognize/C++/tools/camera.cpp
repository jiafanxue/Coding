#include <iostream>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

int main(int argc, char const *argv[])
{
	// 创建VideoCapture对象，打开默认摄像机
	VideoCapture capture(0);
	// 检查是否打开摄像头
	if (!capture.isOpened()) {
		cerr << "VideoCapture construct error!" << endl;
		exit(1);
	}

	Mat dst;
	Mat frame;
	Mat changeframe;
	int i = 1;
	while (1) {
		// 从摄像机中获取一个新的框架
		capture >> frame;
		if (frame.empty()) {
			cerr << "New frame is empty" << endl;
			exit(1);
		}

		// 显示灰度图
		imshow("MyPicture", frame);

		// 转为灰度图
		cvtColor(frame, changeframe, CV_BGR2GRAY);

		// 改变图片大小
		resize(changeframe, dst, Size(92, 112), 0, 0, INTER_LINEAR);
		char c = waitKey(100);
		if (c == 97) {										// 'a'开始拍照
			++i;
			string filename = format("%d.jpg", i);			// 定义图片存储的位置
			imwrite(filename, dst);							// 保存灰度图
		}
		else if (c == 27) {									// Esc推出程序
			break;
		}
	}

	return 0;
}
