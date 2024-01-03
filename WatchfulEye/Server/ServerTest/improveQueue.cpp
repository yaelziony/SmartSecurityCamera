#include <catch2/catch_test_macros.hpp>
#include "opencv2/opencv.hpp"
#include "Queue.h"

class mocked_g_mutex {
public:
	void lock() {}
	void unlock() {}
};

#ifdef UNIT_TESTING
#define g_mutex mocked_g_mutex
#endif

TEST_CASE("improveQueue") {
	VideoFrameQueue& frameQueue = VideoFrameQueue::getInstance();
	frameQueue.setStopServer(false);
	VideoFrameData currentFrame;
	uint8_t buff[] = { 1,1,3,4,5,6,7,8,9 };
	cv::Mat frames(3, 3, CV_8UC1, buff);
	int frameCounter = 1;
	for (size_t i = 0; i < 10; i++)
	{
		currentFrame.timestamp = i;
		currentFrame.frame = frames;
		frameQueue.push(currentFrame);
		REQUIRE(frameQueue.size() == 5);
	}
	// Queue size should be limited to 5 frames
	REQUIRE(frameQueue.size() == 5);
}
