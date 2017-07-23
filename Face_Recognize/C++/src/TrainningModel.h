#ifndef SRC_TRAININGMODEL_H__
#define SRC_TRAININGMODEL_H__

//#include "PredictModel.h"

#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <boost/noncopyable.hpp>
#include <opencv2/face/facerec.hpp>
#include <opencv2/opencv.hpp>
using namespace cv::face;

class TrainningModel : boost::noncopyable/*, public Predict*/
{
public:
	explicit TrainningModel(
		const std::string& filename_,
		const std::string& mapfile = "information.txt") : m_sFilename(filename_)
	{
		initMap(mapfile);
	}

	/*
	TrainningModel() = delete;
	TrainningModel(const TrainningModel&) = delete;
	TrainningModel& operator=(const TrainningModel&) = delete;
	*/

	void read_csv(const std::string& pattern_ = ";")
	{
		// Open csv file and put the data into the relevent container.
		//
		std::fstream fstrm(m_sFilename, std::ios::in | std::ios::binary | std::ios::ate);
		if (!fstrm.is_open()) {
			std::string error_message = "File read failed (One param): No file named " + m_sFilename;
			CV_Error(CV_StsInternal, error_message);
		}
		else {
			auto size = fstrm.tellg();
			std::string allStr(size, '\0');
			// Read back to the beginning
			//
			fstrm.seekg(0);
			if (fstrm.read(&allStr[0], size)) {
				std::istringstream toReadOL(allStr);
				// Read every line
				//
				for(std::array<char, 100> a; toReadOL.getline(&a[0], 100, '\n'); ) {
					// Being String split by pattern_
					std::string tmpStr = &a[0];
					size_t pos = tmpStr.find(pattern_);
					size_t size = tmpStr.size();

					// Cutting is done. Check if get to the end of file.
					if (pos != std::string::npos) {
						// holds images and labels
						m_vMImages.push_back(cv::imread(tmpStr.substr(0, pos), 0));
						m_vILabels.push_back(std::stoi(tmpStr.substr(pos + 1, size)));
					}
				}
			}

			// End read
		}

		if (m_vMImages.size() <= 1) {
			std::string error_message = "The number of images is insufficient";
			CV_Error(CV_BadImageSize, error_message);
		}
	}

	void read_csv(const std::string& filename_, const std::string& pattern_ = ";")
	{
		// Update the filename
		m_sFilename = filename_;

		// Open csv file and put the data into the relevent container.
		//
		std::fstream fstrm(m_sFilename, std::ios::in | std::ios::binary | std::ios::ate);
		if (!fstrm.is_open()) {
			std::string error_message = "File read failed (One param): No file named " + m_sFilename;
			CV_Error(CV_StsInternal, error_message);
		}
		else {
			auto size = fstrm.tellg();
			std::string allStr(size, '\0');
			// Read back to the beginning
			//
			fstrm.seekg(0);
			if (fstrm.read(&allStr[0], size)) {
				std::istringstream toReadOL(allStr);
				// Read every line
				//
				for (std::array<char, 100> a; toReadOL.getline(&a[0], 100, '\n'); ) {
					// Being String split by pattern_
					std::string tmpStr = &a[0];
					size_t pos = tmpStr.find(pattern_);
					size_t size = tmpStr.size();

					// Cutting is done. Check if get to the end of file.
					if (pos != std::string::npos) {
						// holds images and labels
						m_vMImages.push_back(cv::imread(tmpStr.substr(0, pos), 0));
						m_vILabels.push_back(std::stoi(tmpStr.substr(pos + 1, size)));
					}
				}
			}

			// End read
		}

		if (m_vMImages.size() <= 1) {
			std::string error_message = "The number of images is insufficient";
			CV_Error(CV_BadImageSize, error_message);
		}
	}

	std::pair<std::vector<cv::Mat>, std::vector<int>> read_csv_p(
		const std::string& filename_, const std::string& pattern_ = ";")
	{
		std::vector<cv::Mat> newImages;
		std::vector<int> newLabels;
		// Open csv file and put the data into the relevent container.
		//
		std::fstream fstrm(m_sFilename, std::ios::in | std::ios::binary | std::ios::ate);
		if (!fstrm.is_open()) {
			std::string error_message = "File read failed (One param): No file named " + m_sFilename;
			CV_Error(CV_StsInternal, error_message);
		}
		else {
			auto size = fstrm.tellg();
			std::string allStr(size, '\0');
			// Read back to the beginning
			//
			fstrm.seekg(0);
			if (fstrm.read(&allStr[0], size)) {
				std::istringstream toReadOL(allStr);
				// Read every line
				//
				for (std::array<char, 100> a; toReadOL.getline(&a[0], 100, '\n'); ) {
					// Being String split by pattern_
					std::string tmpStr = &a[0];
					size_t pos = tmpStr.find(pattern_);
					size_t size = tmpStr.size();

					// Cutting is done. Check if get to the end of file.
					if (pos != std::string::npos) {
						// holds images and labels
						newImages.push_back(cv::imread(tmpStr.substr(0, pos), 0));
						newLabels.push_back(std::stoi(tmpStr.substr(pos + 1, size)));
					}
				}
			}

			// End read
		}

		if (newImages.size() <= 1) {
			std::string error_message = "The number of images is insufficient";
			CV_Error(CV_BadImageSize, error_message);
		}

		return std::make_pair(newImages, newLabels);
	}

	void train()
	{
		trainEigen();
		trainFisher();
		trainLBPH();
	}

	void trainEigen(const std::string& savefilename_ = "MyFaceEigen.xml")
	{
		if (m_pfEigenModel.empty()) {
			m_pfEigenModel = createEigenFaceRecognizer();
			m_pfEigenModel->train(m_vMImages, m_vILabels);
			//m_pfEigenModel->save(savefilename_);
			save(m_pfEigenModel, savefilename_);
		}
	}

	void trainFisher(const std::string& savefilename_ = "MyFaceFisher.xml")
	{
		if (m_pfFisherModel.empty()) {
			m_pfFisherModel = createFisherFaceRecognizer();
			m_pfFisherModel->train(m_vMImages, m_vILabels);
			//m_pfFisherModel->save(savefilename_);
			save(m_pfFisherModel, savefilename_);
		}
	}

	void trainLBPH(const std::string& savefilename_ = "MyFaceLBPH.xml")
	{
		if (m_pfLBPHModel.empty()) {
			m_pfLBPHModel = createLBPHFaceRecognizer();
			m_pfLBPHModel->train(m_vMImages, m_vILabels);
			//m_pfLBPHModel->save(savefilename_);
			save(m_pfLBPHModel, savefilename_);
		}
	}

	// Only createLBPHFaceRecognizer model supports updating
	//
	//void update(const std::string& filename_, const std::string& savefilename_ = "MyFaceLBPH.xml", const std::string& pattern_ = ";")
	//{
	//	if (m_pfLBPHModel.empty()) {
	//		m_pfLBPHModel = createLBPHFaceRecognizer();
	//	}
	//	auto new_container = read_csv_p(filename_, pattern_);
	//	m_pfLBPHModel->update(new_container.first, new_container.second);
	//	//m_pfLBPHModel->save(savefilename_);
	//	save(m_pfLBPHModel, savefilename_);
	//}

	// 暂时只提供Eigen预测，等我想到如何解决不同算法参数的设置的时候在回来改这段代码
	void predict(const std::string& src)
	{
		// Read in a sample image:
		cv::Mat img = cv::imread(src, CV_LOAD_IMAGE_GRAYSCALE);

		// And get a prediction from cv::FaceRecognizer
		predicted = m_pfEigenModel->predict(img);
		/*std::cout << predicted << std::endl;*/
	}

	// If predicted is already trained.
	const std::string predictToName()
	{
		return m_mMap[predicted];
	}

	const std::string predictToName(const std::string& src)
	{
		// Read in a sample image:
		cv::Mat img = cv::imread(src, CV_LOAD_IMAGE_GRAYSCALE);

		// And get a prediction from cv::FaceRecognizer
		predicted = m_pfEigenModel->predict(img);
		return m_mMap[predicted];
	}

	void predict(const std::string& src, int& label, double& confidence)
	{
		// Read in a sample image:
		cv::Mat img = cv::imread(src, CV_LOAD_IMAGE_GRAYSCALE);

		// Get the prediction and associated confidence from the model
		m_pfEigenModel->predict(img, label, confidence);
	}

	inline void save(cv::Ptr<FaceRecognizer>& ptrFR_, const std::string& savefilename_)
	{
		ptrFR_->save(savefilename_);
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

private:
	int predicted;										// predictive value
	std::string m_sFilename;							// csv file
	std::vector<cv::Mat> m_vMImages;					// relevant container
	std::vector<int> m_vILabels;
	cv::Ptr<FaceRecognizer> m_pfEigenModel;
	cv::Ptr<FaceRecognizer> m_pfFisherModel;
	cv::Ptr<FaceRecognizer> m_pfLBPHModel;
	std::map<int, std::string> m_mMap;
};

#endif // !SRC_TRAININGMODEL_H__
