
// Copyright 2025 DreamWorks Animation LLC
// SPDX-License-Identifier: Apache-2.0

#include "Interface.h"

#include "ExposureWindow.h"
#include "GammaWindow.h"
#include "ImageDisplay.h"
#include "PathVisualizerWindow.h"
#include "PixelInspector.h"
#include "SceneInspector.h"
#include "StatusBar.h"
#include "../Viewport.h"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include <stdexcept>    // for std::runtime_error

namespace moonray_gui_v2 {

Interface::Interface(Viewport* viewport)
: mViewport(viewport)
, mExposureWindow(std::make_unique<ExposureWindow>())
, mGammaWindow(std::make_unique<GammaWindow>())
, mImageDisplay(std::make_unique<ImageDisplay>())
, mPathVisualizerWindow(std::make_unique<PathVisualizerWindow>())
, mPixelInspector(std::make_unique<PixelInspector>())
, mSceneInspector(std::make_unique<SceneInspector>())
, mStatusBar(std::make_unique<StatusBar>())
{
    if (!viewport) {
        throw std::invalid_argument("Viewport cannot be null");
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    // Enable automatic installation of callbacks so that ImGui can handle input events
    // (e.g. slider dragging, button clicks, etc.) within ImGui windows automatically
    if (!ImGui_ImplGlfw_InitForOpenGL(mViewport->getGLFWWindow(), /*automatic callback install*/ true)) {
        throw std::runtime_error("Failed to initialize ImGui GLFW implementation");
    }
    
    if (!ImGui_ImplOpenGL3_Init()) {
        ImGui_ImplGlfw_Shutdown();
        throw std::runtime_error("Failed to initialize ImGui OpenGL3 implementation");
    }

    // Configure ImGui settings
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_NavEnableGamepad;

    // Add UI components here
    mComponents.push_back(mExposureWindow.get());
    mComponents.push_back(mGammaWindow.get());
    mComponents.push_back(mPathVisualizerWindow.get());
    mComponents.push_back(mPixelInspector.get());
    mComponents.push_back(mSceneInspector.get());
    mComponents.push_back(mStatusBar.get());
}

Interface::~Interface()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

bool
Interface::handleKeyPressEvent(const Action action)
{
    switch(action) {
    case ACTION_WINDOW_TOGGLE_EXPOSURE:         toggleExposureWindow();         return true;
    case ACTION_WINDOW_TOGGLE_GAMMA:            toggleGammaWindow();            return true;
    case ACTION_WINDOW_TOGGLE_PATH_VISUALIZER:  togglePathVisualizerWindow();   return true;
    case ACTION_WINDOW_TOGGLE_PIXEL_INSPECTOR:  togglePixelInspector();         return true;
    case ACTION_WINDOW_TOGGLE_SCENE_INSPECTOR:  toggleSceneInspector();         return true;
    case ACTION_WINDOW_TOGGLE_STATUS:           toggleStatusBar();              return true;
    default: break;
    }
    return false;
}

void
Interface::draw()
{
    // Request a new frame
    newFrame(); 

    // Get the current pixel under the mouse
    const ImVec2 currentPixel = getCurrentPixel();

    // Draw all components
    for (Component* component : mComponents) {
        component->draw(mViewport, currentPixel);
    }

    // Resize the viewport if necessary
    resizeViewport();

    // Get how much space is available for the image display
    ImGuiViewport* imguiViewport = ImGui::GetMainViewport();
    const int availWidth = imguiViewport->Size.x - mDockedComponentsWidth;
    const int availHeight = imguiViewport->Size.y - mDockedComponentsHeight;
  
    // Draw the image
    mImageDisplay->draw(mViewport, availWidth, availHeight);

    render();
}

void
Interface::render()
{
    // Set up proper OpenGL viewport and clear framebuffer before rendering
    glViewport(0, 0, mViewport->getFramebufferWidth(), mViewport->getFramebufferHeight());
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  // Black background
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void
Interface::newFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void
Interface::sleep(int milliseconds)
{
    ImGui_ImplGlfw_Sleep(milliseconds);
}

// --------------------------- Sizing/Resizing ---------------------------- ///

int
Interface::getDockedComponentsWidth() const
{ 
    int width = 0;
    for (const auto& component : mComponents) {
        if (component->isDocked() && component->isOpen()) {
            // We don't care about horizontally docked components here
            // since they don't affect viewport width.
            if (!component->isHorizontallyDocked()) {
                width += component->getWidth();
            }
        } 
    } 
    return width; 
}
int
Interface::getDockedComponentsHeight() const
{
    int height = 0;
    for (const auto& component : mComponents) {
        if (component->isDocked() && component->isOpen()) {
            // We don't care about vertically docked components here
            // since they don't affect viewport height.
            if (!component->isVerticallyDocked()) {
                height += component->getHeight();
            }
        } 
    } 
    return height;
}

void
Interface::resizeImage(const int width, const int height)
{
    // Update image size for the display component
    mImageDisplay->setImageSize(width, height);
    glViewport(0, 0, width, height);
}

void
Interface::resizeViewport()
{
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    if (viewport->Size.x == 0 && viewport->Size.y == 0) {
        // Viewport size is not yet initialized
        return;
    }

    // Get the difference between the current width/height 
    // of the docked components and the previous values
    const int newDockedComponentsWidth = getDockedComponentsWidth();
    const int newDockedComponentsHeight = getDockedComponentsHeight();
    const int dockedComponentsWidthDiff = newDockedComponentsWidth - mDockedComponentsWidth;
    const int dockedComponentsHeightDiff = newDockedComponentsHeight - mDockedComponentsHeight;

    // If there's a change, a docked component has been opened/closed
    if (dockedComponentsWidthDiff != 0 || dockedComponentsHeightDiff != 0) {
        mDockedComponentsWidth = newDockedComponentsWidth;
        mDockedComponentsHeight = newDockedComponentsHeight;

        // Update the viewport size
        const int vpWidth = viewport->Size.x + dockedComponentsWidthDiff;
        const int vpHeight = viewport->Size.y + dockedComponentsHeightDiff;
        glfwSetWindowSize(mViewport->getGLFWWindow(), vpWidth, vpHeight);
    }
}

// ------------------- Toggle open/closed windows ----------------------- ///

void Interface::toggleExposureWindow() { mExposureWindow->toggle(); }
void Interface::toggleGammaWindow() { mGammaWindow->toggle(); }
void Interface::togglePathVisualizerWindow() { mPathVisualizerWindow->toggle(); }
void Interface::togglePixelInspector() { mPixelInspector->toggle(); }
void Interface::toggleSceneInspector() { mSceneInspector->toggle(); }
void Interface::toggleStatusBar() { mStatusBar->toggle(); }

/// ----------------------- Image Operations ---------------------------- ///
ImVec2
Interface::getCurrentPixel() const
{
    ImGuiIO& io = ImGui::GetIO();
    const int mousePosX = io.MousePos.x;
    const int mousePosY = io.MousePos.y;

    return mImageDisplay->viewportToImageCoords(mousePosX, mousePosY);
}

void
Interface::zoom(const int xoffset, const int yoffset)
{
    mImageDisplay->zoom(xoffset, yoffset);
}

void
Interface::pan(const int xoffset, const int yoffset)
{
    mImageDisplay->pan(xoffset, yoffset);
}

} // namespace moonray_gui_v2
