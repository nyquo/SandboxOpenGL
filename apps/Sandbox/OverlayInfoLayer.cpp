#include "OverlayInfoLayer.hpp"

#include "imgui.h"

void OverlayInfoLayer::onImGuiRender()
{
    float fps = ImGui::GetIO().Framerate;

    ImGui::SetNextWindowPos(ImVec2(10, 10));
    ImGui::SetNextWindowBgAlpha(0.3f);

    if(ImGui::Begin("FPS Overlay",
                    nullptr,
                    ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove |
                      ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav))
    {
        ImGui::Text("FPS: %.1f", fps);
    }
    ImGui::End();
}
