#include <core/Logger.hpp>
#include <Sandbox.hpp>

int main(int argc, char** argv)
{
#ifndef NDEBUG
    core::Logger::setLogLevel(core::Logger::LogLevel::Debug);
#else
    core::Logger::setLogLevel(core::Logger::LogLevel::Info);
#endif
    core::Logger::logInfo("Program started!");

    SandboxApp app;

    app.run();

    return 0;
}
