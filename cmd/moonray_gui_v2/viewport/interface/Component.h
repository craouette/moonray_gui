// Copyright 2025 DreamWorks Animation LLC
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "imgui.h"

#include <GLFW/glfw3.h>

namespace moonray_gui_v2 {

class Viewport;

class Component {
/* An abstract base class for all UI components in the ImGui interface. It 
   defines the basic interface that all components must implement, such as drawing
   the component and toggling its open/closed state.
   
   NOTE: imgui has a "docking" feature that allows windows to be docked to the main viewport or other windows.
   However, while we use the term "docked" here, it does not refer to imgui's docking feature. Instead, it 
   simply refers to stationary windows that affect the size of the viewport, and they are managed manually by us.
   When the imgui "docking" feature is further along in development, we may switch to using that instead.
*/
public:
    Component(bool openDefault, bool dockedDefault) : mOpen(openDefault), mDocked(dockedDefault) {}
    virtual ~Component() {}
    
    // Draw the component
    virtual void draw(Viewport* viewport, const ImVec2& currentPixel) = 0;

    // Toggle the window open/closed
    bool toggle() { return mOpen = !mOpen; }

    // Does the component affect viewport size?
    bool isDocked() const { return mDocked; }

    // Is the component open?
    bool isOpen() const { return mOpen; }

    // Get the width of the component
    virtual int getWidth() const = 0;

    // Get the height of the component
    virtual int getHeight() const = 0;

    // Is the component docked horizontally or vertically?
    virtual bool isHorizontallyDocked() const { return false; }
    virtual bool isVerticallyDocked() const { return false; }

protected:
    bool mOpen;         // Is the window open?
    bool mDocked;       // Does the component affect viewport size?
};

} // end namespace moonray_gui_v2
