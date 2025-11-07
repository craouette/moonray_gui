// Copyright 2025 DreamWorks Animation LLC
// SPDX-License-Identifier: Apache-2.0

#include "ImageDisplay.h"

namespace moonray_gui_v2 {

void
ImageDisplay::draw(const GLuint textureHandle)
{
    // Only show if we have valid data
    if (textureHandle == 0 || mImageSize.x <= 0 || mImageSize.y <= 0) {
        return;
    }

    // --------------- Set position/size ----------------
    ImGui::SetNextWindowSize(mImageSize);

    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    // Center image in the viewport
    mPosition = ImVec2((viewport->WorkSize.x - mImageSize.x) * 0.5f, (viewport->WorkSize.y - mImageSize.y) * 0.5f);
    ImGui::SetNextWindowPos(mPosition);
    // -------------------------------------------------

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
    
    // Use consistent window flags and ID to ensure single window instance
    if (ImGui::Begin("ImageViewport", nullptr, 
                    ImGuiWindowFlags_NoDecoration | 
                    ImGuiWindowFlags_NoMove | 
                    ImGuiWindowFlags_NoResize |
                    ImGuiWindowFlags_NoScrollbar |
                    ImGuiWindowFlags_NoScrollWithMouse |
                    ImGuiWindowFlags_NoInputs |
                    ImGuiWindowFlags_NoBringToFrontOnFocus)) {
        
        // Ensure cursor is at origin for precise positioning
        ImGui::SetCursorPos(ImVec2(0, 0));
        
        // Draw the image (Note that ImGui uses top-left origin for UVs, OpenGL uses bottom-left)
        ImGui::Image((ImTextureID)(intptr_t)textureHandle, mImageSize, ImVec2(0, 1), ImVec2(1, 0));
    }
    ImGui::End();
    ImGui::PopStyleVar(2); 
}

} // namespace moonray_gui_v2
