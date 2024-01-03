#ifndef SERVER_H
#define SERVER_H
#include <thread>
#include "ServerService.h"
#include  "Yolo.h"
#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#else
#include <unistd.h>
#include <ncurses.h>
#endif

class Server {
private:
	serverMembers serverMember;
	cv::Mat detectedFrame;
	int ch;
	static Server* instancePtr;
	Server();
	void init();
public:
	Server(const Server& obj) = delete;
	static Server& getInstance();
	VideoFrameQueue& frameQueue = VideoFrameQueue::getInstance();
	void runServer();
	void popFromDataStruct();
	void detections(cv::Mat& frame, std::string timestamp);
	void connectingTheServer();
	void exitByESC();
	cv::Mat getDetectedFrame() { return detectedFrame; }
	void setDetectedFrame(cv::Mat frame) { detectedFrame = frame; }
	void setCh(int ch) { this->ch = ch; }
	int getCh() { return ch; }
};
#endif // SERVER_H