#pragma once
#include <queue>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include "opencv2/opencv.hpp"
#include "ReadConfigServer.h"

typedef struct videoFrameData {
	std::string timestamp;
	cv::Mat frame;
} VideoFrameData;

class VideoFrameQueue {
private:
	int backendQueueSize;
	std::atomic<bool> stopServer;
	std::mutex g_mutex;
	std::condition_variable g_cv;
	bool ready = false;
	std::queue<VideoFrameData> container;
	static VideoFrameQueue* instancePtr;
	VideoFrameQueue();
	void init();
public:
	VideoFrameQueue(const VideoFrameQueue& obj) = delete;
	static VideoFrameQueue& getInstance();
	VideoFrameData& front();
	void push(const VideoFrameData& data);
	VideoFrameData pop();
	bool empty();
	size_t size();
	bool getReady() { return ready; }
	void setStopServer(bool value) { stopServer = value; }
	bool getStopServer() { return stopServer; }
	void notifyThread() { g_cv.notify_one(); }
};