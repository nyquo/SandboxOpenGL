#pragma once

#include <Application.hpp>
#include <Events/Event.hpp>
#include <Layer.hpp>
#include <Logger.hpp>

class CustomLayer : public core::Layer
{
  public:
    void onUpdate() override
    {
        glClearColor(0.008f, 0.082f, 0.149f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void onEvent(core::Event& event) override {}
};

class HelloWordApp : public core::Application
{
  public:
    HelloWordApp() { getWindow().setLayer(std::make_unique<CustomLayer>()); }
};
