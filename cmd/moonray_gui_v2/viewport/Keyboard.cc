// Copyright 2025 DreamWorks Animation LLC
// SPDX-License-Identifier: Apache-2.0

#include "Keyboard.h"

namespace moonray_gui_v2 {

#define MOD_NONE 0

// ----------------------------------------- KeyboardBindings Class ------------------------------------------------ ///
KeyboardBindings::KeyboardBindings()
{
    // ------------- Key mappings ------------------
    mKeyPressMap[KeyModPair(GLFW_KEY_1, MOD_NONE)]                = ACTION_CHANNEL_TOGGLE_RED;
    mKeyPressMap[KeyModPair(GLFW_KEY_2, MOD_NONE)]                = ACTION_CHANNEL_TOGGLE_GREEN;
    mKeyPressMap[KeyModPair(GLFW_KEY_3, MOD_NONE)]                = ACTION_CHANNEL_TOGGLE_BLUE;
    mKeyPressMap[KeyModPair(GLFW_KEY_4, MOD_NONE)]                = ACTION_CHANNEL_TOGGLE_ALPHA;
    mKeyPressMap[KeyModPair(GLFW_KEY_5, MOD_NONE)]                = ACTION_CHANNEL_TOGGLE_LUMINANCE;
    mKeyPressMap[KeyModPair(GLFW_KEY_6, MOD_NONE)]                = ACTION_CHANNEL_TOGGLE_RGB_NORMALIZED;
    mKeyPressMap[KeyModPair(GLFW_KEY_7, MOD_NONE)]                = ACTION_CHANNEL_TOGGLE_NUM_SAMPLES;

    mKeyPressMap[KeyModPair(GLFW_KEY_A, MOD_NONE)]                = ACTION_CAM_LEFT;
    mKeyPressMap[KeyModPair(GLFW_KEY_B, MOD_NONE)]                = ACTION_DENOISE_SELECT_BUFFERS;
    mKeyPressMap[KeyModPair(GLFW_KEY_C, MOD_NONE)]                = ACTION_CAM_DOWN;
    mKeyPressMap[KeyModPair(GLFW_KEY_D, MOD_NONE)]                = ACTION_CAM_RIGHT;
    mKeyPressMap[KeyModPair(GLFW_KEY_E, MOD_NONE)]                = ACTION_CAM_SPEED_UP;
    mKeyPressMap[KeyModPair(GLFW_KEY_F, MOD_NONE)]                = ACTION_CAM_RECENTER;
    mKeyPressMap[KeyModPair(GLFW_KEY_H, MOD_NONE)]                = ACTION_PRINT_KEYBINDINGS;
    mKeyPressMap[KeyModPair(GLFW_KEY_K, MOD_NONE)]                = ACTION_SNAPSHOT_TAKE;
    mKeyPressMap[KeyModPair(GLFW_KEY_L, MOD_NONE)]                = ACTION_FAST_PROGRESSIVE_TOGGLE;
    mKeyPressMap[KeyModPair(GLFW_KEY_M, MOD_NONE)]                = ACTION_CAM_PRINT_MATRICES;
    mKeyPressMap[KeyModPair(GLFW_KEY_N, MOD_NONE)]                = ACTION_DENOISE_TOGGLE_ON_OFF;
    mKeyPressMap[KeyModPair(GLFW_KEY_O, MOD_NONE)]                = ACTION_CAM_TOGGLE_ACTIVE_TYPE;
    mKeyPressMap[KeyModPair(GLFW_KEY_Q, MOD_NONE)]                = ACTION_CAM_SLOW_DOWN;
    mKeyPressMap[KeyModPair(GLFW_KEY_R, MOD_NONE)]                = ACTION_CAM_RESET;
    mKeyPressMap[KeyModPair(GLFW_KEY_S, MOD_NONE)]                = ACTION_CAM_BACKWARD;
    mKeyPressMap[KeyModPair(GLFW_KEY_T, MOD_NONE)]                = ACTION_TILE_PROGRESS_TOGGLE;
    mKeyPressMap[KeyModPair(GLFW_KEY_U, MOD_NONE)]                = ACTION_CAM_SET_UP_VECTOR;
    mKeyPressMap[KeyModPair(GLFW_KEY_W, MOD_NONE)]                = ACTION_CAM_FORWARD;
    mKeyPressMap[KeyModPair(GLFW_KEY_Z, MOD_NONE)]                = ACTION_OCIO_TOGGLE;

    mKeyPressMap[KeyModPair(GLFW_KEY_PERIOD, MOD_NONE)]           = ACTION_RENDER_OUTPUT_NEXT;
    mKeyPressMap[KeyModPair(GLFW_KEY_COMMA, MOD_NONE)]            = ACTION_RENDER_OUTPUT_PREV;
    mKeyPressMap[KeyModPair(GLFW_KEY_GRAVE_ACCENT, MOD_NONE)]     = ACTION_CHANNEL_TOGGLE_RGB;
    mKeyPressMap[KeyModPair(GLFW_KEY_UP, MOD_NONE)]               = ACTION_EXPOSURE_INCREASE;
    mKeyPressMap[KeyModPair(GLFW_KEY_DOWN, MOD_NONE)]             = ACTION_EXPOSURE_DECREASE;
    mKeyPressMap[KeyModPair(GLFW_KEY_SPACE, MOD_NONE)]            = ACTION_CAM_UP;
    
    mKeyPressMap[KeyModPair(GLFW_KEY_N, GLFW_MOD_SHIFT)]          = ACTION_DENOISE_TOGGLE_MODE;
    mKeyPressMap[KeyModPair(GLFW_KEY_X, GLFW_MOD_SHIFT)]          = ACTION_EXPOSURE_RESET;
    mKeyPressMap[KeyModPair(GLFW_KEY_Y, GLFW_MOD_SHIFT)]          = ACTION_GAMMA_RESET;

    mKeyPressMap[KeyModPair(GLFW_KEY_UP, GLFW_MOD_ALT)]           = ACTION_FAST_PROGRESSIVE_NEXT_MODE;
    mKeyPressMap[KeyModPair(GLFW_KEY_DOWN, GLFW_MOD_ALT)]         = ACTION_FAST_PROGRESSIVE_PREV_MODE;
    
    mKeyPressMap[KeyModPair(GLFW_KEY_S, GLFW_MOD_CONTROL)]        = ACTION_SAVE_IMAGE;

    // ---------------- Mouse mappings ------------------
    mMousePressMap[KeyModPair(GLFW_MOUSE_BUTTON_LEFT, GLFW_MOD_ALT)]       = ACTION_CAM_ORBIT;
    mMousePressMap[KeyModPair(GLFW_MOUSE_BUTTON_MIDDLE, GLFW_MOD_ALT)]     = ACTION_CAM_PAN;
    mMousePressMap[KeyModPair(GLFW_MOUSE_BUTTON_RIGHT, GLFW_MOD_ALT)]      = ACTION_CAM_DOLLY;
    mMousePressMap[KeyModPair(GLFW_MOUSE_BUTTON_LEFT, GLFW_MOD_CONTROL)]   = ACTION_CAM_ROLL;

    // NOTE: If you use a GLFW_KEY as a modifier, you need to check for it in getMousePressAction()
    // And currently you also have to do special handling in GuiTypes::getModifierName
    // Best for now to keep the GLFW_KEY number to above 20 to avoid conflicts with actual key mods
    mMousePressMap[KeyModPair(GLFW_MOUSE_BUTTON_LEFT, GLFW_KEY_X)]         = ACTION_EXPOSURE_ADJUST;
    mMousePressMap[KeyModPair(GLFW_MOUSE_BUTTON_LEFT, GLFW_KEY_Y)]         = ACTION_GAMMA_ADJUST;
}

Action 
KeyboardBindings::getKeyPressAction(const GLFWwindow* window, const int key, const int mod) const
{
    // If the key/mod combination exists in the map, return the action
    const auto& it = mKeyPressMap.find(KeyModPair(key, mod));
    if (it != mKeyPressMap.end()) {
        return it->second;
    }
    return ACTION_NONE;
}

Action 
KeyboardBindings::getMousePressAction(GLFWwindow* window, const int button, const int mod) const
{
    // First, look for any secondary keys (i.e. a regular key that's acting as a modifier)
    // We need to do this first to avoid conflicts with actual key mods
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
        const auto& it = mMousePressMap.find(KeyModPair(button, GLFW_KEY_X));
        if (it != mMousePressMap.end()) {
            return it->second;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {
        const auto& it = mMousePressMap.find(KeyModPair(button, GLFW_KEY_Y));
        if (it != mMousePressMap.end()) {
            return it->second;
        }
    }
    // If the button/mod combination exists in the map, return the action
    const auto& it = mMousePressMap.find(KeyModPair(button, mod));
    if (it != mMousePressMap.end()) {
        return it->second;
    }
    return ACTION_NONE;
}

// ---------------------------------------- MayaKeyboardBindings Class --------------------------------------------- ///
MayaKeyboardBindings::MayaKeyboardBindings()
{
    // Override any key mappings for Maya keyboard mode
    mKeyPressMap[KeyModPair(GLFW_KEY_HOME, GLFW_MOD_ALT)] = ACTION_CAM_RESET;
    // Add more key mappings here as needed

    // Image 2D Nav mouse actions
    // NOTE: If you use a GLFW_KEY as a modifier, you need to check for it in getMousePressAction()
    mMousePressMap[KeyModPair(GLFW_MOUSE_BUTTON_MIDDLE, GLFW_KEY_BACKSLASH)] = ACTION_IMAGE2D_PAN;
    mMousePressMap[KeyModPair(GLFW_MOUSE_BUTTON_RIGHT, GLFW_KEY_BACKSLASH)]  = ACTION_IMAGE2D_ZOOM;
    // Add more mouse mappings here as needed
}

// Need to override default function to handle secondary key modifiers
Action 
MayaKeyboardBindings::getMousePressAction(GLFWwindow* window, const int button, const int mod) const
{
    // First, check for any secondary keys (i.e. a regular key that's acting as a modifier)
    // We need to do this first to avoid conflicts with actual key mods
    if (glfwGetKey(window, GLFW_KEY_BACKSLASH) == GLFW_PRESS) {
        const auto& it = mMousePressMap.find(KeyModPair(button, GLFW_KEY_BACKSLASH));
        if (it != mMousePressMap.end()) {
            return it->second;
        }
    }
    // If the button/mod combination does not exist in the map, check for secondary keys
    return KeyboardBindings::getMousePressAction(window, button, mod);
}

/// -------------------------------------- HoudiniKeyboardBindings Class ------------------------------------------- ///
HoudiniKeyboardBindings::HoudiniKeyboardBindings()
{
    // Override any key mappings for Houdini keyboard mode
    mKeyPressMap[KeyModPair(GLFW_KEY_H, GLFW_KEY_SPACE)]                    = ACTION_CAM_RESET;
    mKeyPressMap[KeyModPair(GLFW_KEY_H, MOD_NONE)]                          = ACTION_CAM_RECENTER;
    // Add more key mappings here as needed

    // Override mouse mappings for Houdini keyboard mode
    mMousePressMap[KeyModPair(GLFW_MOUSE_BUTTON_LEFT, GLFW_KEY_SPACE)]      = ACTION_CAM_ORBIT;
    mMousePressMap[KeyModPair(GLFW_MOUSE_BUTTON_MIDDLE, GLFW_KEY_SPACE)]    = ACTION_CAM_PAN;
    mMousePressMap[KeyModPair(GLFW_MOUSE_BUTTON_RIGHT, GLFW_KEY_SPACE)]     = ACTION_CAM_DOLLY;
    // Add more mouse mappings here as needed
}

/// ------------------------------------------ Keyboard Class ------------------------------------------------------ ///
Keyboard::Keyboard() : 
    mBindings(std::make_unique<KeyboardBindings>()) 
{}

// -------------------------- Key Binding Lookup ---------------------------- //

bool 
Keyboard::isCameraAction(const Action& action) const
{
    /// NOTE: ACTION_CAM_TOGGLE_ACTIVE_TYPE is not included here, as it's handled by the 
    /// viewport, not the camera itself.
    return (action == ACTION_CAM_FORWARD ||
            action == ACTION_CAM_BACKWARD ||
            action == ACTION_CAM_LEFT ||
            action == ACTION_CAM_RIGHT ||
            action == ACTION_CAM_UP ||
            action == ACTION_CAM_DOWN ||
            action == ACTION_CAM_SLOW_DOWN ||
            action == ACTION_CAM_SPEED_UP ||
            action == ACTION_CAM_RESET ||
            action == ACTION_CAM_RECENTER ||
            action == ACTION_CAM_PRINT_MATRICES ||
            action == ACTION_CAM_SET_UP_VECTOR ||
            action == ACTION_CAM_ORBIT ||
            action == ACTION_CAM_DOLLY ||
            action == ACTION_CAM_PAN ||
            action == ACTION_CAM_ROLL);
}

Action 
Keyboard::getKeyPressAction(const GLFWwindow* glfwWindow, const int key, const int mod) const
{
    return mBindings->getKeyPressAction(glfwWindow, key, mod);
}

Action 
Keyboard::getMousePressAction(GLFWwindow* glfwWindow, const int button, const int mod) const
{
    return mBindings->getMousePressAction(glfwWindow, button, mod);
}

Action 
Keyboard::getScrollAction() const
{
    return mBindings->getScrollAction();
}

void 
Keyboard::printKeyBindings() const
{
    std::cout << "\n ------- Key Bindings -------\n";
    const auto& keyMap = mBindings->getKeyPressMap();
    std::vector<std::pair<KeyModPair, Action>> entries(keyMap.begin(), keyMap.end());
    for (auto& entry : mBindings->getMousePressMap()) {
        entries.push_back(entry);
    }
    std::sort(entries.begin(), entries.end(), [](const std::pair<KeyModPair, Action>& a, 
        const std::pair<KeyModPair, Action>& b) {
            return a.second < b.second;
        });
        
        for (const std::pair<KeyModPair, Action>& entry : entries) {
            KeyModPair pair = entry.first;
            Action action = entry.second;
            
            const int key = pair.first;
            const int mod = pair.second;
            
            std::string actionStr = getActionName(action);
            std::string keyStr = getKeyName(key);
            std::string modStr = getModifierName(mod);
            
            std::cout << actionStr << ": " << modStr << keyStr << std::endl;
        }
    std::cout << " ----------------------------\n";
}
} // end namespace moonray_gui_v2
