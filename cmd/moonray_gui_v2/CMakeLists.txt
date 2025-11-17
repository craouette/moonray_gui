# Copyright 2025 DreamWorks Animation LLC
# SPDX-License-Identifier: Apache-2.0

find_package(glfw3 REQUIRED)

set(target moonray_gui_v2)

add_executable(${target})

# -----------------------------------------

target_sources(${target}
    PRIVATE
        moonray_gui_v2.cc
        ../../ext/imgui/imgui.cpp
        ../../ext/imgui/backends/imgui_impl_glfw.cpp
        ../../ext/imgui/backends/imgui_impl_opengl3.cpp
        ../../ext/imgui/imgui_draw.cpp
        ../../ext/imgui/imgui_tables.cpp
        ../../ext/imgui/imgui_widgets.cpp
        ../../ext/imgui/imgui_demo.cpp
       
        camera/FreeCam.cc
        camera/OrbitCam.cc
        ColorManager.cc
        DenoiserManager.cc
        FileUtil.cc
        GuiTypes.cc
        RenderGui.cc

        viewport/interface/DrawUtil.cc
        viewport/interface/ExposureWindow.cc
        viewport/interface/GammaWindow.cc
        viewport/interface/Interface.cc
        viewport/interface/ImageDisplay.cc
        viewport/interface/PathVisualizerWindow.cc
        viewport/interface/PixelInspector.cc
        viewport/interface/SceneInspector.cc
        viewport/interface/StatusBar.cc

        viewport/Keyboard.cc
        viewport/SnapshotManager.cc
        viewport/Viewport.cc
)

if (NOT IsDarwinPlatform)
    set(PlatformSpecificLibs atomic)
endif()

# ==============================================
# Setup ImGui
# ==============================================

# Dear ImGui
target_include_directories(${target} PRIVATE ${PROJECT_SOURCE_DIR}/ext/imgui)

target_link_libraries(${target}
    PRIVATE
        McrtDenoise::denoiser
        ${MKL_target}
        ${OCIO}
        Moonray::application
        Moonray::rendering_rndr
        SceneRdl2::common_fb_util
        SceneRdl2::common_math
        SceneRdl2::common_platform
        SceneRdl2::render_logging
        SceneRdl2::render_util
        SceneRdl2::scene_rdl2
        OpenGL::GL
        glfw
        ${PlatformSpecificLibs}
)

# Set standard compile/link options
MoonrayGui_cxx_compile_definitions(${target})
MoonrayGui_cxx_compile_features(${target})
MoonrayGui_cxx_compile_options(${target})
MoonrayGui_link_options(${target})

# Disable OCIO if not found
if (NOT OpenColorIO_FOUND)
    target_compile_definitions(${target} PRIVATE DISABLE_OCIO)
endif()

install(TARGETS ${target}
    RUNTIME DESTINATION bin)

