#include "Window.hpp"

#include "Events/KeyEvent.hpp"
#include "Events/MouseEvent.hpp"
#include "Events/WindowEvent.hpp"
#include "Logger.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

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
        glfwWindowHint(GLFW_SAMPLES, 4);

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

        // configure global opengl state
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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

    IMGUI_CHECKVERSION();

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

Window::~Window()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void Window::onUpdate()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glfwPollEvents();

    for(auto& layer : m_layers)
    {
        layer->onUpdate();
    }

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    for(auto& layer : m_uiLayers)
    {
        layer->onUpdate();
    }
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(m_window);
}

void Window::setEventCallBack(std::function<void(Event&)> callBack) { m_eventCallBack = callBack; }

unsigned int Window::getWidth() const { return m_width; }

unsigned int Window::getHeight() const { return m_height; }

bool Window::isKeyPressed(int key) const { return glfwGetKey(m_window, key) == GLFW_PRESS; }

bool Window::isMouseButtonPressed(int button) const { return glfwGetMouseButton(m_window, button) == GLFW_PRESS; }

glm::vec2 Window::getMousePosition() const
{
    double x, y;
    glfwGetCursorPos(m_window, &x, &y);
    return {(float)x, (float)y};
}

void Window::pushLayer(std::shared_ptr<Layer> layer)
{
    if(std::find(m_layers.begin(), m_layers.end(), layer) == m_layers.end())
    {
        layer->setWindow(this);
        m_layers.push_back(layer);
    }
}

void Window::pushUiLayer(std::shared_ptr<Layer> layer)
{
    if(std::find(m_uiLayers.begin(), m_uiLayers.end(), layer) == m_uiLayers.end())
    {
        layer->setWindow(this);
        m_uiLayers.push_back(layer);
    }
}

void Window::removeLayer(std::shared_ptr<Layer> layer)
{
    auto itLayer = std::find(m_layers.begin(), m_layers.end(), layer);
    if(itLayer != m_layers.end())
    {
        m_layers.erase(itLayer);
    }
}

void Window::removeUiLayer(std::shared_ptr<Layer> layer)
{
    auto itLayer = std::find(m_uiLayers.begin(), m_uiLayers.end(), layer);
    if(itLayer != m_uiLayers.end())
    {
        m_uiLayers.erase(itLayer);
    }
}

void Window::onEvent(Event& e)
{
    EventDispatcher dispatcher(e);
    dispatcher.dispatch<core::WindowResizeEvent>(BIND_EVENT_FN(Window::onWindowResized));
    for(auto& layer : m_layers)
    {
        layer->onEvent(e);
    }
    for(auto& layer : m_uiLayers)
    {
        layer->onEvent(e);
    }
}
}
