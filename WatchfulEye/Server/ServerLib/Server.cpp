#include "Server.h"

using namespace std;
using namespace cv;

Server* Server::instancePtr = NULL;

Server::Server() {
	init();
}
void Server::init() {
	//Initialize the static variables.are used to get the configurations
	ConfigurationServer configurationServer;
	serverMember = configurationServer.getServerMembers();
}

Server& Server::getInstance()
{
	if (instancePtr == NULL)
		instancePtr = new Server();
	return *instancePtr;
}

void Server::runServer()
{
	std::thread pushingThread(&Server::connectingTheServer, this);
	std::thread popingThread(&Server::popFromDataStruct, this);
	pushingThread.join();
	popingThread.join();
}

void Server::popFromDataStruct()
{
	Sqlite sqlite;
	sqlite.createTable(sqlite.path);
	while (!frameQueue.getStopServer()) {
		//pop frame from the queue
		VideoFrameData data;
		try { data = frameQueue.pop(); }
		catch (const std::runtime_error& e) { break; }

		Mat frame = data.frame;
		string timestamp = data.timestamp;
		detections(frame, timestamp);
	}
}

void Server::detections(Mat& frame, string timestamp)
{
	Yolo yolo;
	cv::dnn::Net net;
	try {
		net = cv::dnn::readNet(serverMember.yoloPath);
		if (net.empty()) {
			throw std::runtime_error("Error:Yolo- readNet failed.");
		}
	}
	catch (const std::exception& e) {
		Logging::writeTolog(spdlog::level::err, e.what());
	}
	vector<Mat> detections;
	detections = yolo.pre_process(frame, net);
	Mat img = yolo.post_process(frame, detections, yolo.class_list, timestamp);
	vector<double> layersTimes;
	double freq = getTickFrequency() / 1000;
	double time = net.getPerfProfile(layersTimes) / freq;
	string label = format("Inference time : %.2f ms", time);
	putText(img, label, Point(20, 40), yolo.FONT_FACE, yolo.FONT_SCALE, yolo.RED);
	detectedFrame = img;
	string infoDetection = "The dequeued frame was detected and saved in the DB, inference time " + to_string(time);
	Logging::writeTolog(spdlog::level::info, infoDetection);
}

void Server::connectingTheServer()
{
	string server_address(serverMember.backendIP + ":" + serverMember.backendPort);
	grpc::ServerBuilder builder;
	builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
	classFrameService my_service;
	builder.RegisterService(&my_service);
	unique_ptr<grpc::Server> server(builder.BuildAndStart());
	exitByESC();
}

void Server::exitByESC()
{
#ifdef __linux__
	initscr();  // Initialize the ncurses library
	keypad(stdscr, TRUE);  // Enable reading of function keys like ESC
	nodelay(stdscr, TRUE);  // Enable non-blocking input
#endif
	ch = 0;
	while (ch != 27 /*ESC*/) {
#ifdef __linux__
		ch = getch();  // Read a character from the keyboard
#else
		if (_kbhit()) {
			ch = _getch();
		}
#endif
		if (ch == 27) {
			string infoChar = "The server thread exits.";
			Logging::writeTolog(spdlog::level::info, infoChar);
			frameQueue.setStopServer(true);
			frameQueue.notifyThread();
			break;
		}
	}
#ifdef __linux__
	endwin();  // Clean up the ncurses library
#endif
}