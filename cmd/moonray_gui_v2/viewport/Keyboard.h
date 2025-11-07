// Copyright 2025 DreamWorks Animation LLC
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "../GuiTypes.h"

#include <GLFW/glfw3.h>

namespace moonray_gui_v2 {

#define KeyModPair std::pair<int, int>

// Custom hash function for KeyModPair
// (Needed for our key bindings, which map a KeyModPair to an Action)
struct KeyModPairHash {
    std::size_t operator()(const KeyModPair& p) const {
        return std::hash<int>()(p.first) ^ (std::hash<int>()(p.second) << 1);
    }
};

// Global function to print hotkey bindings for debugging
void printHotkeys();

/// ----------------------------------------- KeyboardBindings Class ----------------------------------------------- ///
// KeyboardBindings class that maps key/modifier combinations to Actions
// This class contains the default key bindings for the application.
class KeyboardBindings
{
public:
    KeyboardBindings();
    virtual ~KeyboardBindings() {};

    // Returns the action associated with the given key/mod combination
    virtual Action getKeyPressAction(const GLFWwindow* window, const int key, const int mod) const;
    // Returns the action associated with the given mouse button/mod combination
    virtual Action getMousePressAction(GLFWwindow* window, const int button, const int mod) const;

    Action getScrollAction() const { return mScrollAction; }
    const std::unordered_map<KeyModPair, Action, KeyModPairHash>& getKeyPressMap() const { return mKeyPressMap; }
    const std::unordered_map<KeyModPair, Action, KeyModPairHash>& getMousePressMap() const { return mMousePressMap; }

protected:
    Action mScrollAction = ACTION_IMAGE2D_ZOOM;
    std::unordered_map<KeyModPair, Action, KeyModPairHash> mKeyPressMap;
    std::unordered_map<KeyModPair, Action, KeyModPairHash> mMousePressMap;
};

/// --------------------------------- MayaKeyboardBindings Class --------------------------------------------------- ///
// MayaKeyboardBindings class that overrides default key bindings for Maya-style keyboard input
class MayaKeyboardBindings : public KeyboardBindings
{
public:
    MayaKeyboardBindings();
    Action getMousePressAction(GLFWwindow* window, int button, int mod) const override;

    ~MayaKeyboardBindings() override {}
};

/// ------------------------------- HoudiniKeyboardBindings Class -------------------------------------------------- ///
// HoudiniKeyboardBindings class that overrides default key bindings for Houdini-style keyboard input
class HoudiniKeyboardBindings : public KeyboardBindings
{
public:
    HoudiniKeyboardBindings();
    ~HoudiniKeyboardBindings() override {}
};

/// ----------------------------------------- Keyboard Class ------------------------------------------------------ ///
// Keyboard class that manages the current keyboard mode and provides action lookup
// This class allows switching between different keyboard modes (e.g., Maya, Houdini, Default).
// In the future, it will support overrides for specific key/mouse combinations.

class Keyboard {
public:
    Keyboard();
    ~Keyboard() {}

    // -------------------------- Key Binding Lookup ---------------------------- //
    // Is this an action handled by the camera?
    // If so, we handle these actions differently in the Window class
    bool isCameraAction(const Action& action) const;    

    // Gets the action for a given key/mod combination
    Action getKeyPressAction(const GLFWwindow* window, const int key, const int mod) const;

    // Gets the action for a given mouse button/mod combination
    Action getMousePressAction(GLFWwindow* window, const int button, const int mod) const;

    // Gets the action for scroll input
    Action getScrollAction() const;

    // -------------------------- Setters ---------------------------- //
    void setDefaultKeyboardMode() { mBindings.reset(new KeyboardBindings()); }
    void setMayaKeyboardMode()    { mBindings.reset(new MayaKeyboardBindings()); }
    void setHoudiniKeyboardMode() { mBindings.reset(new HoudiniKeyboardBindings()); }

    // -------------------------- Global Key Binding Operations ---------------------------- //

    // Print current key bindings and overrides to console for debugging
    void printKeyBindings() const;

private:
    std::unique_ptr<KeyboardBindings> mBindings;
};

} // end namespace moonray_gui_v2
