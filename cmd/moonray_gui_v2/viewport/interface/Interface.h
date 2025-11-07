// Copyright 2025 DreamWorks Animation LLC
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <memory>   // for std::unique_ptr

namespace moonray_gui_v2 {

class ImageDisplay;
class Viewport;

class Interface
{
    /* This class manages the ImGui user interface for the application. Any key/mouse interactions in the 
       viewport (i.e. the area that contains the rendered image) are managed by GLFW in the Viewport class. 
       However, any interactions with menus or interactive windows are managed by this class. */

public:
    Interface(Viewport* viewport);
    ~Interface();

    // Draw the UI
    void draw();

    // Sleep for the designated number of ms
    void sleep(const int milliseconds = 10);

    // Resize the image display
    void resizeImage(const int width, const int height);

private:
    // Render the UI
    void render();

    // Create a new frame
    void newFrame();

    Viewport* mViewport;                                        // pointer to the viewport manager
    std::unique_ptr<ImageDisplay> mImageDisplay {nullptr};      // ptr to gui component displaying the rendered image
};

} // namespace moonray_gui_v2
