// Copyright 2025 DreamWorks Animation LLC
// SPDX-License-Identifier: Apache-2.0

#include "ImageDisplay.h"

#include <cmath>

namespace moonray_gui_v2 {

constexpr float MAX_ZOOM = 30.0f;   // max zoom in (30x)
constexpr float MIN_ZOOM = 0.1f;    // max zoom out (0.1x)
constexpr float ZOOM_STEP = 1.05f;  // zoom step factor

ImVec2 
ImageDisplay::getCenteredPosition(const int availWidth, const int availHeight) const
{
    ImVec2 scaledSize = getScaledSize();
    return ImVec2((availWidth - scaledSize.x) * 0.5f, (availHeight - scaledSize.y) * 0.5f);
}

void
ImageDisplay::draw(const GLuint textureHandle, const int availWidth, const int availHeight)
{
    // Only show if we have valid data
    if (textureHandle == 0 || mImageSize.x <= 0 || mImageSize.y <= 0 || availWidth <= 0 || availHeight <= 0) {
        return;
    }

    // --------------- Set position/size ----------------
    // Get the scaled image size to account for zoom in/out
    ImGui::SetNextWindowSize(getScaledSize());

    // Center image in the viewport
    mPosition = getCenteredPosition(availWidth, availHeight);

    // Apply any panning offset
    mPosition.x += mPositionOffset.x;
    mPosition.y += mPositionOffset.y;
    ImGui::SetNextWindowPos(mPosition);
    // -------------------------------------------------

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
    
    // This window will just contain the image, no decorations or inputs
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
        ImGui::Image((ImTextureID)(intptr_t)textureHandle, getScaledSize(), ImVec2(0, 1), ImVec2(1, 0));
    }
    ImGui::End();
    ImGui::PopStyleVar(2); 
}

ImVec2
ImageDisplay::getPixel(int mousePosX, int mousePosY) const
{
    const ImVec2 imageSize = getScaledSize();

    // Offset by the image position
    // to put mousePos in image coordinates
    mousePosX -= mPosition.x;
    mousePosY -= mPosition.y;

    if (mousePosX >= 0 && mousePosY >= 0 && mousePosX < imageSize.x && mousePosY < imageSize.y) {
        // Adjust mouse pos to account for difference between texture size and ui size
        // i.e. if the ui is scaled up, we need to scale the mouse pos down to get the correct pixel
        // Also, since the pixel coord is the center of the pixel, we need to subtract 0.5
        // to get the top left corner of the pixel before scaling
        mousePosX = std::floor((mousePosX - 0.5f) * (1.f/mZoomScale));
        mousePosY = std::floor((mousePosY - 0.5f) * (1.f/mZoomScale));
        return ImVec2(mousePosX, mImageSize.y - 1 - mousePosY);
    }
    return ImVec2(-1, -1);
}

void
ImageDisplay::zoom(const int xoffset, const int yoffset)
{
    if (yoffset > 0 && mZoomScale < MAX_ZOOM) {
        mZoomScale *= ZOOM_STEP;
        // Adjust position offset to zoom towards center
        // As the image size increases, the offset point
        // moves further away from the center, so we need to scale it proportionally
        mPositionOffset.x *= ZOOM_STEP;
        mPositionOffset.y *= ZOOM_STEP;
    } else if (yoffset <= 0 && mZoomScale > MIN_ZOOM) {
        mZoomScale /= ZOOM_STEP;
        // Adjust position offset to zoom away from center
        // As the image size decreases, the offset point
        // moves closer to the center, so we need to scale it proportionally
        mPositionOffset.x /= ZOOM_STEP;
        mPositionOffset.y /= ZOOM_STEP;
    }
}

void
ImageDisplay::pan(const int xoffset, const int yoffset)
{
    mPositionOffset.x = mPositionOffset.x + xoffset;
    mPositionOffset.y = mPositionOffset.y + yoffset;
}

} // namespace moonray_gui_v2
