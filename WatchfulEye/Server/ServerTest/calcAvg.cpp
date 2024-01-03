#include <catch2/catch_test_macros.hpp>
#include "opencv2/opencv.hpp"
#include "Sqlite.h"

TEST_CASE("check_calcAvg_function") {
	cv::Mat im(3, 3, CV_8UC3);
	im.at<cv::Vec3b>(0, 0) = cv::Vec3b(10, 123, 4);
	im.at<cv::Vec3b>(0, 1) = cv::Vec3b(10, 123, 4);
	im.at<cv::Vec3b>(0, 2) = cv::Vec3b(10, 123, 4);
	im.at<cv::Vec3b>(1, 0) = cv::Vec3b(10, 123, 4);
	im.at<cv::Vec3b>(1, 1) = cv::Vec3b(10, 123, 4);
	im.at<cv::Vec3b>(1, 2) = cv::Vec3b(10, 123, 4);
	im.at<cv::Vec3b>(2, 0) = cv::Vec3b(10, 123, 4);
	im.at<cv::Vec3b>(2, 1) = cv::Vec3b(10, 123, 4);
	im.at<cv::Vec3b>(2, 2) = cv::Vec3b(10, 123, 4);
	Sqlite sqlite;
	Avgs avg = sqlite.calcAvgs(im, cv::Rect(0, 0, 3, 3));
	REQUIRE(avg.AvgB == 10);
	REQUIRE(avg.AvgG == 123);
	REQUIRE(avg.AvgR == 4);
}
