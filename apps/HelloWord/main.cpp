#include <Logger.hpp>

#include "HelloWordApp.hpp"

int main(int argc, char** argv) {
	core::Logger::setLogLevel(core::Logger::LogLevel::Info);
	core::Logger::logInfo("Program started!");

    HelloWordApp app;

    app.run();

    return 0;
}