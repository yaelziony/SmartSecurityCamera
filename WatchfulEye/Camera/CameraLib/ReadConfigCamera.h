#pragma once
#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>
#include "Logging.h"
#include "json.hpp"
#ifdef _WIN32
#define FILE_CONFIG_PATH R"(C:\ourProject\MobileyeBootcamp_RBNY\WatchfulEye\Camera\CameraLib\config.json)"
#else
#define FILE_CONFIG_PATH R"(../../CameraLib/config.json)"
#endif
using json = nlohmann::json;

typedef struct cameraMembers {
	float cameraThreshold;
	std::string cameraIP;
	std::string cameraPort;
	int cameraDiffPixels;
	std::string videoPath;
	LoggerType type;
	std::string loggerName;
	spdlog::level::level_enum level;
	std::string fileName;
	size_t maxFileSize;
	size_t maxFiles;
} cameraMembers;

class ConfigurationCamera {
public:
	//reads configuration data from a JSON file and sets variables 
	cameraMembers getCameraMembers();
	int getQueueMembers();
	cameraMembers setLoggingMembers(cameraMembers cameraMember);
	json readConfigFile();
};