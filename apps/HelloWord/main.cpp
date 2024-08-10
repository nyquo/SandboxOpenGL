#include <Application.hpp>
#include <Logger.hpp>

bool continueRunning{true};

void windowCloseCallback(GLFWwindow* window) {
    continueRunning = false;
}

int main(int argc, char** argv) {
	core::Logger::setLogLevel(core::Logger::LogLevel::Info);
	core::Logger::logInfo("Program started!");

    core::Application app;

    app.run();

    return 0;
}