#include "Camera.h"

using namespace std;
using namespace cv;

Camera* Camera::instancePtr = NULL;

Camera& Camera::getInstance()
{
	if (instancePtr == NULL)
		instancePtr = new Camera();
	return *instancePtr;
}

Camera::Camera() {
	init();
}
void Camera::init() {
	//Initialize the variables are used to get the configurations
	ConfigurationCamera configurationCamera;
	cameraMember = configurationCamera.getCameraMembers();
	initializeChannelAndStub();
}
void Camera::runCamera() {
	stopCamera = 0;
#ifdef __linux__
	initscr();  // Initialize the ncurses library
	keypad(stdscr, TRUE);  // Enable reading of function keys like ESC
	nodelay(stdscr, TRUE);  // Enable non-blocking input
#endif

	VideoCapture cap(this->cameraMember.videoPath);
	//read the video
	try {
		if (!cap.isOpened()) {
			throw std::runtime_error("Error opening video stream");
		}
	}
	catch (const std::exception& e) {
		Logging::writeTolog(spdlog::level::err, e.what());
	}
	//Variables to use to ensure that only 3fps will be sent to the server
	long frameCounter = 1;
	double capFrameRate = cap.get(CAP_PROP_FPS);
	if (capFrameRate == 0.0)
		capFrameRate = 30.0;
	const int frameSkip = static_cast<int>(capFrameRate / 3);

	Mat prevFrame;
	cap >> currentFrame;
	if (!currentFrame.empty()) {
		string timestamp = calcTime();
		sendToServer(currentFrame, timestamp);
		frameCounter++;
	}

	prevFrame = currentFrame.clone();

	while (!currentFrame.empty() && (stopCamera != 27/*ESC*/)) {
		cap >> currentFrame;
		if (currentFrame.empty())
			break;

		if (frameCounter % frameSkip == 0) {
			if (cmpFrames(prevFrame, currentFrame)) {
				string timestamp = calcTime();
				sendToServer(currentFrame, timestamp);
			}
		}
		frameCounter++;

		frameCounter = (frameCounter == capFrameRate) ? 0 : frameCounter;
		prevFrame = currentFrame.clone();
#ifdef __linux__
		sleep(1);
		stopCamera = getch();  // Read a character from the keyboard
#else
		Sleep(1);
		if (_kbhit())
			stopCamera = _getch();
#endif
		if (stopCamera == 27) {
			string infoChar = "The camera exits";
			Logging::writeTolog(spdlog::level::info, infoChar);
			break;
		}
	}
#ifdef __linux__
	endwin();  // Clean up the ncurses library
#endif
	cap.release();
	destroyAllWindows();
}

bool Camera::cmpFrames(const Mat &prevFrame, const Mat &currentFrame) {
	//Calculate absolute difference between the frames.
	Mat diffFrames;
	absdiff(prevFrame, currentFrame, diffFrames);
	if (diffFrames.channels() > 1)
		cvtColor(diffFrames, diffFrames, COLOR_BGR2GRAY);
	//Checks if the frames are different enough
	double amountDiffPixels = static_cast<double>(countNonZero(diffFrames > 0));
	double amountPixelsNeeded = currentFrame.size().area() * cameraMember.cameraThreshold;
	return amountDiffPixels >= amountPixelsNeeded;
}

string Camera::calcTime() {
	// Get the current system time
	std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
	std::time_t time = std::chrono::system_clock::to_time_t(now);
	std::tm timeinfo;
#ifdef _WIN32
	localtime_s(&timeinfo, &time);
#else
	localtime_r(&time, &timeinfo);
#endif
	char buffer[20];
	strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);
	std::string formattedTime(buffer);
	return formattedTime;
}

void Camera::initializeChannelAndStub() {
	auto channel = grpc::CreateChannel(cameraMember.cameraIP + ":" + cameraMember.cameraPort, grpc::InsecureChannelCredentials());
	stub = framevideoPackage::FrameVideoService::NewStub(channel);
}

void Camera::sendToServer(const Mat& frame, const string& timestamp) {
	// Convert the image to bytes
	std::vector<uchar> imageData;
	cv::imencode(".png", frame, imageData);
	// Setup request
	framevideoPackage::frame request;
	framevideoPackage::frameResponse result;
	request.set_width(frame.cols);
	request.set_height(frame.rows);
	request.set_channels(frame.channels());
	request.set_imagedata(imageData.data(), imageData.size());
	request.set_timestamp(timestamp);

	//Sending to the server
	grpc::ClientContext context;
	grpc::Status status = stub->getFrame(&context, request, &result);

	if (status.ok())
	{
		std::string info = "-----Sending to the server was successful-----";
		Logging::writeTolog(spdlog::level::info, info);
	}
	else {
		std::string failedInfo = "----- The connection to the server failed -----";
		Logging::writeTolog(spdlog::level::err, failedInfo);
	}
	return;
}
