#include "Window.hpp"

#include "Events/KeyEvent.hpp"
#include "Events/MouseEvent.hpp"
#include "Events/WindowEvent.hpp"
#include "Logger.hpp"

namespace core {

bool Window::s_glfwInitialized = false;
bool Window::s_gladInitialized = false;

Window::Window(const std::string& name, unsigned int width, unsigned int height)
  : m_name(name)
  , m_width(width)
  , m_height(height)
{
    if(!s_glfwInitialized)
    {
        if(!glfwInit())
        {
            Logger::logError("Failed to init GLFW!");
            throw(std::runtime_error("Failed to init GLFW!"));
        }
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

        s_glfwInitialized = true;
    }

    m_window = glfwCreateWindow(m_width, m_height, m_name.c_str(), nullptr, nullptr);
    glfwMakeContextCurrent(m_window);

    if(!s_gladInitialized)
    {
        if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            Logger::logError("Failed to initialize GLAD");
            throw(std::runtime_error("Failed to initialize GLAD"));
        }

        s_gladInitialized = true;
    }

    glfwSetWindowUserPointer(m_window, &m_eventCallBack);

    glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window) {
        WindowCloseEvent e;
        auto eventCallBack = *(std::function<void(Event&)>*)glfwGetWindowUserPointer(window);
        eventCallBack(e);
    });

    glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int width, int height) {
        WindowResizeEvent e(width, height);
        auto eventCallBack = *(std::function<void(Event&)>*)glfwGetWindowUserPointer(window);
        eventCallBack(e);
    });

    glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xOffset, double yOffset) {
        MouseScrolledEvent e(xOffset, yOffset);
        auto eventCallBack = *(std::function<void(Event&)>*)glfwGetWindowUserPointer(window);
        eventCallBack(e);
    });

    glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scanCode, int action, int modifiers) {
        auto eventCallBack = *(std::function<void(Event&)>*)glfwGetWindowUserPointer(window);
        if(action == GLFW_PRESS)
        {
            KeyPressedEvent e(key);
            eventCallBack(e);
        }

        if(action == GLFW_RELEASE)
        {
            KeyReleasedEvent e(key);
            eventCallBack(e);
        }

        if(action == GLFW_REPEAT)
        {
            KeyPressedEvent e(key, true);
            eventCallBack(e);
        }
    });

    glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double x, double y) {
        MouseMovedEvent e(x, y);
        auto eventCallBack = *(std::function<void(Event&)>*)glfwGetWindowUserPointer(window);
        eventCallBack(e);
    });
}

Window::~Window() {}

void Window::update()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    if(m_mainLayer != nullptr)
    {
        m_mainLayer->onUpdate();
    }

    glfwPollEvents();
    glfwSwapBuffers(m_window);
}

void Window::setEventCallBack(std::function<void(Event&)> callBack) { m_eventCallBack = callBack; }

unsigned int Window::getWidth() const { return m_width; }

unsigned int Window::getHeight() const { return m_height; }

void Window::onEvent(Event& e)
{
    EventDispatcher dispatcher(e);
    dispatcher.dispatch<core::WindowResizeEvent>(BIND_EVENT_FN(Window::onWindowResized));
    m_mainLayer->onEvent(e);
}
}
