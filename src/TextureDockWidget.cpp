#include "../header/TextureDockWidget.h"
#include <QLabel>
#include <QLayout>
#include <QFileDialog>
#include <QGroupBox>
#include <QComboBox>


using namespace std;
TextureDockWidget::TextureDockWidget(QWidget * parent ):QDockWidget(parent)
{
    QWidget * contents = new QWidget();

    QVBoxLayout * contentLayout = new QVBoxLayout(contents);


    // Create a tab widget to organize the sections
    QTabWidget *tabWidget = new QTabWidget(contents);

    //////////////////////////////////////////////////////////////////////////////////NOISE TAB
    // Create the first tab for noise parameters
    QWidget *noiseTab = new QWidget();
    QVBoxLayout *noiseLayout = new QVBoxLayout(noiseTab);
    QGroupBox *noiseGroupBox = new QGroupBox("Paramètres de Bruit", noiseTab);
    noiseLayout->addWidget(noiseGroupBox);

    // Add the noise tab to the tab widget
    tabWidget->addTab(noiseTab, "Bruit");

    //////////////////////////////////////////////////////////////////////////////////Light TAB
    // Create the second tab for light/color parameters
    QWidget *lightTab = new QWidget();
    QVBoxLayout *lightLayout = new QVBoxLayout(lightTab);

    // Create a frame for "Nuage" section
    QFrame *nuageFrame = new QFrame();
    nuageFrame->setFrameShape(QFrame::StyledPanel);
    nuageFrame->setFrameShadow(QFrame::Sunken);

    QVBoxLayout *nuageLayout = new QVBoxLayout(nuageFrame);

    QLabel *nuageLabel = new QLabel("Paramètre du Nuage:", nuageFrame);
    nuageLayout->addWidget(nuageLabel);

    // Color controls layout
    QHBoxLayout *colorNuageLayout = new QHBoxLayout();

    // Create three QDoubleSpinBoxes for RGB values (0.0 to 1.0)
    QLabel *colorNuageLabel = new QLabel("Couleur du nuage (R, G, B):", nuageFrame);
    redColorNuageSpinBox = new QDoubleSpinBox(nuageFrame);
    redColorNuageSpinBox->setRange(0.0, 1.0);
    redColorNuageSpinBox->setSingleStep(0.01);
    redColorNuageSpinBox->setValue(1.0);
    redColorNuageSpinBox->setFixedWidth(60); // Reduce the width of the spinboxes

    greenColorNuageSpinBox = new QDoubleSpinBox(nuageFrame);
    greenColorNuageSpinBox->setRange(0.0, 1.0);
    greenColorNuageSpinBox->setSingleStep(0.01);
    greenColorNuageSpinBox->setValue(1.0);
    greenColorNuageSpinBox->setFixedWidth(60);

    blueColorNuageSpinBox = new QDoubleSpinBox(nuageFrame);
    blueColorNuageSpinBox->setRange(0.0, 1.0);
    blueColorNuageSpinBox->setSingleStep(0.01);
    blueColorNuageSpinBox->setValue(1.0);
    blueColorNuageSpinBox->setFixedWidth(60);

    // Add the color controls to the color layout
    colorNuageLayout->addWidget(colorNuageLabel);
    colorNuageLayout->addWidget(redColorNuageSpinBox);
    colorNuageLayout->addWidget(greenColorNuageSpinBox);
    colorNuageLayout->addWidget(blueColorNuageSpinBox);
    nuageLayout->addLayout(colorNuageLayout);

    // Absorption controls layout
    QLabel *absorptionLabel = new QLabel("Coeff d'absorption:", nuageFrame);
    absorptionSlider = new QSlider(Qt::Horizontal, nuageFrame);
    absorptionSlider->setRange(60,300);
    absorptionSlider->setValue(90);
    absorptionSpinBox = new QDoubleSpinBox(nuageFrame);
    absorptionSpinBox->setRange(0.6, 3.0);
    absorptionSpinBox->setSingleStep(0.1);
    absorptionSpinBox->setValue(0.90);
    QHBoxLayout *absorptionLayout = new QHBoxLayout();
    absorptionLayout->addWidget(absorptionLabel);
    absorptionLayout->addWidget(absorptionSlider);
    absorptionLayout->addWidget(absorptionSpinBox);
    nuageLayout->addLayout(absorptionLayout);


    nuageFrame->adjustSize();

    // Add Nuage frame to layout
    lightLayout->addWidget(nuageFrame);

    // Create a frame for "Light" section
    QFrame *lightFrame = new QFrame();
    lightFrame->setFrameShape(QFrame::StyledPanel);
    lightFrame->setFrameShadow(QFrame::Sunken);

    QVBoxLayout *lightParamLayout = new QVBoxLayout(lightFrame);
    QLabel *lightLabel = new QLabel("Paramètres de la lumière:", lightFrame);
    lightParamLayout->addWidget(lightLabel);
    // Add more light-related controls here

    // Add Light frame to main layout
    lightLayout->addWidget(lightFrame);




    // Add the light/color tab to the tab widget
    tabWidget->addTab(lightTab, "Lumière/Couleur");

    //////////////////////////////////////////////////////////////////////////////////other TAB je sais pas si jamais sinon on suppr
    QWidget *otherTab = new QWidget();
    QVBoxLayout *otherLayout = new QVBoxLayout(otherTab);
    QGroupBox *otherGroupBox = new QGroupBox("Autres Paramètres", otherTab);
    otherLayout->addWidget(otherGroupBox);

    // Add the other tab to the tab widget
    tabWidget->addTab(otherTab, "Autres");

    //////////////////////////////////////////////////////////////////////////////////////

    // Add the tab widget to the main layout
    contentLayout->addWidget(tabWidget);
    contentLayout->addStretch(0);

    this->setWidget(contents);

    // QGroupBox * groupBox = new QGroupBox("Paramètres", parent);
    // groupBox->setMaximumSize(QSize(16777215, 200));

    // contentLayout->addWidget ( groupBox) ;

//    QGridLayout * cuttingPlaneGridLayout = new QGridLayout(groupBox);
//    xHSlider = new QSlider(groupBox);
//    xHSlider->setOrientation(Qt::Horizontal);
//    xHSlider->setMaximum(sliderMax);
//    cuttingPlaneGridLayout->addWidget(xHSlider, 1, 0, 1, 1);


//    yHSlider = new QSlider(groupBox);
//    yHSlider->setOrientation(Qt::Horizontal);
//    yHSlider->setMaximum(sliderMax);
//    cuttingPlaneGridLayout->addWidget(yHSlider, 3, 0, 1, 1);


//    zHSlider = new QSlider(groupBox);
//    zHSlider->setOrientation(Qt::Horizontal);
//    zHSlider->setMaximum(sliderMax);
//    cuttingPlaneGridLayout->addWidget(zHSlider, 5, 0, 1, 1);


//    QPushButton * invertXPushButton = new QPushButton("invert", groupBox);
//    cuttingPlaneGridLayout->addWidget(invertXPushButton, 1, 1, 1, 1);

//    QPushButton * invertYPushButton = new QPushButton("invert", groupBox);
//    cuttingPlaneGridLayout->addWidget(invertYPushButton, 3, 1, 1, 1);

//    QPushButton * invertZPushButton = new QPushButton("invert", groupBox);
//    cuttingPlaneGridLayout->addWidget(invertZPushButton, 5, 1, 1, 1);

//    QLabel * labelCutX = new QLabel("x cut position", groupBox);
//    cuttingPlaneGridLayout->addWidget(labelCutX, 0, 0, 1, 1);

//    QLabel * labelCutY = new QLabel("y cut position", groupBox);
//    cuttingPlaneGridLayout->addWidget(labelCutY, 2, 0, 1, 1);

//    QLabel * labelCutZ = new QLabel("z cut position", groupBox);
//    cuttingPlaneGridLayout->addWidget(labelCutZ, 4, 0, 1, 1);

//    QCheckBox * displayXCut = new QCheckBox("display", groupBox);
//    cuttingPlaneGridLayout->addWidget(displayXCut, 0, 1, 1, 1);

//    QCheckBox * displayYCut = new QCheckBox("display", groupBox);
//    cuttingPlaneGridLayout->addWidget(displayYCut, 2, 1, 1, 1);

//    QCheckBox * displayZCut = new QCheckBox("display", groupBox);
//    cuttingPlaneGridLayout->addWidget(displayZCut, 4, 1, 1, 1);

//    connect(xHSlider, &QSlider::valueChanged, this, &TextureDockWidget::xSliderChangedSlot);
//    connect(yHSlider, &QSlider::valueChanged, this, &TextureDockWidget::ySliderChangedSlot);
//    connect(zHSlider, &QSlider::valueChanged, this, &TextureDockWidget::zSliderChangedSlot);

//    connect(invertXPushButton, &QPushButton::pressed, this, &TextureDockWidget::xInvertPlaneSlot);
//    connect(invertYPushButton, &QPushButton::pressed, this, &TextureDockWidget::yInvertPlaneSlot);
//    connect(invertZPushButton, &QPushButton::pressed, this, &TextureDockWidget::zInvertPlaneSlot);

//    connect(displayXCut, &QCheckBox::stateChanged, this, &TextureDockWidget::xDisplaySlot);
//    connect(displayYCut, &QCheckBox::stateChanged, this, &TextureDockWidget::yDisplaySlot);
//    connect(displayZCut, &QCheckBox::stateChanged, this, &TextureDockWidget::zDisplaySlot);
    connect(redColorNuageSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &TextureDockWidget::redNuageValueChanged);
    connect(greenColorNuageSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &TextureDockWidget::greenNuageValueChanged);
    connect(blueColorNuageSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &TextureDockWidget::blueNuageValueChanged);
    connect(absorptionSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &TextureDockWidget::absorptionValueChanged);
    connect(absorptionSlider, &QSlider::valueChanged, this, &TextureDockWidget::absorptionSliderChangedSlot);

    // Make the two-way connections
    connect(absorptionSlider, &QSlider::valueChanged, this, [this](int value) {
        absorptionSpinBox->setValue(value / 100.0);  // Assuming max slider value is 300
    });
    connect(absorptionSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, [this](double value) {
        absorptionSlider->setValue(static_cast<int>(value * 100));  // Assuming max slider value is 300
    });

    // contentLayout->addStretch(0);
    // this->setWidget(contents);
}

void TextureDockWidget::redNuageSpinBoxChangedSlot(float value){emit redNuageValueChanged(value) ;}
void TextureDockWidget::greenNuageSpinBoxChangedSlot(float value){emit greenNuageValueChanged(value) ;}
void TextureDockWidget::blueNuageSpinBoxChangedSlot(float value){emit blueNuageValueChanged(value);}
void TextureDockWidget::absorptionSpinBoxChangedSlot(float value){emit absorptionValueChanged(value) ;}
void TextureDockWidget::absorptionSliderChangedSlot(int i){emit absorptionValueChanged((float) i/(float)sliderAbsorptionMax);}

//void TextureDockWidget::xSliderChangedSlot(int i) {emit xValueChanged((float)i/(float) sliderMax);}
//void TextureDockWidget::ySliderChangedSlot(int i) {emit yValueChanged((float)i/(float) sliderMax);}
//void TextureDockWidget::zSliderChangedSlot(int i) {emit zValueChanged((float)i/(float) sliderMax);}

//void TextureDockWidget::xInvertPlaneSlot() {emit xInvert();}
//void TextureDockWidget::yInvertPlaneSlot() {emit yInvert();}
//void TextureDockWidget::zInvertPlaneSlot() {emit zInvert();}

//void TextureDockWidget::xDisplaySlot(bool v) {emit xDisplay(v);}
//void TextureDockWidget::yDisplaySlot(bool v) {emit yDisplay(v);}
//void TextureDockWidget::zDisplaySlot(bool v) {emit zDisplay(v);}


//void TextureDockWidget::setMaxCutPlanes( int x, int y , int z ){
//        xHSlider->setRange(0,x);
//        yHSlider->setRange(0,y);
//        zHSlider->setRange(0,z);
//}


