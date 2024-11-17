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
    QAction * recompileShaders = new QAction("Recompile shaders", this);


    QMenu * menuFile = new QMenu("File", this);

    //menuFile->addAction(actionLoad3Dimage);



    //connect(actionLoad3Dimage, SIGNAL(triggered()), this, SLOT(open3DImage()));

    connect(recompileShaders, &QAction::triggered, viewer, &TextureViewer::recompileShaders);

    QGroupBox * viewerGroupBox = new QGroupBox ("Clouds: Volume Rendering", this);
    QHBoxLayout * viewerLayout = new QHBoxLayout (viewerGroupBox);
    viewerLayout->addWidget (textureWidget);

    madDockWidget = new TextureDockWidget(this);

    this->addDockWidget(Qt::RightDockWidgetArea, madDockWidget);

    //TODO : Connect madDockWidget signals to viewer slots

    connect(madDockWidget, &TextureDockWidget::redNuageValueChanged, viewer, &TextureViewer::setRedNuage);
    connect(madDockWidget, &TextureDockWidget::blueNuageValueChanged, viewer, &TextureViewer::setBlueNuage);
    connect(madDockWidget, &TextureDockWidget::greenNuageValueChanged, viewer, &TextureViewer::setGreenNuage);

    connect(madDockWidget, &TextureDockWidget::setlightposXValueChanged, viewer, &TextureViewer::setXlightpos);
    connect(madDockWidget, &TextureDockWidget::setlightposYValueChanged, viewer, &TextureViewer::setYlightpos);
    connect(madDockWidget, &TextureDockWidget::setlightposZValueChanged, viewer, &TextureViewer::setZlightpos);

    connect(madDockWidget, &TextureDockWidget::absorptionValueChanged, viewer, &TextureViewer::setAbsorptionNuage);

    connect(madDockWidget, &TextureDockWidget::onNuageSliderChanged, viewer, &TextureViewer::onNuageSliderChanged);
    connect(madDockWidget, &TextureDockWidget::onNuageSpinBoxChanged, viewer, &TextureViewer::onNuageSpinBoxChanged);

    connect(madDockWidget, &TextureDockWidget::onLightSliderChanged, viewer, &TextureViewer::onLightSliderChanged);
    connect(madDockWidget, &TextureDockWidget::onLightSpinBoxChanged, viewer, &TextureViewer::onLightSpinBoxChanged);


//    connect(madDockWidget, &TextureDockWidget::xValueChanged, viewer, &TextureViewer::setXCut);
//    connect(madDockWidget, &TextureDockWidget::yValueChanged, viewer, &TextureViewer::setYCut);
//    connect(madDockWidget, &TextureDockWidget::zValueChanged, viewer, &TextureViewer::setZCut);

//    connect(madDockWidget, &TextureDockWidget::xInvert, viewer, &TextureViewer::invertXCut);
//    connect(madDockWidget, &TextureDockWidget::yInvert, viewer, &TextureViewer::invertYCut);
//    connect(madDockWidget, &TextureDockWidget::zInvert, viewer, &TextureViewer::invertZCut);

//    connect(madDockWidget, &TextureDockWidget::xDisplay, viewer, &TextureViewer::setXCutDisplay);
//    connect(madDockWidget, &TextureDockWidget::yDisplay, viewer, &TextureViewer::setYCutDisplay);
//    connect(madDockWidget, &TextureDockWidget::zDisplay, viewer, &TextureViewer::setZCutDisplay);



    this->setCentralWidget(viewerGroupBox);

    QMenuBar * menubar = new QMenuBar(this);

    menubar->addAction(menuFile->menuAction());
    menubar->addAction(recompileShaders);

    this->setMenuBar(menubar);

    statusbar = new QStatusBar(this);

    this->setStatusBar(statusbar);

    this->setWindowTitle("PROJECT: Nuage");


}

//void Window::open3DImage(){

//    QString selectedFilter, openFileNameLabel;
//    QString fileFilter = "Known Filetypes (*.dim *.nii);;IMA (*.dim);;NIFTI (*.nii)";

//    QString fileName = QFileDialog::getOpenFileName(this,
//                                                    tr("Select an input 3D image"),
//                                                    openFileNameLabel,
//                                                    fileFilter,
//                                                    &selectedFilter);

//    // In case of Cancel
//    if ( fileName.isEmpty() ) {
//        return;
//    }

//    statusBar()->showMessage("Opening 3D image...");
//    if(fileName.endsWith(".dim") || fileName.endsWith(".nii") ){
//        viewer->open3DImage(fileName);
//        statusBar()->showMessage("3D image opened");

//    }

//}
