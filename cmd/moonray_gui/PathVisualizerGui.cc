// Copyright 2025 DreamWorks Animation LLC
// SPDX-License-Identifier: Apache-2.0

#include "ColorPicker.h"
#include "PathVisualizerGui.h"
#include "RenderViewport.h"

#include <QCheckBox>
#include <QColorDialog>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QSpinBox>

namespace moonray_gui {

PathVisualizerGui::PathVisualizerGui(QWidget* parent) :
    QWidget(parent), mRenderViewport(parent), mCurrentRow(0)
{
    // Set up the layout
    QGridLayout* layout = new QGridLayout;
    layout->setContentsMargins(5, 5, 5, 5);
    
    // Set up the title
    QLabel* title = new QLabel("Path Visualizer", this);
    title->setStyleSheet("QLabel { font-size: 25px; border-bottom: 1px solid white; }");
    layout->addWidget(title, mCurrentRow++, 0, 1, 2);

    // Setup the rest of the path visualizer GUI
    setupRecordBtn(layout);
    setupPixelUI(layout);
    setupDepthUI(layout);
    setupVisibilityUI(layout);
    setupStyleUI(layout);

    // Set up style for the whole UI
    this->setStyleSheet("QWidget { color: white; padding: 4px; margin: 1px; }");

    // Set the layout for this widget
    setLayout(layout);
}

PathVisualizerGui::~PathVisualizerGui() {}

void PathVisualizerGui::setupRecordBtn(QGridLayout* layout)
{
    QPushButton* recordBtn = new QPushButton("Record Ray Data", this);
    connect(recordBtn, SIGNAL(clicked()), mRenderViewport, SLOT(slot_attachPathVisualizer()));

    recordBtn->setStyleSheet("QPushButton { background-color: #b8d3ff; color: black; border-radius: 5px; margin: 10px; } \
                              QPushButton:hover { background-color: #98b9ed; }");
    recordBtn->setCursor(Qt::PointingHandCursor);

    QCheckBox* progressiveDrawToggle = new QCheckBox("Draw rays progressively", this);
    progressiveDrawToggle->setCheckState(Qt::Checked);
    /// NOTE: stateChanged will be deprecated starting with Qt v6.9
    connect(progressiveDrawToggle, SIGNAL(stateChanged(int)), mRenderViewport, SLOT(slot_processProgressiveDraw(int)));

    layout->addWidget(recordBtn, mCurrentRow++, 0, 1, 1);
    layout->addWidget(progressiveDrawToggle, mCurrentRow++, 0, 1, 1);
}

void PathVisualizerGui::setupPixelUI(QGridLayout* layout)
{
    QLabel* pixelTitle = new QLabel("Pixel", this);
    QSpinBox* pixelX = new QSpinBox(this);
    QSpinBox* pixelY = new QSpinBox(this);
    pixelX->setRange(0, 10000);
    pixelY->setRange(0, 10000);
    pixelX->setSingleStep(1);
    pixelY->setSingleStep(1);
    pixelX->setSuffix(" px");
    pixelY->setSuffix(" py");
    pixelX->setValue(0);
    pixelY->setValue(0);
    // only process the input once the user is done typing and presses enter
    pixelX->setKeyboardTracking(false);
    pixelY->setKeyboardTracking(false);    
    connect(pixelX, SIGNAL(valueChanged(int)), mRenderViewport, SLOT(slot_processPixelXValue(int)));
    connect(pixelY, SIGNAL(valueChanged(int)), mRenderViewport, SLOT(slot_processPixelYValue(int)));

    /// TODO: create style sheet
    pixelTitle->setStyleSheet("QLabel { background-color: #383838; font: bold 15px; }");

    layout->addWidget(pixelTitle, mCurrentRow++, 0, 1, 2);
    layout->addWidget(pixelX, mCurrentRow, 0, 1, 1);
    layout->addWidget(pixelY, mCurrentRow++, 1, 1, 1);
}

void PathVisualizerGui::setupDepthUI(QGridLayout* layout)
{
    QLabel* depthTitle = new QLabel("Max Depth", this);
    QSpinBox* maxDepth = new QSpinBox(this);
    maxDepth->setValue(1);
    maxDepth->setRange(0, 20);
    maxDepth->setKeyboardTracking(false);
    connect(maxDepth, SIGNAL(valueChanged(int)), mRenderViewport, SLOT(slot_processMaxDepth(int)));

    depthTitle->setStyleSheet("QLabel { background-color: #383838; font: bold 15px; }");

    layout->addWidget(depthTitle, mCurrentRow++, 0, 1, 2);
    layout->addWidget(maxDepth, mCurrentRow++, 0, 1, 1);
}

void PathVisualizerGui::setupVisibilityUI(QGridLayout* layout)
{
    QLabel* visibilityTitle = new QLabel("Visibility Toggles", this);
    QCheckBox* occlusionRaysOn = new QCheckBox("Occlusion rays", this);
    QCheckBox* specularRaysOn = new QCheckBox("Specular rays", this);
    QCheckBox* diffuseRaysOn = new QCheckBox("Diffuse rays", this);
    QCheckBox* bsdfSamplesOn = new QCheckBox("Bsdf samples", this);
    QCheckBox* lightSamplesOn = new QCheckBox("Light samples", this);
    occlusionRaysOn->setCheckState(Qt::Checked);
    specularRaysOn->setCheckState(Qt::Checked);
    diffuseRaysOn->setCheckState(Qt::Checked);
    bsdfSamplesOn->setCheckState(Qt::Checked);
    lightSamplesOn->setCheckState(Qt::Checked);
    /// NOTE: stateChanged will be deprecated starting with Qt v6.9
    connect(occlusionRaysOn, SIGNAL(stateChanged(int)), mRenderViewport, SLOT(slot_processOcclusionRayFlag(int)));
    connect(specularRaysOn,  SIGNAL(stateChanged(int)), mRenderViewport, SLOT(slot_processSpecularRayFlag(int)));
    connect(diffuseRaysOn,   SIGNAL(stateChanged(int)), mRenderViewport, SLOT(slot_processDiffuseRayFlag(int)));
    connect(bsdfSamplesOn,   SIGNAL(stateChanged(int)), mRenderViewport, SLOT(slot_processBsdfSampleFlag(int)));
    connect(lightSamplesOn,  SIGNAL(stateChanged(int)), mRenderViewport, SLOT(slot_processLightSampleFlag(int)));

    visibilityTitle->setStyleSheet("QLabel { background-color: #383838; font: bold 15px; }");
    
    layout->addWidget(visibilityTitle, mCurrentRow++, 0, 1, 2);
    layout->addWidget(occlusionRaysOn, mCurrentRow++, 0, 1, 1);
    layout->addWidget(specularRaysOn,  mCurrentRow++, 0, 1, 1);
    layout->addWidget(diffuseRaysOn,   mCurrentRow++, 0, 1, 1);
    layout->addWidget(bsdfSamplesOn,   mCurrentRow++, 0, 1, 1);
    layout->addWidget(lightSamplesOn,  mCurrentRow++, 0, 1, 1);
}

void PathVisualizerGui::setupStyleUI(QGridLayout* layout)
{
    QLabel* styleTitle = new QLabel("Style Options", this);
    styleTitle->setStyleSheet("QLabel { background-color: #383838; font: bold 15px; }");

    QLabel* lineWidthLabel = new QLabel("Line Width: ", this);
    QSlider* lineWidthSlider = new QSlider(this);
    lineWidthSlider->setTickInterval(1);
    lineWidthSlider->setValue(2);
    lineWidthSlider->setMinimum(1);
    lineWidthSlider->setMaximum(8);
    lineWidthSlider->setStyleSheet("QSlider::handle { background-color: white; border-radius: 5px; }");
    lineWidthSlider->setOrientation(Qt::Horizontal);
    connect(lineWidthSlider, SIGNAL(sliderMoved(int)), mRenderViewport, SLOT(slot_setLineWidth(int)));

    mDiffuseRayColorPicker = new ColorPicker(this, "Diffuse Ray Color: ", QColor(255, 0, 255));
    mSpecularRayColorPicker = new ColorPicker(this, "Specular Ray Color: ", QColor(0, 255, 255));
    mBsdfSampleColorPicker = new ColorPicker(this, "Bsdf Sample Ray Color: ", QColor(255, 102, 0));
    mLightSampleColorPicker = new ColorPicker(this, "Light Sample Ray Color: ", QColor(255, 255, 0));
    mCameraRayColorPicker = new ColorPicker(this, "Camera Ray Color: ", QColor(0, 0, 255));

    connect(mDiffuseRayColorPicker, SIGNAL(sig_colorChanged(float, float, float)), 
            mRenderViewport, SLOT(slot_setDiffuseRayColor(float, float, float)));
    connect(mSpecularRayColorPicker, SIGNAL(sig_colorChanged(float, float, float)), 
            mRenderViewport, SLOT(slot_setSpecularRayColor(float, float, float)));
    connect(mBsdfSampleColorPicker, SIGNAL(sig_colorChanged(float, float, float)), 
            mRenderViewport, SLOT(slot_setBsdfSampleColor(float, float, float)));
    connect(mLightSampleColorPicker, SIGNAL(sig_colorChanged(float, float, float)), 
            mRenderViewport, SLOT(slot_setLightSampleColor(float, float, float)));
    connect(mCameraRayColorPicker, SIGNAL(sig_colorChanged(float, float, float)), 
            mRenderViewport, SLOT(slot_setCameraRayColor(float, float, float)));
    
    layout->addWidget(styleTitle, mCurrentRow++, 0, 1, 2);
    layout->addWidget(lineWidthLabel, mCurrentRow++, 0, 1, 2);
    layout->addWidget(lineWidthSlider, mCurrentRow++, 0, 1, 2);
    layout->addWidget(mDiffuseRayColorPicker, mCurrentRow++, 0, 1, 1);
    layout->addWidget(mSpecularRayColorPicker, mCurrentRow++, 0, 1, 1);
    layout->addWidget(mBsdfSampleColorPicker, mCurrentRow++, 0, 1, 1);
    layout->addWidget(mLightSampleColorPicker, mCurrentRow++, 0, 1, 1);
    layout->addWidget(mCameraRayColorPicker, mCurrentRow++, 0, 1, 1);
}

}
