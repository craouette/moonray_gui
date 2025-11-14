// Copyright 2025 DreamWorks Animation LLC
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "imgui.h"

#include <GLFW/glfw3.h>     // for GLuint

namespace moonray_gui_v2 {

class ImageDisplay {
public:
    ImageDisplay() {}

    // Draw the window to the screen
    void draw(GLuint textureHandle, const int availWidth, const int availHeight);

    // Get the coordinate for the pixel underneath 
    // the mouse at the given position
    ImVec2 getPixel(int mousePosX, int mousePosY) const;

    // Get the (unscaled) size of the image
    ImVec2 getImageSize() const { return mImageSize; }

    // Get the (scaled) size of the image
    ImVec2 getScaledSize() const { return ImVec2(mImageSize.x * mZoomScale, mImageSize.y * mZoomScale); }

    // Set the base image size (for resizing in the rdla)
    void setImageSize(const int width, const int height) { mImageSize = ImVec2(width, height); }

    // Zoom in/out (i.e. increase/decrease the image scale)
    void zoom(const int xoffset, const int yoffset);

    // Pan the image by the given offset
    void pan(const int xoffset, const int yoffset);

private:
    // Get the position that will center the image in the viewport
    ImVec2 getCenteredPosition(const int availWidth, const int availHeight) const;

    ImVec2 mImageSize {ImVec2(0,0)};        // size of the image
    ImVec2 mPosition  {ImVec2(0,0)};        // position of the image in the viewport

    ImVec2 mPositionOffset {ImVec2(0,0)};   // panning offset
    float mZoomScale {1.0f};                // zoom scale
};

} // end namespace moonray_gui_v2
