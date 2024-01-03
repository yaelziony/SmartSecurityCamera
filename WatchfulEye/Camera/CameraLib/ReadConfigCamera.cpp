#include "ReadConfigCamera.h"
json ConfigurationCamera::readConfigFile()
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
cameraMembers ConfigurationCamera::getCameraMembers()
{
		json config = readConfigFile();
		cameraMembers cameraMember;
		cameraMember.cameraThreshold = config["camera_threshold"];
		cameraMember.cameraIP = config["camera_ip"];
		cameraMember.cameraPort = config["camera_port"];
		cameraMember.cameraDiffPixels = config["camera_diff_pixels"];
#ifdef _WIN32
		cameraMember.videoPath = config["windows_video_path"];
#else
		cameraMember.videoPath = config["linux_video_path"];
#endif
		return setLoggingMembers(cameraMember);
}
cameraMembers ConfigurationCamera::setLoggingMembers(cameraMembers cameraMember) {
	std::ifstream configFile(FILE_CONFIG_LOGGER_PATH);
	json loggingConfig;
	if (configFile.is_open()) {
		configFile >> loggingConfig;
		configFile.close();
		cameraMember.type = loggingConfig["type"];
		cameraMember.loggerName = loggingConfig["loggerName"];
		cameraMember.level = loggingConfig["level"];
		cameraMember.fileName = loggingConfig["fileName"];
		cameraMember.maxFileSize = loggingConfig["maxFileSize"];
		cameraMember.maxFiles = loggingConfig["maxFiles"];

		Logging::createLogger(cameraMember.type, cameraMember.loggerName, cameraMember.level,
			cameraMember.fileName, cameraMember.maxFileSize, cameraMember.maxFiles);
	}
	else {
		Logging::initLogger();
	}
	return cameraMember;
}
int ConfigurationCamera::getQueueMembers()
{
	json config = readConfigFile();
	return config["backend_queue_size"];
}