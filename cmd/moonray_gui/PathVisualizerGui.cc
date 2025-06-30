// Copyright 2025 DreamWorks Animation LLC
// SPDX-License-Identifier: Apache-2.0

#include "ColorPicker.h"
#include "PathVisualizerGui.h"
#include "RenderViewport.h"

#include <QCheckBox>
#include <QColorDialog>
#include <QGridLayout>
#include <QFile>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QSpinBox>
#include <QTextStream>

namespace moonray_gui {

PathVisualizerGui::PathVisualizerGui(QWidget* parent) :
    QWidget(parent), mRenderViewport(static_cast<RenderViewport*>(parent)), mCurrentRow(0)
{
    // Set up the layout
    QGridLayout* layout = new QGridLayout;
    layout->setContentsMargins(5, 5, 5, 5);
    
    // Set up the title
    QLabel* title = new QLabel("Path Visualizer", this);
    title->setStyleSheet("QLabel { font-size: 25px; border-bottom: 1px solid white; margin-bottom:5px; }");
    layout->addWidget(title, mCurrentRow++, 0, 1, 2);

    // Setup the rest of the path visualizer GUI
    setupPixelUI(layout);
    setupSamplingUI(layout);
    setupDepthUI(layout);
    setupVisibilityUI(layout);
    setupRecordBtn(layout);
    setupStyleUI(layout);

    // Setup stylesheet
    QFile styleFile(":/PathVisualizerGui.qss");

    if (styleFile.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream ts(&styleFile);
        QString styleSheet = ts.readAll();
        this->setStyleSheet(styleSheet);
    }

    // Set the layout for this widget
    setLayout(layout);
}

PathVisualizerGui::~PathVisualizerGui() {}

void PathVisualizerGui::setupRecordBtn(QGridLayout* layout)
{
    QPushButton* recordBtn = new QPushButton("Draw", this);
    recordBtn->setProperty("class", "button");
    connect(recordBtn, SIGNAL(clicked()), mRenderViewport, SLOT(slot_attachPathVisualizer()));

    recordBtn->setCursor(Qt::PointingHandCursor);

    layout->addWidget(recordBtn, mCurrentRow++, 0, 1, 3);
}

void PathVisualizerGui::setupPixelUI(QGridLayout* layout)
{
    QLabel* pixelTitle = new QLabel("Pixel: ", this);
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

    layout->addWidget(pixelTitle, mCurrentRow, 0, 1, 1);
    layout->addWidget(pixelX, mCurrentRow, 1, 1, 1);
    layout->addWidget(pixelY, mCurrentRow++, 2, 1, 1);
}

void PathVisualizerGui::setupSamplingUI(QGridLayout* layout)
{
    QLabel* samplingTitle = new QLabel("Sampling Settings", this);
    samplingTitle->setProperty("class", "header");

    QCheckBox* useSceneSamples = new QCheckBox("Use Scene Sampling Settings", this);
    useSceneSamples->setCheckState(Qt::Checked);

    QLabel* pixelSamplesTitle = new QLabel("Pixel Samples:", this);
    QLabel* lightSamplesTitle = new QLabel("Light Samples:", this);
    QLabel* bsdfSamplesTitle = new QLabel("Bsdf Samples:", this);
    mPixelSamples = new QSpinBox(this);
    mLightSamples = new QSpinBox(this);
    mBsdfSamples = new QSpinBox(this);
    mPixelSamples->setRange(0, 32);
    mLightSamples->setRange(0, 32);
    mBsdfSamples->setRange(0, 32);
    mPixelSamples->setValue(2);
    mLightSamples->setValue(1);
    mBsdfSamples->setValue(1);
    mPixelSamples->setEnabled(false);
    mLightSamples->setEnabled(false);
    mBsdfSamples->setEnabled(false);

    connect(useSceneSamples, SIGNAL(stateChanged(int)), mRenderViewport, SLOT(slot_processUseSceneSamples(int)));
    connect(useSceneSamples, SIGNAL(stateChanged(int)), this, SLOT(slot_toggleSamplingSettings(int)));
    connect(mPixelSamples, SIGNAL(valueChanged(int)), mRenderViewport, SLOT(slot_processPixelSamples(int)));
    connect(mLightSamples, SIGNAL(valueChanged(int)), mRenderViewport, SLOT(slot_processLightSamples(int)));
    connect(mBsdfSamples, SIGNAL(valueChanged(int)), mRenderViewport, SLOT(slot_processBsdfSamples(int)));

    layout->addWidget(samplingTitle, mCurrentRow++, 0, 1, 3);
    layout->addWidget(useSceneSamples, mCurrentRow++, 0, 1, 3);
    layout->addWidget(pixelSamplesTitle, mCurrentRow, 0, 1, 2);
    layout->addWidget(mPixelSamples, mCurrentRow++, 2, 1, 1);
    layout->addWidget(lightSamplesTitle, mCurrentRow, 0, 1, 2);
    layout->addWidget(mLightSamples, mCurrentRow++, 2, 1, 1);
    layout->addWidget(bsdfSamplesTitle, mCurrentRow, 0, 1, 2);
    layout->addWidget(mBsdfSamples, mCurrentRow++, 2, 1, 1);
}

void PathVisualizerGui::setupDepthUI(QGridLayout* layout)
{
    QLabel* depthTitle = new QLabel("Max Depth", this);
    depthTitle->setProperty("class", "header");

    QSpinBox* maxDepth = new QSpinBox(this);
    maxDepth->setValue(1);
    maxDepth->setRange(0, 50);
    maxDepth->setKeyboardTracking(false);
    connect(maxDepth, SIGNAL(valueChanged(int)), mRenderViewport, SLOT(slot_processMaxDepth(int)));

    layout->addWidget(depthTitle, mCurrentRow++, 0, 1, 3);
    layout->addWidget(maxDepth,   mCurrentRow++, 0, 1, 1);
}

void PathVisualizerGui::setupVisibilityUI(QGridLayout* layout)
{
    QLabel* visibilityTitle = new QLabel("Visibility Toggles", this);
    visibilityTitle->setProperty("class", "header");

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
    
    layout->addWidget(visibilityTitle, mCurrentRow++, 0, 1, 3);
    layout->addWidget(occlusionRaysOn, mCurrentRow++, 0, 1, 3);
    layout->addWidget(specularRaysOn,  mCurrentRow++, 0, 1, 3);
    layout->addWidget(diffuseRaysOn,   mCurrentRow++, 0, 1, 3);
    layout->addWidget(bsdfSamplesOn,   mCurrentRow++, 0, 1, 3);
    layout->addWidget(lightSamplesOn,  mCurrentRow++, 0, 1, 3);
}

void PathVisualizerGui::setupStyleUI(QGridLayout* layout)
{
    QLabel* styleTitle = new QLabel("Style Options", this);
    styleTitle->setProperty("class", "header");

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
    
    layout->addWidget(styleTitle, mCurrentRow++, 0, 1, 3);
    layout->addWidget(lineWidthLabel, mCurrentRow++, 0, 1, 3);
    layout->addWidget(lineWidthSlider, mCurrentRow++, 0, 1, 3);
    layout->addWidget(mDiffuseRayColorPicker, mCurrentRow++, 0, 1, 3);
    layout->addWidget(mSpecularRayColorPicker, mCurrentRow++, 0, 1, 3);
    layout->addWidget(mBsdfSampleColorPicker, mCurrentRow++, 0, 1, 3);
    layout->addWidget(mLightSampleColorPicker, mCurrentRow++, 0, 1, 3);
    layout->addWidget(mCameraRayColorPicker, mCurrentRow++, 0, 1, 3);
}

void PathVisualizerGui::slot_toggleSamplingSettings(int useSceneSamples)
{
    bool enableSamplingSettings = !useSceneSamples;

    mPixelSamples->setEnabled(enableSamplingSettings);
    mLightSamples->setEnabled(enableSamplingSettings);
    mBsdfSamples->setEnabled(enableSamplingSettings);
}

}
