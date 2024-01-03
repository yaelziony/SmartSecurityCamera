#ifndef READCONFIGSERVER_H
#define READCONFIGSERVER_H
#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>
#include "Logging.h"
#include "json.hpp"
#ifdef _WIN32
#define FILE_CONFIG_PATH R"(C:\ourProject\MobileyeBootcamp_RBNY\WatchfulEye\Server\ServerLib\config.json)"
#else
#define FILE_CONFIG_PATH  R"(../../ServerLib/config.json)"
#endif

using json = nlohmann::json;

typedef struct serverMembers {
	std::string backendPort;
	std::string backendIP;
	LoggerType type;
	std::string loggerName;
	spdlog::level::level_enum level;
	std::string fileName;
	size_t maxFileSize;
	size_t maxFiles;
	std::string yoloPath;
}serverMembers;

typedef struct yoloMembers {
	std::string detectionPath;
	std::string itemToDetect;
	int rows;
	int cx;
	int cy;
	int w;
	int h;
	float numberForAligns;
	int jumpToTheNextRow;
} yoloMembers;

class ConfigurationServer {
public:
	json readConfigFile();
	//reads configuration data from a JSON file and sets variables 
	serverMembers getServerMembers();
	//Initialize the logging configuration based on another JSON file-if it doesn't exist.
	//if it exist, reads configuration data from logging JSON
	serverMembers setLoggingMembers(serverMembers serverMember);
	int getQueueMembers();
	std::string getSqliteMembers();
	yoloMembers getYoloMembers();
};
#endif