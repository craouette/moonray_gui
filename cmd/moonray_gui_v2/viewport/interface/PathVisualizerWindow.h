// Copyright 2025 DreamWorks Animation LLC
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "Component.h"

#include <scene_rdl2/common/math/Color.h>

#include <functional>
#include <string>

namespace moonray { namespace rndr { class PathVisualizerManager; }}

namespace moonray_gui_v2 {

class PathVisualizerWindow : public Component {
/* This window manages the UI for the path visualizer settings and controls. It is by 
 * default docked on the righthand side of the viewport, and can be opened using the hotkey
 * in the Keyboard class. Whenever a setting is changed that might require the visualizer to 
 * re-process, we need to set "update" to true in the setter. Any purely aesthetic changes 
 * (e.g. line width/color) do not require restarting the simulation. Once the path visualizer 
 * has finished processing, it will generate line data, which will be drawn as an overlay in 
 * the ImageDisplay class.
 */
    public:
        PathVisualizerWindow() : Component(/*isOpen*/ false, /*isDocked*/ true) {}

        ~PathVisualizerWindow() override {}

        void draw(Viewport* viewport, const ImVec2& currentPixel) override;

        int getWidth() const override { return mWidth; }
        int getHeight() const override { return -1; } // auto-size height

        bool isVerticallyDocked() const override { return true; }

    private:
        // Draw enable/disable button
        void drawEnableButton();

        // Draw various submenus
        void drawPixelSelector();
        void drawSamplingSettingsMenu();
        void drawMaxDepthMenu();
        void drawVisibilityTogglesMenu();
        void drawStyleMenu();

        moonray::rndr::PathVisualizerManager* mManager {nullptr};  // Pointer to the path visualizer manager
        int mWidth {250};                                          // Width of the window
};

} // end namespace moonray_gui_v2
