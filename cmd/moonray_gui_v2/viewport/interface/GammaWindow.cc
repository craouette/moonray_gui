// Copyright 2025 DreamWorks Animation LLC
// SPDX-License-Identifier: Apache-2.0

#include "GammaWindow.h"

#include "../Viewport.h"

namespace moonray_gui_v2 {

ImVec2
GammaWindow::getCenteredPosition() const
{
    ImGuiViewport* imguiViewport = ImGui::GetMainViewport();
    return ImVec2((imguiViewport->Size.x - mWidth) * 0.5f, (imguiViewport->Size.y - mHeight) * 0.5f);
}

void
GammaWindow::draw(Viewport* viewport, const ImVec2& /*currentPixel*/)
{
    if (!mOpen) { return; }

    // Center the window in the viewport when first opened
    ImGui::SetNextWindowPos(getCenteredPosition(), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(mWidth, mHeight), ImGuiCond_Once);

    ImGui::Begin("Gamma", &mOpen, ImGuiWindowFlags_NoResize);
    // Get ptr to the editable gamma amount
    float* gamma = viewport->getGammaPtr();
    ImGui::SliderFloat("##Gamma", gamma, 0.1f, 5.0f);

    if (ImGui::Button("Reset")) {
        *gamma = 1.0f;
    }
    ImGui::End();
}

} // namespace moonray_gui_v2
