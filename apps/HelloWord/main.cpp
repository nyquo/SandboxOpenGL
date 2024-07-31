#include <gl.h>

#include <Logger.hpp>

bool continueRunning{true};

void windowCloseCallback(GLFWwindow* window) {
    continueRunning = false;
}

int main(int argc, char** argv) {
	core::Logger::setLogLevel(core::Logger::LogLevel::Info);

	core::Logger::logInfo("Program started!");

    if (!glfwInit())
    {
        core::Logger::logError("Failed to init GLFW!");
        throw(std::runtime_error("Failed to init GLFW!"));
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    auto window = glfwCreateWindow(800, 600, "Hello word!", nullptr, nullptr);
    glfwMakeContextCurrent(window);


    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        core::Logger::logError("Failed to initialize GLAD");
        throw(std::runtime_error("Failed to initialize GLAD"));
    }

    glfwSetWindowCloseCallback(window, windowCloseCallback);

    while(continueRunning) {
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    
}