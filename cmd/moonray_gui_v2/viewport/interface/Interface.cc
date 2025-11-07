
// Copyright 2025 DreamWorks Animation LLC
// SPDX-License-Identifier: Apache-2.0

#include "Interface.h"

#include "ImageDisplay.h"
#include "../Viewport.h"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include <stdexcept>    // for std::runtime_error

namespace moonray_gui_v2 {

Interface::Interface(Viewport* viewport)
: mViewport(viewport)
, mImageDisplay(std::make_unique<ImageDisplay>())
{
    if (!viewport) {
        throw std::invalid_argument("Viewport cannot be null");
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    if (!ImGui_ImplGlfw_InitForOpenGL(mViewport->getGLFWWindow(), /*automatic callback install*/ false)) {
        throw std::runtime_error("Failed to initialize ImGui GLFW implementation");
    }
    
    if (!ImGui_ImplOpenGL3_Init()) {
        ImGui_ImplGlfw_Shutdown();
        throw std::runtime_error("Failed to initialize ImGui OpenGL3 implementation");
    }

    // Configure ImGui settings
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_NavEnableGamepad;
}

Interface::~Interface()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void
Interface::resizeImage(const int width, const int height)
{
    // Update image size for the display component
    mImageDisplay->setImageSize(width, height);
    glViewport(0, 0, width, height);
}

void
Interface::draw()
{
    // Request a new frame
    newFrame(); 
  
    // Draw UI components
    const int textureHandle = mViewport->getFramebufferTexture();
    mImageDisplay->draw(textureHandle);

    // Set up proper OpenGL viewport and clear framebuffer before rendering
    glViewport(0, 0, mViewport->getFramebufferWidth(), mViewport->getFramebufferHeight());
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  // Black background
    glClear(GL_COLOR_BUFFER_BIT);

    render();
}

void
Interface::render()
{
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

} // namespace moonray_gui_v2
