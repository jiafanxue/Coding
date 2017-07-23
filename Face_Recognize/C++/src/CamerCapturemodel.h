#ifndef CAMERACAPTUREMODEL_H__
#define CAMERACAPTUREMODEL_H__

#include "PredictModel.h"
#include <opencv2/opencv.hpp>

class CameraCaptureModel
{
public:
	// Use the default constructor (PredictModel)
	CameraCaptureModel()
		: m_vCapture(0)
	{
		// empty
	}

	CameraCaptureModel(const std::string src_)
		: m_vCapture(0),
		  m_Predict(src_)
	{
		// empty
	}

	void openCamera()
	{
		if (!m_vCapture.isOpened()) {
			std::cerr << "VideoCapture construct error!" << std::endl;
			exit(1);
		}

		while (1) {
			m_vCapture >> m_mFrame;
			if (m_mFrame.empty()) {
				std::cerr << "New frame is empty" << std::endl;
				exit(1);
			}

			imshow("MyPicture", m_mFrame);

			cv::cvtColor(m_mFrame, m_mGray, CV_BGR2GRAY);

			cv::resize(m_mGray, m_mEdge, cv::Size(92, 112), 0, 0, CV_INTER_LINEAR);

			m_Predict.prediction(m_mEdge);

			char c = cv::waitKey(100);
			if (c == 27) {									// Esc推出程序
				break;
			}
		}
	}

private:
	cv::VideoCapture m_vCapture;
	cv::Mat m_mFrame;
	cv::Mat m_mEdge;
	cv::Mat m_mGray;
	PredictModel m_Predict;
};

#endif // !CAMERACAPTUREMODEL_H__
