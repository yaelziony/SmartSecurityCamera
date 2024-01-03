#pragma once
#include "opencv2/opencv.hpp"
#include <fstream>
#include "Sqlite.h"
#include "Logging.h"

class Yolo
{
private:
	yoloMembers yoloMember;
	Sqlite sqlite;
	void init();
public:
	const float INPUT_WIDTH = 640.0;
	const float INPUT_HEIGHT = 640.0;
	const float SCORE_THRESHOLD = 0.5;
	const float NMS_THRESHOLD = 0.45;
	const float CONFIDENCE_THRESHOLD = 0.45;

	// Text parameters.
	const float FONT_SCALE = 0.7;
	const int FONT_FACE = cv::HersheyFonts::FONT_HERSHEY_SIMPLEX;
	const int THICKNESS = 1;

	// Colors.
	cv::Scalar BLACK = cv::Scalar(0, 0, 0);
	cv::Scalar BLUE = cv::Scalar(255, 178, 50);
	cv::Scalar YELLOW = cv::Scalar(0, 255, 255);
	cv::Scalar RED = cv::Scalar(0, 0, 255);

	std::vector<std::string>class_list;

	//functions
	Yolo();
	std::vector<cv::Mat> pre_process(cv::Mat& input_image, cv::dnn::Net& net);

	cv::Mat post_process(cv::Mat& input_image,
		std::vector<cv::Mat>& outputs,
		const std::vector<std::string>& class_list,
		std::string timestamp);

	void detect_objects(cv::Mat& input_image,
		std::vector<cv::Mat>& outputs,
		const std::vector<std::string>& class_list,
		std::vector<int>& class_ids,
		std::vector<float>& confidences,
		std::vector<cv::Rect>& boxes);

	void performNMS(const std::vector<cv::Rect>& boxes,
		const std::vector<float>& confidences,
		std::vector<int>& indices);

	void process_detected_results(cv::Mat& input_image,
		const std::vector<std::string>& class_list,
		const std::vector<int>& class_ids,
		const std::vector<float>& confidences,
		const std::vector<cv::Rect>& boxes,
		std::string timestamp, std::vector<int> indices);

	void draw_label(cv::Mat& input_image, std::string label, int left, int top);
};