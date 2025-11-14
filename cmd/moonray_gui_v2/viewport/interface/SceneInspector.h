// Copyright 2025 DreamWorks Animation LLC
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "Component.h"

namespace moonray_gui_v2 {

class SceneInspector : public Component {

public:
    SceneInspector() : Component(/*isOpen*/ false, /*isDocked*/ false) {}
    ~SceneInspector() override {}

    void draw(Viewport* viewport, const ImVec2& currentPixel) override;

    int getWidth() const override { return mWidth; }
    int getHeight() const override { return mHeight; }

private:
    ImVec2 getBottomRightPosition() const;

    int mWidth {300};
    int mHeight {100};
};

} // end namespace moonray_gui_v2
