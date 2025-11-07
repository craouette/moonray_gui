// Copyright 2025 DreamWorks Animation LLC
// SPDX-License-Identifier: Apache-2.0

#include "SnapshotManager.h"

#include <moonray/rendering/rndr/rndr.h>

#include <filesystem>
#include <fstream>

using namespace moonray;
using namespace scene_rdl2::logging;

namespace {

bool validateSnapPath(const std::string& snapPath)
{
    auto warn = [snapPath] (const std::string& msg) {
        Logger::warn(msg + " '" + snapPath + "'. Snapshotting has been disabled.\n");
        return false;
    };
    
    // Check if path is a directory and exists
    if (!std::filesystem::is_directory(snapPath) || !std::filesystem::exists(snapPath)) {
        return warn("Invalid snapshot path");
    } else {
        // Create a temporary file in the given directory to test write permissions
        std::filesystem::path testFilePath = std::filesystem::path(snapPath) / ".__test_write_permission__";
        std::ofstream testFile(testFilePath.string());
        if (!testFile.is_open()) {
            return warn("Invalid permissions for snapshot path");
        }
        testFile.close();
    }
    return true;
}

} // end anonymous namespace

namespace moonray_gui_v2 {

SnapshotManager::SnapshotManager(const std::string& snapPath) 
    : mSnapPath(snapPath)
{
    // If path is empty, use current directory
    if (snapPath.empty()) {
        mSnapPath = std::filesystem::current_path().string();
    }

    // Ensure the path is valid
    mValid = validateSnapPath(mSnapPath);
    if (!mValid) { return; }

    // Append directory separator
    mSnapPath = (std::filesystem::path(mSnapPath) / "").string();

    // get index for snapshot
    while (true) {
        std::stringstream ss;
        ss << mSnapPath << "snapshot.";
        ss << std::setw(4) << std::setfill('0') << mSnapIdx << ".exr";
        if (!std::filesystem::exists(ss.str())) {
            break;
        }
        mSnapIdx++;
    }
}

void
SnapshotManager::takeASnapshot(const moonray::rndr::RenderContext* renderContext)
{
    // If snapshot path was not valid, we can't take any new snapshots
    if (!mValid) { 
        Logger::error("Failed to take snapshot. Snapshotting is disabled.");
        return; 
    }
    // Ensure the render context exists and can be displayed.
    // Key bindings can call this function before everything is fully ready.
    if (!renderContext || !renderContext->isFrameReadyForDisplay()) return;

    std::stringstream ss;
    ss << "snapshot." << std::setw(4) << std::setfill('0') << mSnapIdx << ".exr";
    const std::string outputFilename = ss.str();
    // Write the image
    scene_rdl2::fb_util::RenderBuffer outputBuffer;
    const scene_rdl2::rdl2::SceneObject *metadata = renderContext->getSceneContext().getSceneVariables()
                                                                  .getExrHeaderAttributes();
    const scene_rdl2::math::HalfOpenViewport aperture = renderContext->getRezedApertureWindow();
    const scene_rdl2::math::HalfOpenViewport region = renderContext->getRezedRegionWindow();
    renderContext->snapshotRenderBuffer(&outputBuffer, /*untile*/ true, /*parallel*/ true, /*usePrimaryAov*/ true);
    try {
        moonray::rndr::writePixelBuffer(outputBuffer, mSnapPath + outputFilename, metadata, aperture, region);
        std::cout << "Snapshot " << outputFilename << " taken and saved to " << mSnapPath << std::endl;
        mSnapIdx++;
    } catch (...) {
        Logger::error("Failed to write out snapshot");
    }
}

} // end namespace moonray_gui_v2
