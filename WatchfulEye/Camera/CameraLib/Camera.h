#ifndef CAMERA_H
#define CAMERA_H

#include <chrono>
#include "opencv2/opencv.hpp"
#include <grpc/grpc.h>
#include <grpcpp/create_channel.h>
#include "myproto/frameVideo.grpc.pb.h"
#include "ReadConfigCamera.h"
#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#else
#include <unistd.h>
#include <ncurses.h>
#endif

class Camera {
private:
	cameraMembers cameraMember;
	std::unique_ptr<framevideoPackage::FrameVideoService::Stub> stub;
	cv::Mat currentFrame;
	int stopCamera;
	static Camera* instancePtr;
	Camera();
	void init();
public:
	Camera(const Camera& obj) = delete;
	static Camera& getInstance();
	void runCamera();
	bool cmpFrames(const cv::Mat &prevFrame, const cv::Mat &currentFrame);
	std::string calcTime();
	void initializeChannelAndStub();
	void sendToServer(const cv::Mat& frame, const std::string& timestamp);
	cv::Mat& getCurrentFrame() { return currentFrame; }
	void setCurrentFrame(const cv::Mat &frame) { currentFrame = frame; }
	int getStopCamera() { return stopCamera; }
	void setStopCamera(int c) { stopCamera = c; }
};
#endif // CAMERA_H
