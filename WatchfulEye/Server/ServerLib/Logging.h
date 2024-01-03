#pragma once
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <string>
#include <vector>
#include <iostream>
#include <filesystem>
#include <fstream>
#include "json.hpp"
#define FILE_CONFIG_LOGGER_PATH "logging_config.json"
using json = nlohmann::json;

enum class LoggerType {
	Console,
	File,
	DailyFile,
	RotatingFile
};

class Logging
{
private:
	spdlog::level::level_enum static getLevel(spdlog::level::level_enum level);
	void static closeSpdlog();
	static void createJsonLoging(LoggerType type = LoggerType::File
		, const std::string loggerName = "logger"
		, spdlog::level::level_enum level = spdlog::level::debug,
		const std::string fileName = "fileLogger.txt"
		, size_t maxFileSize = 1024, size_t maxFiles = 3);
public:
	static std::vector<std::shared_ptr<spdlog::logger>>loggers;
	void static initLogger();
	static void  createLogger(LoggerType type = LoggerType::File,
		const std::string loggerName = "logger",
		spdlog::level::level_enum level = spdlog::level::debug,
		const std::string fileName = "fileLogger.txt",
		size_t maxFileSize = 1024,
		size_t maxFiles = 3);
	static void writeTolog(spdlog::level::level_enum loggerType, std::string messege);
};