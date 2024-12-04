#include "Sandbox.hpp"

SandboxApp::SandboxApp()
  : core::Application("Sandbox")
{
    renderer::Shader shader(fs::path(RESSOURCES_FOLDER) / "shaders" / "LightCubeShader.vert",
                            fs::path(RESSOURCES_FOLDER) / "shaders" / "LightCubeShader.frag");
}
