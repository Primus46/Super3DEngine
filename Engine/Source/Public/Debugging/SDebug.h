#pragma once
#include <cstdint>
#include <iostream>
#include <string>

enum SELogType : uint8_t {
	ST_LOG = 0U,
	ST_WARN,
	ST_ERROR,
	ST_SUCCESS
};

class SDebug {
public:
	// log a message to the console based on the log type
	static void Log(const std::string& msg, const SELogType& logtype = ST_LOG) {
		std::cout << msg << std::endl;
	}

};