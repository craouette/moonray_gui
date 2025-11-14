// Copyright 2025 DreamWorks Animation LLC
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "Component.h"

namespace moonray_gui_v2 {

class GammaWindow : public Component {
public:
    GammaWindow() : Component(/*isOpen*/ false, /*isDocked*/ false) {}

    // Draw the window
    void draw(Viewport* viewport, const ImVec2& currentPixel) override;

    int getWidth() const override { return mWidth; }
    int getHeight() const override { return mHeight; }

private:
    // Get the position of the window, centered in the viewport
    ImVec2 getCenteredPosition() const;

    int mWidth {100};       // Width of the window
    int mHeight {77};       // Height of the window
};

} // end namespace moonray_gui_v2
