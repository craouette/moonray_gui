// Copyright 2025 DreamWorks Animation LLC
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <string>

namespace moonray { namespace rndr { class RenderContext; }}

namespace moonray_gui_v2 {

class SnapshotManager {

public:
    SnapshotManager(const std::string& snapPath);
    ~SnapshotManager() {};

    void takeASnapshot(const moonray::rndr::RenderContext* renderContext);

private:
    bool mValid {true};                             // whether the snapshot path given is valid
    int mSnapIdx {1};                               // index of the next snapshot to save
    std::string mSnapPath;                          // path to the user's snapshot directory
};

} // end namespace moonray_gui_v2
