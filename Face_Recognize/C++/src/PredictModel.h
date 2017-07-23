#ifndef SRC_PREDICTMODEL_H__
#define SRC_PREDICTMODEL_H__

#include "UserCenterModel.h"
#include <exception>
#include <array>
#include <iostream>
#include <memory>
#include <vector>
#include <boost/noncopyable.hpp>
#include <opencv2/face/facerec.hpp>
#include <opencv2/opencv.hpp>
using namespace cv::face;

class PredictModel : boost::noncopyable
{
public:
	PredictModel(
		const std::string& src = "",
		const std::string& mapfile = "information.txt",
		const std::string& xml = "MyFaceEigen.xml")
		: m_sFilename(src), m_pfModel(createEigenFaceRecognizer())
	{
		if (m_mMap.empty()) {
			initMap(mapfile);
		}

		m_pfModel->load(xml);

	}

	void initMap(const std::string& information, const std::string& pattern_ = ";")
	{
		// Open csv file and put the data into the relevent container.
		//
		FILE* fp = fopen(information.c_str(), "r");
		char szTest[100] = { 0 };
		while (fgets(szTest, sizeof(szTest), fp)) {
			char* label = strtok(szTest, pattern_.c_str());
			char* name = strtok(NULL, pattern_.c_str());
			if (sizeof(label) != 0 && sizeof(name) != 00) {
				m_mMap.insert(
					std::make_pair(
						atoi(label),
						name
					)
				);
			}
		}
	}

	void loadXml(const std::string& xml_)
	{
		m_pfModel->load(xml_);
	}

	void loadImage(const std::string& filename_)
	{
		m_sFilename = filename_;
	}

	void prediction()
	{
		if (m_sFilename.empty()) {
			std::cout << "No images loaded" << std::endl;
		}
		else {
			cv::Mat img = cv::imread(m_sFilename, CV_LOAD_IMAGE_GRAYSCALE);
			int predicted = m_pfModel->predict(img);
			try
			{
				m_upUser = std::make_unique<UserCenterModel>();
				m_upUser->setUsername(m_mMap[predicted]);
				m_upUser->queryUser();
				m_upUser->showUser();
			}
			catch (const std::exception& e)
			{
				std::cerr << e.what() << std::endl;
			}

		}
	}

	void prediction(cv::Mat& img_)
	{
		int predicted = m_pfModel->predict(img_);

		try
		{
			m_upUser = std::make_unique<UserCenterModel>();
			m_upUser->setUsername(m_mMap[predicted]);
			m_upUser->queryUser();
			m_upUser->showUser();
		}
		catch (const std::exception& e)
		{
			std::cerr << e.what() << std::endl;
		}

	}

private:
	std::string m_sFilename;
	cv::Ptr<FaceRecognizer> m_pfModel;
	std::map<int, std::string> m_mMap;
	std::unique_ptr<UserCenterModel> m_upUser;
};

#endif // !SRC_PREDICTMODEL_H__
