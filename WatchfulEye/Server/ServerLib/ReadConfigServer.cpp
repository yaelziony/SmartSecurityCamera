#include "ReadConfigServer.h"

json ConfigurationServer::readConfigFile()
{
	try {
		std::ifstream configFile(FILE_CONFIG_PATH);
		if (configFile.is_open()) {
			json config;
			configFile >> config;
			configFile.close();
			return config;
		}
		else {
			throw std::runtime_error("Unable to open config file.");
		}
	}
	catch (const std::exception& e) {
		std::ifstream configFile(FILE_CONFIG_LOGGER_PATH);
		if (configFile.is_open())
			Logging::writeTolog(spdlog::level::err, e.what());
		else
			std::cout << e.what();
	}
}

serverMembers ConfigurationServer::getServerMembers()
{
	json config = readConfigFile();
	serverMembers serverMember;
	serverMember.backendIP = config["backend_ip"];
	serverMember.backendPort = config["backend_port"];
#ifdef _WIN32
	serverMember.yoloPath = config["windows_yolo_path"];
#else
	serverMember.yoloPath = config["linux_yolo_path"];
#endif
	return setLoggingMembers(serverMember);
}

serverMembers ConfigurationServer::setLoggingMembers(serverMembers serverMember) {
	std::ifstream configFile(FILE_CONFIG_LOGGER_PATH);
	json loggingConfig;
	if (configFile.is_open()) {
		configFile >> loggingConfig;
		configFile.close();
		serverMember.type = loggingConfig["type"];
		serverMember.loggerName = loggingConfig["loggerName"];
		serverMember.level = loggingConfig["level"];
		serverMember.fileName = loggingConfig["fileName"];
		serverMember.maxFileSize = loggingConfig["maxFileSize"];
		serverMember.maxFiles = loggingConfig["maxFiles"];

		Logging::createLogger(serverMember.type, serverMember.loggerName, serverMember.level,
			serverMember.fileName, serverMember.maxFileSize, serverMember.maxFiles);
	}
	else {
		Logging::initLogger();
	}
	return serverMember;
}

int ConfigurationServer::getQueueMembers()
{
	json config = readConfigFile();
	return config["backend_queue_size"];
}

std::string ConfigurationServer::getSqliteMembers()
{
	json config = readConfigFile();
#ifdef _WIN32
	return config["windows_db_path"];
#else
	return config["linux_db_path"].get<std::string>();
#endif
}

yoloMembers ConfigurationServer::getYoloMembers()
{
	json config = readConfigFile();
	yoloMembers yoloMember;
	yoloMember.itemToDetect = config["item_to_detect"];
#ifdef _WIN32
	yoloMember.detectionPath = config["windows_detections_path"];
#else
	yoloMember.detectionPath = config["linux_detections_path"];
#endif
	yoloMember.rows = config["rows"];
	yoloMember.cx = config["cx"];
	yoloMember.cy = config["cy"];
	yoloMember.w = config["w"];
	yoloMember.h = config["h"];
	yoloMember.numberForAligns = config["number_for_aligns"];
	yoloMember.jumpToTheNextRow = config["jump_to_the_next_row"];
	return yoloMember;
}
