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
    noiseLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    noiseTab->setMaximumWidth(450);  // Set a maximum width
    noiseTab->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    // Create a frame for "Nuage" section
    QFrame *bruitFrame = new QFrame();
    bruitFrame->setFrameShape(QFrame::StyledPanel);
    bruitFrame->setFrameShadow(QFrame::Sunken);

    QVBoxLayout *bruitLayout = new QVBoxLayout(bruitFrame);

    QLabel *bruitLabel = new QLabel("Paramètre du Bruit Worley:", bruitFrame);
    noiseLayout->addWidget(bruitLabel);

    // Color controls layout
    QHBoxLayout *bruitworleyLayout = new QHBoxLayout();

    // Create three QDoubleSpinBoxes for RGB values (0.0 to 1.0)
    QLabel *bruitworleyLabel = new QLabel("Resolution tex3D X/Y/Z: ", bruitFrame);
    xbruitworleySpinBox = new PowerOfTwoSpinBox(bruitFrame);
//    xbruitworleySpinBox->setRange(0.0, 512.0);
//    xbruitworleySpinBox->setSingleStep(1.0);
//    xbruitworleySpinBox->setValue(128.0);
    xbruitworleySpinBox->setFixedWidth(50); // Reduce the width of the spinboxes

    ybruitworleySpinBox = new PowerOfTwoSpinBox(bruitFrame);
//    ybruitworleySpinBox->setRange(0.0, 512.0);
//    ybruitworleySpinBox->setSingleStep(1.0);
//    ybruitworleySpinBox->setValue(128.0);
    ybruitworleySpinBox->setFixedWidth(50);

    zbruitworleySpinBox = new PowerOfTwoSpinBox(bruitFrame);
//    zbruitworleySpinBox->setRange(0.0, 512.0);
//    zbruitworleySpinBox->setSingleStep(1.0);
//    zbruitworleySpinBox->setValue(128.0);
    zbruitworleySpinBox->setFixedWidth(50);

    // Add the color controls to the color layout
    bruitworleyLayout->addWidget(bruitworleyLabel);
    bruitworleyLayout->addWidget(xbruitworleySpinBox);
    bruitworleyLayout->addWidget(ybruitworleySpinBox);
    bruitworleyLayout->addWidget(zbruitworleySpinBox);

    // freq controls layout
    QHBoxLayout *freqbruitworleyLayout = new QHBoxLayout();

    QLabel *freqworleyLabel = new QLabel("Frequence R/G/B/A: ", bruitFrame);
    rfreqWorleySpinBox = new QDoubleSpinBox(bruitFrame);
    rfreqWorleySpinBox->setRange(1.0, 32.0);
    rfreqWorleySpinBox->setSingleStep(0.5);
    rfreqWorleySpinBox->setValue(2.0);
    rfreqWorleySpinBox->setFixedWidth(50); // Reduce the width of the spinboxes

    gfreqWorleySpinBox = new QDoubleSpinBox(bruitFrame);
    gfreqWorleySpinBox->setRange(1.0, 32.0);
    gfreqWorleySpinBox->setSingleStep(0.5);
    gfreqWorleySpinBox->setValue(6.0);
    gfreqWorleySpinBox->setFixedWidth(50);

    bfreqWorleySpinBox = new QDoubleSpinBox(bruitFrame);
    bfreqWorleySpinBox->setRange(1.0, 32.0);
    bfreqWorleySpinBox->setSingleStep(0.5);
    bfreqWorleySpinBox->setValue(12.0);
    bfreqWorleySpinBox->setFixedWidth(50);

    afreqWorleySpinBox = new QDoubleSpinBox(bruitFrame);
    afreqWorleySpinBox->setRange(1.0, 32.0);
    afreqWorleySpinBox->setSingleStep(0.5);
    afreqWorleySpinBox->setValue(24.0);
    afreqWorleySpinBox->setFixedWidth(50);

    // Add the color controls to the color layout
    freqbruitworleyLayout->addWidget(freqworleyLabel);
    freqbruitworleyLayout->addWidget(rfreqWorleySpinBox);
    freqbruitworleyLayout->addWidget(gfreqWorleySpinBox);
    freqbruitworleyLayout->addWidget(bfreqWorleySpinBox);
    freqbruitworleyLayout->addWidget(afreqWorleySpinBox);

    bruitLayout->addLayout(bruitworleyLayout);
    bruitLayout->addLayout(freqbruitworleyLayout);
    bruitFrame->adjustSize();

    // Add Nuage frame to layout
    noiseLayout->addWidget(bruitFrame);

    // Add the noise tab to the tab widget
    tabWidget->addTab(noiseTab, "Bruit");

    //////////////////////////////////////////////////////////////////////////////////Light TAB
    // Create the second tab for light/color parameters
    QWidget *lightTab = new QWidget();
    QVBoxLayout *lightLayout = new QVBoxLayout(lightTab);
    lightLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    lightTab->setMaximumWidth(450);  // Set a maximum width
    lightTab->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    // Create a frame for "Nuage" section
    QFrame *nuageFrame = new QFrame();
    nuageFrame->setFrameShape(QFrame::StyledPanel);
    nuageFrame->setFrameShadow(QFrame::Sunken);


    QVBoxLayout *nuageLayout = new QVBoxLayout(nuageFrame);

    QLabel *nuageLabel = new QLabel("Paramètre du Nuage:", nuageFrame);
    lightLayout->addWidget(nuageLabel);

    // Color controls layout
    QHBoxLayout *colorNuageLayout = new QHBoxLayout();

    // Create three QDoubleSpinBoxes for RGB values (0.0 to 1.0)
    QLabel *colorNuageLabel = new QLabel("Couleur du nuage (R, G, B):", nuageFrame);
    redColorNuageSpinBox = new QDoubleSpinBox(nuageFrame);
    redColorNuageSpinBox->setRange(0.0, 1.0);
    redColorNuageSpinBox->setSingleStep(0.01);
    redColorNuageSpinBox->setValue(1.0);
    redColorNuageSpinBox->setFixedWidth(50); // Reduce the width of the spinboxes

    greenColorNuageSpinBox = new QDoubleSpinBox(nuageFrame);
    greenColorNuageSpinBox->setRange(0.0, 1.0);
    greenColorNuageSpinBox->setSingleStep(0.01);
    greenColorNuageSpinBox->setValue(1.0);
    greenColorNuageSpinBox->setFixedWidth(50);

    blueColorNuageSpinBox = new QDoubleSpinBox(nuageFrame);
    blueColorNuageSpinBox->setRange(0.0, 1.0);
    blueColorNuageSpinBox->setSingleStep(0.01);
    blueColorNuageSpinBox->setValue(1.0);
    blueColorNuageSpinBox->setFixedWidth(50);

    // Add the color controls to the color layout
    colorNuageLayout->addWidget(colorNuageLabel);
    colorNuageLayout->addWidget(redColorNuageSpinBox);
    colorNuageLayout->addWidget(greenColorNuageSpinBox);
    colorNuageLayout->addWidget(blueColorNuageSpinBox);
    nuageLayout->addLayout(colorNuageLayout);

    // Absorption controls layout
    QLabel *absorptionLabel = new QLabel("Coeff d'absorption:", nuageFrame);
    absorptionSlider = new QSlider(Qt::Horizontal, nuageFrame);
    absorptionSlider->setRange(60,2500);
    absorptionSlider->setValue(900);
    absorptionSlider->setFixedWidth(100);
    absorptionSpinBox = new QDoubleSpinBox(nuageFrame);
    absorptionSpinBox->setRange(0.6, 25.0);
    absorptionSpinBox->setSingleStep(0.1);
    absorptionSpinBox->setValue(9.0);
    absorptionSpinBox->setFixedWidth(50);
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
    QVBoxLayout *lightlayout = new QVBoxLayout(lightFrame);
    QHBoxLayout *lightposLayout = new QHBoxLayout();
    QHBoxLayout *lightcolLayout = new QHBoxLayout();

    QLabel *lightLabel = new QLabel("Paramètres de la lumière:", nuageFrame);
    lightLayout->addWidget(lightLabel);


    // Add more light-related controls here

    QLabel *positionLight = new QLabel("Position de la lumière:", lightFrame);
    LightPosX = new QDoubleSpinBox(lightFrame);
    LightPosX->setRange(-25.0, 25.0);
    LightPosX->setSingleStep(0.1);
    LightPosX->setValue(0.0);
    LightPosX->setFixedWidth(50); // Reduce the width of the spinboxes

    LightPosY = new QDoubleSpinBox(lightFrame);
    LightPosY->setRange(-25.0, 25.0);
    LightPosY->setSingleStep(0.1);
    LightPosY->setValue(1.0);
    LightPosY->setFixedWidth(50); // Reduce the width of the spinboxes

    LightPosZ = new QDoubleSpinBox(lightFrame);
    LightPosZ->setRange(-25.0, 25.0);
    LightPosZ->setSingleStep(0.1);
    LightPosZ->setValue(0.0);
    LightPosZ->setFixedWidth(50); // Reduce the width of the spinboxes

    QLabel *colorLight = new QLabel("Couleur de la lumière (R, G, B):", lightFrame);

    LightColorR = new QDoubleSpinBox(lightFrame);
    LightColorR->setRange(0.0, 1.0);
    LightColorR->setSingleStep(0.01);
    LightColorR->setValue(1.0);
    LightColorR->setFixedWidth(50); // Reduce the width of the spinboxes

    LightColorG = new QDoubleSpinBox(lightFrame);
    LightColorG->setRange(0.0, 1.0);
    LightColorG->setSingleStep(0.01);
    LightColorG->setValue(1.0);
    LightColorG->setFixedWidth(50); // Reduce the width of the spinboxes

    LightColorB = new QDoubleSpinBox(lightFrame);
    LightColorB->setRange(0.0, 1.0);
    LightColorB->setSingleStep(0.01);
    LightColorB->setValue(1.0);
    LightColorB->setFixedWidth(50); // Reduce the width of the spinboxes

    lightposLayout->addWidget(positionLight);
    lightposLayout->addWidget(LightPosX);
    lightposLayout->addWidget(LightPosY);
    lightposLayout->addWidget(LightPosZ);

    lightcolLayout->addWidget(colorLight);
    lightcolLayout->addWidget(LightColorR);
    lightcolLayout->addWidget(LightColorG);
    lightcolLayout->addWidget(LightColorB);

    lightlayout->addLayout(lightposLayout);
    lightlayout->addLayout(lightcolLayout);

    // Add Light frame to main layout
    lightLayout->addWidget(lightFrame);

    // Add the light/color tab to the tab widget
    tabWidget->addTab(lightTab, "Lumière/Couleur");

    //////////////////////////////////////////////////////////////////////////////////other TAB je sais pas si jamais sinon on suppr
    QWidget *otherTab = new QWidget();
      QVBoxLayout *otherLayout = new QVBoxLayout(otherTab);

      QGroupBox *otherGroupBox = new QGroupBox("Échantillons", otherTab);
      QVBoxLayout *groupBoxLayout = new QVBoxLayout(otherGroupBox);

      // Widgets pour le nuage
      QLabel *SampleNuageLabel = new QLabel("Nuage :", otherGroupBox);
      QSlider *NuageSampleSlider = new QSlider(Qt::Horizontal, otherGroupBox);
      NuageSampleSlider->setRange(3, 200); // Plage entière
      NuageSampleSlider->setValue(50);    // Valeur par défaut

      QSpinBox *NuageSampleBox = new QSpinBox(otherGroupBox);
      NuageSampleBox->setRange(3, 200);   // Plage entière
      NuageSampleBox->setSingleStep(1);
      NuageSampleBox->setValue(50);       // Valeur par défaut

      // Disposition horizontale pour les contrôles de nuage
      QHBoxLayout *nuageEchLayout = new QHBoxLayout();
      nuageEchLayout->addWidget(SampleNuageLabel);
      nuageEchLayout->addWidget(NuageSampleSlider);
      nuageEchLayout->addWidget(NuageSampleBox);

      // Synchronisation entre Slider et SpinBox pour le nuage
      QObject::connect(NuageSampleSlider, &QSlider::valueChanged, NuageSampleBox, &QSpinBox::setValue);
      QObject::connect(NuageSampleBox, QOverload<int>::of(&QSpinBox::valueChanged), NuageSampleSlider, &QSlider::setValue);

      // Widgets pour la lumière
      QLabel *SampleLightLabel = new QLabel("Lumière :", otherGroupBox);
      QSlider *LightSampleSlider = new QSlider(Qt::Horizontal, otherGroupBox);
      LightSampleSlider->setRange(3, 100); // Plage entière
      LightSampleSlider->setValue(20);    // Valeur par défaut

      QSpinBox *LightSampleBox = new QSpinBox(otherGroupBox);
      LightSampleBox->setRange(3,  100);   // Plage entière
      LightSampleBox->setSingleStep(1);
      LightSampleBox->setValue(20);       // Valeur par défaut

      // Disposition horizontale pour les contrôles de lumière
      QHBoxLayout *lightEchLayout = new QHBoxLayout();
      lightEchLayout->addWidget(SampleLightLabel);
      lightEchLayout->addWidget(LightSampleSlider);
      lightEchLayout->addWidget(LightSampleBox);

      // Synchronisation entre Slider et SpinBox pour la lumière
      QObject::connect(LightSampleSlider, &QSlider::valueChanged, LightSampleBox, &QSpinBox::setValue);
      QObject::connect(LightSampleBox, QOverload<int>::of(&QSpinBox::valueChanged), LightSampleSlider, &QSlider::setValue);

      // Ajout des layouts horizontaux au QGroupBox
      groupBoxLayout->addLayout(nuageEchLayout);
      groupBoxLayout->addLayout(lightEchLayout);

      otherGroupBox->setLayout(groupBoxLayout);
      otherLayout->addWidget(otherGroupBox);

      // Ajout de l'onglet
      tabWidget->addTab(otherTab, "Autres");
    // Add the tab widget to the main layout
    contentLayout->addWidget(tabWidget);
    contentLayout->addStretch(0);

    this->setWidget(contents);

    connect(redColorNuageSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &TextureDockWidget::redNuageValueChanged);
    connect(greenColorNuageSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &TextureDockWidget::greenNuageValueChanged);
    connect(blueColorNuageSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &TextureDockWidget::blueNuageValueChanged);

    connect(LightPosX, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &TextureDockWidget::setlightposXValueChanged);
    connect(LightPosY, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &TextureDockWidget::setlightposYValueChanged);
    connect(LightPosZ, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &TextureDockWidget::setlightposZValueChanged);


    connect(LightColorR, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &TextureDockWidget::setlightcolRValueChanged);
    connect(LightColorG, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &TextureDockWidget::setlightcolGValueChanged);
    connect(LightColorB, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &TextureDockWidget::setlightcolBValueChanged);

    connect(NuageSampleSlider, &QSlider::valueChanged, this, &TextureDockWidget::onNuageSliderChanged);
     connect(NuageSampleBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &TextureDockWidget::onNuageSpinBoxChanged);

     connect(LightSampleSlider, &QSlider::valueChanged, this, &TextureDockWidget::onLightSliderChanged);
     connect(LightSampleBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &TextureDockWidget::onLightSpinBoxChanged);


    connect(absorptionSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &TextureDockWidget::absorptionValueChanged);
    connect(absorptionSlider, &QSlider::valueChanged, this, &TextureDockWidget::absorptionSliderChangedSlot);

    // Make the two-way connections
    connect(absorptionSlider, &QSlider::valueChanged, this, [this](int value) {
        absorptionSpinBox->setValue(value / 100.0);  // Assuming max slider value is 300
    });
    connect(absorptionSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, [this](double value) {
        absorptionSlider->setValue(static_cast<int>(value * 100));  // Assuming max slider value is 300
    });
    connect(xbruitworleySpinBox, QOverload<double>::of(&PowerOfTwoSpinBox::valueChanged), this, &TextureDockWidget::xResolutionBruitValueChanged);
    connect(ybruitworleySpinBox, QOverload<double>::of(&PowerOfTwoSpinBox::valueChanged), this, &TextureDockWidget::yResolutionBruitValueChanged);
    connect(zbruitworleySpinBox, QOverload<double>::of(&PowerOfTwoSpinBox::valueChanged), this, &TextureDockWidget::zResolutionBruitValueChanged);

    connect(rfreqWorleySpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &TextureDockWidget::rFreqBruitValueChanged);
    connect(gfreqWorleySpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &TextureDockWidget::gFreqBruitValueChanged);
    connect(bfreqWorleySpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &TextureDockWidget::bFreqBruitValueChanged);
    connect(afreqWorleySpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &TextureDockWidget::aFreqBruitValueChanged);

}
void TextureDockWidget::onNuageSliderChangedSlot(int value) {emit onNuageSliderChanged(value);}
void TextureDockWidget::onNuageSpinBoxChangedSlot(int value){emit onNuageSpinBoxChanged(value);}
void TextureDockWidget::onLightSliderChangedSlot(int value){emit onLightSliderChanged(value);}
void TextureDockWidget::onLightSpinBoxChangedSlot(int value){emit onLightSpinBoxChanged(value);}
void TextureDockWidget::redNuageSpinBoxChangedSlot(float value){emit redNuageValueChanged(value) ;}
void TextureDockWidget::greenNuageSpinBoxChangedSlot(float value){emit greenNuageValueChanged(value) ;}
void TextureDockWidget::blueNuageSpinBoxChangedSlot(float value){emit blueNuageValueChanged(value);}
void TextureDockWidget::setlightposXSlot(float value){emit setlightposXValueChanged(value) ;}
void TextureDockWidget::setlightposYSlot(float value){emit setlightposYValueChanged(value) ;}
void TextureDockWidget::setlightposZSlot(float value){emit setlightposZValueChanged(value);}
void TextureDockWidget::setlightcolRSlot(float value){emit setlightcolRValueChanged(value) ;}
void TextureDockWidget::setlightcolGSlot(float value){emit setlightcolGValueChanged(value) ;}
void TextureDockWidget::setlightcolBSlot(float value){emit setlightcolBValueChanged(value);}
void TextureDockWidget::absorptionSpinBoxChangedSlot(float value){emit absorptionValueChanged(value) ;}
void TextureDockWidget::absorptionSliderChangedSlot(int i){emit absorptionValueChanged((float) i/(float)sliderAbsorptionMax);}
void TextureDockWidget::xResolutionBruitSpinBoxChangedSlot(float value){emit xResolutionBruitValueChanged(value) ;}
void TextureDockWidget::yResolutionBruitSpinBoxChangedSlot(float value){emit yResolutionBruitValueChanged(value) ;}
void TextureDockWidget::zResolutionBruitSpinBoxChangedSlot(float value){emit zResolutionBruitValueChanged(value);}
void TextureDockWidget::rFreqBruitSpinBoxChangedSlot(float value){emit rFreqBruitValueChanged(value) ;}
void TextureDockWidget::gFreqBruitSpinBoxChangedSlot(float value){emit gFreqBruitValueChanged(value) ;}
void TextureDockWidget::bFreqBruitSpinBoxChangedSlot(float value){emit bFreqBruitValueChanged(value);}
void TextureDockWidget::aFreqBruitSpinBoxChangedSlot(float value){emit aFreqBruitValueChanged(value) ;}



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


