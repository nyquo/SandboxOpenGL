#include "HelloWordApp.hpp"

#include <Logger.hpp>

int main(int argc, char** argv)
{
#ifndef NDEBUG
    core::Logger::setLogLevel(core::Logger::LogLevel::Debug);
#else
    core::Logger::setLogLevel(core::Logger::LogLevel::Info);
#endif
    core::Logger::logInfo("Program started!");

    HelloWordApp app;

    app.run();

    return 0;
}
