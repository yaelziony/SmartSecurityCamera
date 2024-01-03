#include <catch2/catch_test_macros.hpp>
#include "opencv2/opencv.hpp"
#include "Camera.h"

using namespace cv;

TEST_CASE("check_cmpFrames_function") {
	uint8_t buff[] = { 1,2,3,4,5,6,7,8,9 };
	uint8_t buff1[] = { 11,12,14,15,16,17,18,19,20 };
	uint8_t buff2[] = { 25,26,27,28,29,23,29,35,37 };
	uint8_t buff3[] = { 1,1,3,4,5,6,7,8,9 };
	Mat frames(3, 3, CV_8UC1, buff);
	Mat frames1(3, 3, CV_8UC1, buff1);
	Mat frames2(3, 3, CV_8UC1, buff2);
	Mat frames3(3, 3, CV_8UC1, buff3);
	Camera& camera = Camera::getInstance();
	bool a = camera.cmpFrames(frames, frames1);
	bool b = camera.cmpFrames(frames2, frames3);

	REQUIRE(true == a);
	REQUIRE(true == b);
}
