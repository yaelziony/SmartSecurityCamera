#include "Yolo.h"

using namespace std;
using namespace cv;

Yolo::Yolo() {
	init();
	try {
		ifstream ifs(yoloMember.detectionPath);
		if (!ifs.is_open()) {
			throw std::runtime_error("Error opening classes file");
		}
		string line;
		while (getline(ifs, line))
		{
			class_list.push_back(line);
		}
	}
	catch (const std::exception& e) {
		Logging::writeTolog(spdlog::level::err, e.what());
	}
}
void Yolo::init() {
	//Initialize the static variables.are used to get the configurations
	ConfigurationServer configurationServer;
	yoloMember = configurationServer.getYoloMembers();
}

// Pre-processes theinput image and performs forward pass in the YOLO network.
// Returns the network outputs (detections) as a vector of Mats.
vector<Mat> Yolo::pre_process(Mat& input_image, cv::dnn::Net& net)
{
	Mat blob;
	cv::dnn::blobFromImage(input_image, blob, 1. / 255., Size(INPUT_WIDTH, INPUT_HEIGHT), Scalar(), true, false);
	net.setInput(blob);
	vector<Mat> outputs;
	net.forward(outputs, net.getUnconnectedOutLayersNames());
	return outputs;
}

// Post-processes the YOLO network outputs, performs Non-Maximum Suppression (NMS),
// and calls the function to process the detected results (draw bounding boxes, labels, and save data).
Mat Yolo::post_process(Mat& input_image, vector<Mat>& outputs, const vector<string>& class_list, string timestamp) {
	vector<int> class_ids;
	vector<float> confidences;
	vector<Rect> boxes;

	detect_objects(input_image, outputs, class_list, class_ids, confidences, boxes);

	vector<int> indices;
	performNMS(boxes, confidences, indices);
	process_detected_results(input_image, class_list, class_ids, confidences, boxes, timestamp, indices);
	return input_image;
}

// Detects objects in the input image by analyzing the network outputs,
// and populates the class_ids, confidences, and boxes vectors with the detected information.
void Yolo::detect_objects(Mat& input_image, vector<Mat>& outputs, const vector<string>& class_list, vector<int>& class_ids, vector<float>& confidences, vector<Rect>& boxes)
{
	float x_factor = input_image.cols / INPUT_WIDTH;
	float y_factor = input_image.rows / INPUT_HEIGHT;
	float* data = (float*)outputs[0].data;
	for (int i = 0; i < yoloMember.rows; ++i)
	{
		float confidence = data[4];
		if (confidence >= CONFIDENCE_THRESHOLD)
		{
			float* classes_scores = data + 5;
			Mat scores(1, class_list.size(), CV_32FC1, classes_scores);
			Point class_id;
			double max_class_score;
			minMaxLoc(scores, 0, &max_class_score, 0, &class_id);
			if (max_class_score > SCORE_THRESHOLD)
			{
				confidences.push_back(confidence);
				class_ids.push_back(class_id.x);
				float cx = data[yoloMember.cx];
				float cy = data[yoloMember.cy];
				float w = data[yoloMember.w];
				float h = data[yoloMember.h];
				int left = int((cx - yoloMember.numberForAligns * w) * x_factor);
				int top = int((cy - yoloMember.numberForAligns * h) * y_factor);
				int width = int(w * x_factor);
				int height = int(h * y_factor);

				boxes.push_back(Rect(left, top, width, height));
			}
		}
		data += yoloMember.jumpToTheNextRow;
	}
}

// Performs Non-Maximum Suppression (NMS) on the detected bounding boxes and
// returns the selected indices of the non-overlapping detections.
void Yolo::performNMS(const vector<Rect>& boxes, const vector<float>& confidences, vector<int>& indices)
{
	cv::dnn::NMSBoxes(boxes, confidences, SCORE_THRESHOLD, NMS_THRESHOLD, indices);
}

// Processes the detected results by drawing bounding boxes and labels on the input image,
// and saves the detection data to a CSV file.
void Yolo::process_detected_results(Mat& input_image,
	const vector<string>& class_list,
	const vector<int>& class_ids,
	const vector<float>& confidences,
	const vector<Rect>& boxes,
	string timestamp,
	vector<int> indices)
{
	for (int i = 0; i < indices.size(); i++)
	{
		int idx = indices[i];
		Rect box = boxes[idx];
		int left = box.x;
		int top = box.y;
		int width = box.width;
		int height = box.height;

		rectangle(input_image, Point(left, top), Point(left + width, top + height), BLUE, 3 * THICKNESS);
		if (class_list[class_ids[idx]] == yoloMember.itemToDetect) {
			string label = format("%.2f", confidences[idx]);
			label = class_list[class_ids[idx]] + ":" + label;
			draw_label(input_image, label, left, top);
			sqlite.insertData(sqlite.path, input_image, box, timestamp);
			Logging::writeTolog(spdlog::level::info, "The frame saved in the data base");
		}
	}
}

void Yolo::draw_label(Mat& input_image, string label, int left, int top)
{
	int baseLine;
	Size label_size = getTextSize(label, FONT_FACE, FONT_SCALE, THICKNESS, &baseLine);
	top = max(top, label_size.height);
	Point tlc = Point(left, top);
	Point brc = Point(left + label_size.width, top + label_size.height + baseLine);
	rectangle(input_image, tlc, brc, BLACK, FILLED);
	putText(input_image, label, Point(left, top + label_size.height), FONT_FACE, FONT_SCALE, YELLOW, THICKNESS);
}