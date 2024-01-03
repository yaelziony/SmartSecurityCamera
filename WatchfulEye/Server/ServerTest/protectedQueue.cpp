#include <catch2/catch_test_macros.hpp>
#include "opencv2/opencv.hpp"
#include "Queue.h"
#include <thread>

using namespace cv;
using namespace std;


TEST_CASE("VideoFrameQueue concurrent push test") {
	const int numThreads = 4;
	VideoFrameQueue& frameQueue = VideoFrameQueue::getInstance();

	auto pushThread = [&frameQueue]() {
		VideoFrameData frameData;
		uint8_t buff[] = { 1,1,3,4,5,6,7,8,9 };
		Mat frames(3, 3, CV_8UC1, buff);

		string time = "14:03";

		frameData.timestamp = time;
		frameData.frame = frames;
		// Push the frameData to the queue using the push function
		frameQueue.push(frameData);
	};

	// Create and start the threads
	std::vector<std::thread> threads;
	for (int i = 0; i < numThreads; i++) {
		threads.emplace_back(pushThread);
	}

	// Wait for all threads to finish
	for (auto& thread : threads) {
		thread.join();
	}

	REQUIRE(frameQueue.size() == numThreads); // Check if the container has the expected number of elements
	}
