// Copyright 2025 DreamWorks Animation LLC
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <QWidget>

class QGridLayout;
class QPushButton;
class QSpinBox;

namespace moonray_gui {

class ColorPicker;
class RenderViewport;

class PathVisualizerGui : public QWidget
{
    Q_OBJECT

public:
    explicit PathVisualizerGui(QWidget* parent);

    ~PathVisualizerGui();

public slots:
    void slot_toggleSamplingSettings(int useSceneSamples);

private:

    void setupPixelUI(QGridLayout* layout);
    void setupSamplingUI(QGridLayout* layout);
    void setupDepthUI(QGridLayout* layout);
    void setupVisibilityUI(QGridLayout* layout);
    void setupRecordBtn(QGridLayout* layout);
    void setupStyleUI(QGridLayout* layout);

    RenderViewport* mRenderViewport;
    int mCurrentRow;

    QSpinBox* mPixelSamples;
    QSpinBox* mLightSamples;
    QSpinBox* mBsdfSamples;

    ColorPicker* mDiffuseRayColorPicker;
    ColorPicker* mSpecularRayColorPicker;
    ColorPicker* mBsdfSampleColorPicker;
    ColorPicker* mLightSampleColorPicker;
    ColorPicker* mCameraRayColorPicker;
};

} // namespace moonray_gui
