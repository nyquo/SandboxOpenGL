#pragma once

#include "EditorLayer.hpp"

#include <core/Application.hpp>
#include <core/Events/WindowEvent.hpp>
#include <renderer/Shader.hpp>

namespace fs = std::filesystem;

class SandboxApp : public core::Application
{
  public:
    SandboxApp();

    void onEvent(core::Event& e) override;

    bool onWindowResized(core::WindowResizeEvent& e);

  private:
    std::shared_ptr<EditorLayer> m_editorLayer;
};
