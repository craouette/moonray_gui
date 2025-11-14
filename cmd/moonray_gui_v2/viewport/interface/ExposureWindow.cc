// Copyright 2025 DreamWorks Animation LLC
// SPDX-License-Identifier: Apache-2.0

#include "ExposureWindow.h"

#include "../Viewport.h"

namespace moonray_gui_v2 {

ImVec2
ExposureWindow::getCenteredPosition() const
{
    ImGuiViewport* imguiViewport = ImGui::GetMainViewport();
    return ImVec2((imguiViewport->Size.x - mWidth) * 0.5f, (imguiViewport->Size.y - mHeight) * 0.5f);
}

void
ExposureWindow::draw(Viewport* viewport, const ImVec2& /*currentPixel*/)
{
    if (!mOpen) { return; }

    // Center the window in the viewport when first opened
    ImGui::SetNextWindowPos(getCenteredPosition(), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(mWidth, mHeight), ImGuiCond_Once);

    ImGui::Begin("Exposure", &mOpen, ImGuiWindowFlags_NoResize);
    // Get ptr to the editable exposure amount
    float* exposure = viewport->getExposurePtr();
    ImGui::SliderFloat("##Exposure", exposure, -10.0f, 10.0f);

    if (ImGui::Button("Reset")) {
        *exposure = 0.0f;
    }
    ImGui::End();
}

} // namespace moonray_gui_v2
