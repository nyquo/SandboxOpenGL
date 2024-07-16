#include <iostream>

#include <Logger.hpp>

int main(int argc, char** argv) {
	core::Logger::setLogLevel(core::Logger::LogLevel::Info);

	core::Logger::logInfo("Program started!");
}