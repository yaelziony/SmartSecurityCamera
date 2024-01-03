#include "Queue.h"

VideoFrameQueue* VideoFrameQueue::instancePtr = NULL;

VideoFrameQueue::VideoFrameQueue() {
	init();
}
void VideoFrameQueue::init() {
	stopServer = false;
	//Initialize the static variables.are used to get the configurations
	ConfigurationServer configurationServer;
	backendQueueSize = configurationServer.getQueueMembers();
}
VideoFrameQueue& VideoFrameQueue::getInstance()
{
	if (instancePtr == NULL)
		instancePtr = new VideoFrameQueue();
	return *instancePtr;
}

VideoFrameData& VideoFrameQueue::front() {
	std::unique_lock<std::mutex> lock(g_mutex);
	g_cv.wait(lock, [this] { return ready || stopServer; });
	//critical section
	if (stopServer)//If it enters because ESC is pressed
		throw std::runtime_error("ESC key was pressed");
	return container.front();
}

void VideoFrameQueue::push(const VideoFrameData& data) {
	std::lock_guard<std::mutex> lock(g_mutex);
	//critical section
	if (container.size() == backendQueueSize)
	{
		container.pop();
		std::string info = "Pop from queueFrames, because the queue is limited to 5";
		Logging::writeTolog(spdlog::level::info, info);
	}
	container.push(data);
	std::string info1 = "Pushed to the frame queue, a frame that was at the time: " + data.timestamp;
	Logging::writeTolog(spdlog::level::info, info1);
	ready = true;
	g_cv.notify_one();
}

VideoFrameData VideoFrameQueue::pop() {
	std::unique_lock<std::mutex> lock(g_mutex);
	g_cv.wait(lock, [this] { return ready || stopServer; });
	//critical section
	if (stopServer)//If it enters because ESC is pressed
		throw std::runtime_error("ESC key was pressed");
	VideoFrameData data = container.front();
	container.pop();
	std::string info = "Pop from queueFrames, a frame that was at the time: " + data.timestamp;
	Logging::writeTolog(spdlog::level::info, info);
	if (container.empty())
		ready = false;
	return data;
}

bool VideoFrameQueue::empty() {
	std::lock_guard<std::mutex> lock(g_mutex);
	return container.empty();
}

size_t VideoFrameQueue::size() {
	std::lock_guard<std::mutex> lock(g_mutex);
	return container.size();
}