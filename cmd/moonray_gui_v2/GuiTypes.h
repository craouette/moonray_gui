// Copyright 2025 DreamWorks Animation LLC
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <scene_rdl2/common/fb_util/FbTypes.h>

#include <string>

using namespace scene_rdl2;

namespace moonray_gui_v2 {

enum CameraType
{
    ORBIT_CAM,
    FREE_CAM,
    NUM_CAMERA_TYPES,
};

enum DebugMode
{
    RGB,
    RED,
    GREEN,
    BLUE,
    ALPHA,
    LUMINANCE,
    SATURATION,
    RGB_NORMALIZED,
    NUM_SAMPLES,

    NUM_DEBUG_MODES,
};

enum InspectorMode
{
    INSPECT_NONE,
    INSPECT_LIGHT_CONTRIBUTIONS,
    INSPECT_GEOMETRY,
    INSPECT_GEOMETRY_PART,
    INSPECT_MATERIAL,
    NUM_INSPECTOR_MODES
};

// Which additional buffers do we want to use for denoising.
enum DenoisingBufferMode
{
    DN_BUFFERS_BEAUTY,
    DN_BUFFERS_BEAUTY_ALBEDO,
    DN_BUFFERS_BEAUTY_ALBEDO_NORMALS,
    NUM_DENOISING_BUFFER_MODES,
};

// Define all bindable actions that can be triggered by keyboard/mouse input
enum Action
{
    ACTION_NONE = 0,

    // Camera movement actions
    ACTION_CAM_TOGGLE_ACTIVE_TYPE,
    ACTION_CAM_FORWARD,
    ACTION_CAM_BACKWARD,
    ACTION_CAM_LEFT,
    ACTION_CAM_RIGHT,
    ACTION_CAM_UP,
    ACTION_CAM_DOWN,
    ACTION_CAM_SLOW_DOWN,
    ACTION_CAM_SPEED_UP,
    ACTION_CAM_RESET,
    ACTION_CAM_RECENTER,
    ACTION_CAM_PRINT_MATRICES,
    ACTION_CAM_SET_UP_VECTOR,
    ACTION_CAM_ORBIT,
    ACTION_CAM_DOLLY,
    ACTION_CAM_PAN,
    ACTION_CAM_ROLL,

    // Image 2D Nav
    ACTION_IMAGE2D_PAN,
    ACTION_IMAGE2D_ZOOM,

    // Denoising actions
    ACTION_DENOISE_TOGGLE_ON_OFF,
    ACTION_DENOISE_TOGGLE_MODE,
    ACTION_DENOISE_SELECT_BUFFERS,    

    // Channel actions
    ACTION_CHANNEL_TOGGLE_RGB,
    ACTION_CHANNEL_TOGGLE_RED,
    ACTION_CHANNEL_TOGGLE_GREEN,
    ACTION_CHANNEL_TOGGLE_BLUE,
    ACTION_CHANNEL_TOGGLE_ALPHA,
    ACTION_CHANNEL_TOGGLE_LUMINANCE,
    ACTION_CHANNEL_TOGGLE_RGB_NORMALIZED,
    ACTION_CHANNEL_TOGGLE_NUM_SAMPLES,

    // Image adjustment actions
    ACTION_EXPOSURE_INCREASE,
    ACTION_EXPOSURE_DECREASE,
    ACTION_EXPOSURE_ADJUST,
    ACTION_EXPOSURE_RESET,
    ACTION_GAMMA_ADJUST,
    ACTION_GAMMA_RESET,
    ACTION_OCIO_TOGGLE,

    // Fast progressive mode actions
    ACTION_FAST_PROGRESSIVE_TOGGLE,
    ACTION_FAST_PROGRESSIVE_NEXT_MODE,
    ACTION_FAST_PROGRESSIVE_PREV_MODE,

    // Window toggles
    ACTION_WINDOW_TOGGLE_EXPOSURE,
    ACTION_WINDOW_TOGGLE_GAMMA,
    ACTION_WINDOW_TOGGLE_INSPECTOR,
    ACTION_WINDOW_TOGGLE_PATH_VISUALIZER,
    ACTION_WINDOW_TOGGLE_PIXEL_INSPECTOR,
    ACTION_WINDOW_TOGGLE_SNAPSHOT,

    // Output actions
    ACTION_RENDER_OUTPUT_PREV,
    ACTION_RENDER_OUTPUT_NEXT,
    ACTION_SAVE_IMAGE,
    ACTION_SNAPSHOT_TAKE,
    ACTION_SNAPSHOT_PREV,
    ACTION_SNAPSHOT_NEXT,

    // Misc actions
    ACTION_PICK_PATH_VISUALIZER_PIXEL,
    ACTION_TILE_PROGRESS_TOGGLE,
    ACTION_PRINT_KEYBINDINGS,

    // Add more actions here as needed
    ACTION_COUNT // Keep this as the last element to count the number of actions
};

// Get the human-readable action name
std::string getActionName(const Action& action);

// Get the human-readable key name
std::string getKeyName(const int& glfwKey);

// Get the human-readable modifier name, in the format: NAME+
std::string getModifierName(const int& mod);

} // namespace moonray_gui_v2

