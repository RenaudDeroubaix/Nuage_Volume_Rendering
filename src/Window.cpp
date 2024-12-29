#include "../header/Window.h"

#include <QFileDialog>
#include <QLayout>
#include <QMenu>
#include <QStatusBar>
#include <QGroupBox>

Window::Window()
{
    if (this->objectName().isEmpty())
        this->setObjectName("mainWindow");
    this->resize(1280, 720);

    viewer = new TextureViewer(this);

    QWidget * textureWidget = new QWidget(this);
    QGridLayout * gridLayout = new QGridLayout(textureWidget);

    gridLayout->addWidget(viewer, 0, 1, 1, 1);

    //QAction * actionLoad3Dimage = new QAction("Load 3D image", this);
    //QAction * recompileShaders = new QAction("Recompile shaders", this);


    QMenu * menuFile = new QMenu("File", this);

    //menuFile->addAction(actionLoad3Dimage);

    //connect(recompileShaders, &QAction::triggered, viewer, &TextureViewer::recompileShaders);

    QGroupBox * viewerGroupBox = new QGroupBox ("Clouds: Volume Rendering", this);
    QHBoxLayout * viewerLayout = new QHBoxLayout (viewerGroupBox);
    viewerLayout->addWidget (textureWidget);

    madDockWidget = new TextureDockWidget(this);

    this->addDockWidget(Qt::RightDockWidgetArea, madDockWidget);

    //TODO : Connect madDockWidget signals to viewer slots

    connect(madDockWidget, &TextureDockWidget::xBBminValueChanged, viewer, &TextureViewer::setxBBmin);
    connect(madDockWidget, &TextureDockWidget::yBBminValueChanged, viewer, &TextureViewer::setyBBmin);
    connect(madDockWidget, &TextureDockWidget::zBBminValueChanged, viewer, &TextureViewer::setzBBmin);

    connect(madDockWidget, &TextureDockWidget::xBBmaxValueChanged, viewer, &TextureViewer::setxBBmax);
    connect(madDockWidget, &TextureDockWidget::yBBmaxValueChanged, viewer, &TextureViewer::setyBBmax);
    connect(madDockWidget, &TextureDockWidget::zBBmaxValueChanged, viewer, &TextureViewer::setzBBmax);

    connect(madDockWidget, &TextureDockWidget::redNuageValueChanged, viewer, &TextureViewer::setRedNuage);
    connect(madDockWidget, &TextureDockWidget::blueNuageValueChanged, viewer, &TextureViewer::setBlueNuage);
    connect(madDockWidget, &TextureDockWidget::greenNuageValueChanged, viewer, &TextureViewer::setGreenNuage);


    connect(madDockWidget, &TextureDockWidget::absorptionValueChanged, viewer, &TextureViewer::setAbsorptionNuage);

    connect(madDockWidget, &TextureDockWidget::onNuageSliderChanged, viewer, &TextureViewer::onNuageSliderChanged);
    connect(madDockWidget, &TextureDockWidget::onNuageSpinBoxChanged, viewer, &TextureViewer::onNuageSpinBoxChanged);

    connect(madDockWidget, &TextureDockWidget::onLightSliderChanged, viewer, &TextureViewer::onLightSliderChanged);
    connect(madDockWidget, &TextureDockWidget::onLightSpinBoxChanged, viewer, &TextureViewer::onLightSpinBoxChanged);

    connect(madDockWidget, &TextureDockWidget::xResolutionBruitValueChanged, viewer, &TextureViewer::setResolutionBruitX);
    connect(madDockWidget, &TextureDockWidget::yResolutionBruitValueChanged, viewer, &TextureViewer::setResolutionBruitY);
    connect(madDockWidget, &TextureDockWidget::zResolutionBruitValueChanged, viewer, &TextureViewer::setResolutionBruitZ);

    connect(madDockWidget, &TextureDockWidget::rFreqBruitValueChanged, viewer, &TextureViewer::setFreqBruitR);
    connect(madDockWidget, &TextureDockWidget::gFreqBruitValueChanged, viewer, &TextureViewer::setFreqBruitG);
    connect(madDockWidget, &TextureDockWidget::bFreqBruitValueChanged, viewer, &TextureViewer::setFreqBruitB);
    connect(madDockWidget, &TextureDockWidget::aFreqBruitValueChanged, viewer, &TextureViewer::setFreqBruitA);

    connect(madDockWidget, &TextureDockWidget::rFacteurBruitValueChanged, viewer, &TextureViewer::setFacteurBruitR);
    connect(madDockWidget, &TextureDockWidget::gFacteurBruitValueChanged, viewer, &TextureViewer::setFacteurBruitG);
    connect(madDockWidget, &TextureDockWidget::bFacteurBruitValueChanged, viewer, &TextureViewer::setFacteurBruitB);
    connect(madDockWidget, &TextureDockWidget::aFacteurBruitValueChanged, viewer, &TextureViewer::setFacteurBruitA);

    connect(madDockWidget, &TextureDockWidget::rayonSoleilSliderChanged, viewer, &TextureViewer::setRayonSoleil);

    connect(madDockWidget, &TextureDockWidget::absorptionLightValueChanged, viewer, &TextureViewer::setAbsorptionLight);

    connect(madDockWidget, &TextureDockWidget::xResolutionBruitValueChanged, viewer, &TextureViewer::setResolutionBruitX);
    connect(madDockWidget, &TextureDockWidget::yResolutionBruitValueChanged, viewer, &TextureViewer::setResolutionBruitY);
    connect(madDockWidget, &TextureDockWidget::rFreqBruitCurlValueChanged, viewer, &TextureViewer::setFreqBruitCurlR);
    connect(madDockWidget, &TextureDockWidget::gFreqBruitCurlValueChanged, viewer, &TextureViewer::setFreqBruitCurlG);
    connect(madDockWidget, &TextureDockWidget::bFreqBruitCurlValueChanged, viewer, &TextureViewer::setFreqBruitCurlB);

    connect(madDockWidget, &TextureDockWidget::vitesseSliderChanged, viewer, &TextureViewer::setVitesse);


    connect(madDockWidget, &TextureDockWidget::setlightposXValueChanged, viewer, &TextureViewer::setXlightpos);
    connect(madDockWidget, &TextureDockWidget::setlightposYValueChanged, viewer, &TextureViewer::setYlightpos);
    connect(madDockWidget, &TextureDockWidget::setlightposZValueChanged, viewer, &TextureViewer::setZlightpos);

    connect(madDockWidget, &TextureDockWidget::setlightcolRValueChanged, viewer, &TextureViewer::setRlightcol);
    connect(madDockWidget, &TextureDockWidget::setlightcolGValueChanged, viewer, &TextureViewer::setGlightcol);
    connect(madDockWidget, &TextureDockWidget::setlightcolBValueChanged, viewer, &TextureViewer::setBlightcol);

    connect(madDockWidget, &TextureDockWidget::setcamerapos, viewer, &TextureViewer::setcamerapos);
    connect(madDockWidget, &TextureDockWidget::setIsLightUtime, viewer, &TextureViewer::setIsLightUTime);
    connect(madDockWidget, &TextureDockWidget::setboolOnlyCloud, viewer, &TextureViewer::setboolOnlyCloud);
    connect(madDockWidget, &TextureDockWidget::setboolCameraOrbital, viewer, &TextureViewer::setboolCameraOrbital);

    connect(madDockWidget, &TextureDockWidget::formeSelectionChanged,viewer, &TextureViewer::setFormeNuage);
    connect(madDockWidget, &TextureDockWidget::attenuationSliderChanged,viewer, &TextureViewer::setAttenuationBord);
    connect(madDockWidget, &TextureDockWidget::rSphereSliderChanged,viewer, &TextureViewer::setRayonSphere);
    connect(madDockWidget, &TextureDockWidget::rITorusSliderChanged,viewer, &TextureViewer::setRayonInternTorus);
    connect(madDockWidget, &TextureDockWidget::rETorusSliderChanged,viewer, &TextureViewer::setRayonExternTorus);
    connect(madDockWidget, &TextureDockWidget::lCapsuleSliderChanged,viewer, &TextureViewer::setLongueurCapsule);
    connect(madDockWidget, &TextureDockWidget::rCapsuleSliderChanged,viewer, &TextureViewer::setRayonCapsule);
    connect(madDockWidget, &TextureDockWidget::tBarreCroixSliderChanged,viewer, &TextureViewer::setTailleBarre);

    this->setCentralWidget(viewerGroupBox);

    QMenuBar * menubar = new QMenuBar(this);

    menubar->addAction(menuFile->menuAction());
    //menubar->addAction(recompileShaders);

    this->setMenuBar(menubar);

    statusbar = new QStatusBar(this);

    this->setStatusBar(statusbar);

    this->setWindowTitle("PROJECT: Nuage");


}

