// Copyright 2025 DreamWorks Animation LLC
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "Component.h"

#include <scene_rdl2/common/math/Color.h>

namespace moonray_gui_v2 {

class PixelInspector : public Component {
// Displays a small subimage around the pixel currently under the mouse cursor, along with the color value of that 
// pixel. The subimage appears as a magnified view of the area around the pixel being inspected. This means that
// we are dealing with multiple coordinate systems to get the correct portion of the framebuffer to display in the 
// subimage window:
//
// 1) ImGui window coordinates: the position within the ImGui window
// 2) Framebuffer pixel coordinates: the position within the rendered image
// 3) Texture UV coordinates: the normalized coordinates used to sample the texture
// 4) Subimage pixel coordinates: the position within the subimage display
//    (i.e. which pixel in the 5x5 grid)
//
public:
    PixelInspector() : Component(/*isOpen*/ false, /*isDocked*/ false) {}
    ~PixelInspector() override {}

    void draw(Viewport* viewport, const ImVec2& currentPixel) override;

    int getWidth() const override { return mWidth; }
    int getHeight() const override { return mHeight; }

private:
    // Get the UV coordinates for the subimage to display
    // uv0: top left corner of subimage
    // uv1: bottom right corner of subimage
    void getSubimageUVCoords(ImVec2& uv0, ImVec2& uv1, const ImVec2& currentPixel, 
                             const float textureWidth, const float textureHeight) const;

    // Set the initial position and size of the window when first opened
    void setInitialWindowPositionAndSize();

    // Get the top-left and bottom-right corners in imgui window coordinates of a "pixel" in the subimage display.
    // The input is a subimage pixel coordinate, where (0,0) is the top-left pixel of the subimage.
    void getSubimagePixelRect(ImVec2& rectMin, ImVec2& rectMax, const ImVec2& subimagePixel, 
                              const ImVec2& imageDisplaySize) const;

    // Draw an outline around the inspected pixel in the subimage display
    void drawOutlineAroundPixel(const ImVec2& imageDisplaySize);

    // Draw the color value of the inspected pixel
    void drawColorValue(const scene_rdl2::math::Color& color);

    // Draw black pixels in areas of the subimage that are outside of the framebuffer
    void drawOutOfRangePixels(const ImVec2& imageDisplaySize, const ImVec2& currentPixel, 
                              const float textureWidth, const float textureHeight);

    int mSubimageWidth {5};         // # horizontal pixels in subimage
    int mSubimageHeight {5};        // # vertical pixels in subimage
    int mWidth {200};               // default width of window when first opened
    int mHeight {200};              // default height of window when first opened
};

} // end namespace moonray_gui_v2
