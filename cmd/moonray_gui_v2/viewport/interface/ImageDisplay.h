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
    void draw(GLuint textureHandle);

    // Get the (unscaled) size of the image
    ImVec2 getImageSize() const { return mImageSize; }

    // Set the base image size (for resizing in the rdla)
    void setImageSize(const int width, const int height) { mImageSize = ImVec2(width, height); }

private:
    ImVec2 mImageSize {ImVec2(0,0)};    // size of the image
    ImVec2 mPosition  {ImVec2(0,0)};    // position of the image in the viewport
};

} // end namespace moonray_gui_v2
