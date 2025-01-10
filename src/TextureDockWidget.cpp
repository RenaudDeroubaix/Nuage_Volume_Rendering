#include "../header/TextureDockWidget.h"
#include "../header/WidgetSetup.h"
#include "qdebug.h"
#include <QLabel>
#include <QLayout>
#include <QFileDialog>
#include <QGroupBox>
#include <QComboBox>


using namespace std;

TextureDockWidget::TextureDockWidget(QWidget * parent ):QDockWidget(parent)
{
    QWidget * contents = new QWidget();

    // Create a tab widget to organize the sections
    QTabWidget *tabWidget = new QTabWidget(contents);

    //////////////////////////////////////////////////////////////////////////////////NOISE TAB
    // Create the first tab for noise parameters
    QWidget *noiseTab = new QWidget();
    noiseTab->setMaximumWidth(450);  // Set a maximum width
    noiseTab->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    // Create a frame for "Nuage" section
    QFrame *paramNuageFrame = new QFrame();
    paramNuageFrame->setFrameShape(QFrame::StyledPanel);
    paramNuageFrame->setFrameShadow(QFrame::Sunken);


    QLabel *paramNuageLabel = new QLabel("Information sur le Nuage:");
    QLabel *BBminLabel = new QLabel("Position BBmin Nuage:", paramNuageFrame);
    xBBminSpinBox = new QDoubleSpinBox(paramNuageFrame);
    setSpinBox(xBBminSpinBox,-500,500.0,0.1,-5.0,50);
    yBBminSpinBox = new QDoubleSpinBox(paramNuageFrame);
    setSpinBox(yBBminSpinBox,-500,500.0,0.1,-5.0,50);
    zBBminSpinBox = new QDoubleSpinBox(paramNuageFrame);
    setSpinBox(zBBminSpinBox,-500,500.0,0.1,-5.0,50);

    QLabel *BBmaxLabel = new QLabel("Position BBmax Nuage:", paramNuageFrame);
    xBBmaxSpinBox = new QDoubleSpinBox(paramNuageFrame);
    setSpinBox(xBBmaxSpinBox,-500,500.0,0.1,5.0,50);
    yBBmaxSpinBox = new QDoubleSpinBox(paramNuageFrame);
    setSpinBox(yBBmaxSpinBox,-500,500.0,0.1,5.0,50);
    zBBmaxSpinBox = new QDoubleSpinBox(paramNuageFrame);
    setSpinBox(zBBmaxSpinBox,-500,500.0,0.1,5.0,50);

    QBoxLayout *BBminLayout = createLayout(NULL,false,{BBminLabel,xBBminSpinBox,yBBminSpinBox,zBBminSpinBox},{});
    QBoxLayout *BBmaxLayout = createLayout(NULL,false,{BBmaxLabel,xBBmaxSpinBox,yBBmaxSpinBox,zBBmaxSpinBox},{});
    QBoxLayout *paramNuageLayout = createLayout(paramNuageFrame,true,{},{BBminLayout,BBmaxLayout});

    QFrame *bruitFrame = new QFrame();
    bruitFrame->setFrameShape(QFrame::StyledPanel);
    bruitFrame->setFrameShadow(QFrame::Sunken);

    QLabel *bruitLabel = new QLabel("Paramètre du Bruit de forme du nuage:");
    // Create three QDoubleSpinBoxes for RGB values (0.0 to 1.0)
    QLabel *bruitworleyLabel = new QLabel("Resolution tex3D X/Y/Z: ", bruitFrame);
    xbruitworleySpinBox = new PowerOfTwoSpinBox(bruitFrame);
    ybruitworleySpinBox = new PowerOfTwoSpinBox(bruitFrame);
    zbruitworleySpinBox = new PowerOfTwoSpinBox(bruitFrame);
    // Add the color controls to the color layout
    QBoxLayout *bruitworleyLayout = createLayout(NULL,false,{bruitworleyLabel,xbruitworleySpinBox,ybruitworleySpinBox,zbruitworleySpinBox},{});

    QLabel *freqworleyLabel = new QLabel("Frequence R/G/B/A: ", bruitFrame);
    rfreqWorleySpinBox = new QDoubleSpinBox(bruitFrame);
    setSpinBox(rfreqWorleySpinBox,1.0,32.0,0.5,2.0,50);
    gfreqWorleySpinBox = new QDoubleSpinBox(bruitFrame);
    setSpinBox(gfreqWorleySpinBox,1.0,32.0,0.5,6.0,50);
    bfreqWorleySpinBox = new QDoubleSpinBox(bruitFrame);
    setSpinBox(bfreqWorleySpinBox,1.0,32.0,0.5,12.0,50);
    afreqWorleySpinBox = new QDoubleSpinBox(bruitFrame);
    setSpinBox(afreqWorleySpinBox,1.0,32.0,0.5,24.0,50);

    QLabel *facteurworleyLabel = new QLabel("Facteur de R/G/B/A: ", bruitFrame);
    rfacteurWorleySpinBox = new QDoubleSpinBox(bruitFrame);
    setSpinBox(rfacteurWorleySpinBox,-10.0,10.0,0.01,-3.0,50);
    gfacteurWorleySpinBox = new QDoubleSpinBox(bruitFrame);
    setSpinBox(gfacteurWorleySpinBox,-10.0,10.0,0.01,0.33,50);
    bfacteurWorleySpinBox = new QDoubleSpinBox(bruitFrame);
    setSpinBox(bfacteurWorleySpinBox,-10.0,10.0,0.01,0.33,50);
    afacteurWorleySpinBox = new QDoubleSpinBox(bruitFrame);
    setSpinBox(afacteurWorleySpinBox,-10.0,10.0,0.01,0.33,50);

    // Add the color controls to the color layout
    QBoxLayout * facteurbruitworleyLayout = createLayout(NULL,false,{facteurworleyLabel,rfacteurWorleySpinBox,gfacteurWorleySpinBox,bfacteurWorleySpinBox,afacteurWorleySpinBox},{});
    QBoxLayout *freqbruitworleyLayout = createLayout(NULL,false,{freqworleyLabel,rfreqWorleySpinBox,gfreqWorleySpinBox,bfreqWorleySpinBox,afreqWorleySpinBox},{});
    QBoxLayout *bruitLayout = createLayout(bruitFrame,true,{},{bruitworleyLayout,freqbruitworleyLayout,facteurbruitworleyLayout});

    QFrame *bruitCurlFrame = new QFrame();
    bruitCurlFrame->setFrameShape(QFrame::StyledPanel);
    bruitCurlFrame->setFrameShadow(QFrame::Sunken);
    QLabel *bruitCurlParamLabel = new QLabel("Paramètre du Bruit de turbulance du nuage:");
    QLabel *bruitCurlLabel = new QLabel("Resolution tex2D X/Y: ", bruitCurlFrame);
    xbruitCurlSpinBox = new PowerOfTwoSpinBox(bruitCurlFrame);
    ybruitCurlSpinBox = new PowerOfTwoSpinBox(bruitCurlFrame);
    QBoxLayout *bruitCurlLayout = createLayout(NULL,false,{bruitCurlLabel,xbruitCurlSpinBox,ybruitCurlSpinBox},{});

    QLabel *freqCurlLabel = new QLabel("Frequence R/G/B: ", bruitCurlFrame);
    rfreqCurlSpinBox = new QDoubleSpinBox(bruitCurlFrame);
    setSpinBox(rfreqCurlSpinBox,1.0,32.0,0.5,2.0,50);
    gfreqCurlSpinBox = new QDoubleSpinBox(bruitCurlFrame);
    setSpinBox(gfreqCurlSpinBox,1.0,32.0,0.5,6.0,50);
    bfreqCurlSpinBox = new QDoubleSpinBox(bruitCurlFrame);
    setSpinBox(bfreqCurlSpinBox,1.0,32.0,0.5,12.0,50);

    // Add the color controls to the color layout
    QBoxLayout *freqbruitCurlLayout = createLayout(NULL,false,{freqCurlLabel,rfreqCurlSpinBox,gfreqCurlSpinBox,bfreqCurlSpinBox},{});
    QBoxLayout *bruitCurlParamLayout = createLayout(bruitCurlFrame,true,{},{bruitCurlLayout,freqbruitCurlLayout});

    QFrame *formeNuageFrame = new QFrame();
    formeNuageFrame->setFrameShape(QFrame::StyledPanel);
    formeNuageFrame->setFrameShadow(QFrame::Sunken);
    QLabel *formeLabel = new QLabel("Forme du nuage (SDF):", formeNuageFrame);

    // Créer des QCheckBox
    cubeCheckBox = new QCheckBox("Cube", formeNuageFrame);
    cubeCheckBox->setChecked(true);
    sphereCheckBox = new QCheckBox("Sphère", formeNuageFrame);
    torusCheckBox = new QCheckBox("Torus", formeNuageFrame);
    croixCheckBox = new QCheckBox("Croix", formeNuageFrame);
    capsuleCheckBox = new QCheckBox("Capsule", formeNuageFrame);

    // Grouper les QCheckBox pour permettre une seule sélection
    buttonGroupForme = new QButtonGroup(formeNuageFrame);
    buttonGroupForme->setExclusive(true);
    buttonGroupForme->addButton(cubeCheckBox,0);
    buttonGroupForme->addButton(sphereCheckBox,1);
    buttonGroupForme->addButton(torusCheckBox,2);
    buttonGroupForme->addButton(capsuleCheckBox,3);
    buttonGroupForme->addButton(croixCheckBox,4);

    QLabel *attenuationLabel = new QLabel("Attenuation des bords:", formeNuageFrame);
    attenuationSlider = new QSlider(Qt::Horizontal,formeNuageFrame);
    setSlider(attenuationSlider,0,100,20,150);
    QBoxLayout *attenuationLayout = createLayout(NULL,false,{attenuationLabel,attenuationSlider},{});

    QLabel *rSphereLabel = new QLabel("Rayon Sphère:", formeNuageFrame);
    rSphereSlider = new QSlider(Qt::Horizontal,formeNuageFrame);
    setSlider(rSphereSlider,0,100,80,150);
    QBoxLayout *rSphereLayout = createLayout(NULL,false,{rSphereLabel,rSphereSlider},{});

    QLabel *rITorusLabel = new QLabel("Epaisseur Torus:", formeNuageFrame);
    rITorusSlider = new QSlider(Qt::Horizontal,formeNuageFrame);
    setSlider(rITorusSlider,0,100,20,150);
    QBoxLayout *rITorusLayout = createLayout(NULL,false,{rITorusLabel,rITorusSlider},{});

    QLabel *rETorusLabel = new QLabel("Trou Torus:", formeNuageFrame);
    rETorusSlider = new QSlider(Qt::Horizontal,formeNuageFrame);
    setSlider(rETorusSlider,0,100,50,150);
    QBoxLayout *rETorusLayout = createLayout(NULL,false,{rETorusLabel,rETorusSlider},{});

    QLabel *lCapsuleLabel = new QLabel("Longueur Capsule:", formeNuageFrame);
    lCapsuleSlider = new QSlider(Qt::Horizontal,formeNuageFrame);
    setSlider(lCapsuleSlider,0,100,100,150);
    QBoxLayout *lCapsuleLayout = createLayout(NULL,false,{lCapsuleLabel,lCapsuleSlider},{});

    QLabel *rCapsuleLabel = new QLabel("Rayon Capsule:", formeNuageFrame);
    rCapsuleSlider = new QSlider(Qt::Horizontal,formeNuageFrame);
    setSlider(rCapsuleSlider,0,100,20,150);
    QBoxLayout *rCapsuleLayout = createLayout(NULL,false,{rCapsuleLabel,rCapsuleSlider},{});

    QLabel *tBarreCroixLabel = new QLabel("Taille des barres Croix:", formeNuageFrame);
    tBarreCroixSlider = new QSlider(Qt::Horizontal,formeNuageFrame);
    setSlider(tBarreCroixSlider,0,100,30,150);
    QBoxLayout *tBarreCroixLayout = createLayout(NULL,false,{tBarreCroixLabel,tBarreCroixSlider},{});

    QBoxLayout *formeLayout = createLayout(NULL,false,{cubeCheckBox,sphereCheckBox,torusCheckBox,capsuleCheckBox,croixCheckBox},{});
    QBoxLayout *formeframeLayout = createLayout(formeNuageFrame,true,{},{formeLayout,attenuationLayout,rSphereLayout,rITorusLayout,rETorusLayout,lCapsuleLayout,rCapsuleLayout,tBarreCroixLayout});

    QBoxLayout *noiseLayout = createLayout(noiseTab,true,{paramNuageLabel,paramNuageFrame,bruitLabel,bruitFrame,bruitCurlParamLabel,bruitCurlFrame,formeLabel,formeNuageFrame},{});
    noiseLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    // Add the noise tab to the tab widget
    tabWidget->addTab(noiseTab, "Bruit");

    //////////////////////////////////////////////////////////////////////////////////Light TAB
    // Create the second tab for light/color parameters
    QWidget *lightTab = new QWidget();
    lightTab->setMaximumWidth(450);  // Set a maximum width
    lightTab->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    // Create a frame for "Nuage" section
    QFrame *nuageFrame = new QFrame();
    nuageFrame->setFrameShape(QFrame::StyledPanel);
    nuageFrame->setFrameShadow(QFrame::Sunken);

    QLabel *nuageLabel = new QLabel("Paramètre du Nuage:");

    // Create three QDoubleSpinBoxes for RGB values (0.0 to 1.0)
    QLabel *colorNuageLabel = new QLabel("Couleur du nuage (R, G, B):", nuageFrame);
    redColorNuageSpinBox = new QDoubleSpinBox(nuageFrame);
    setSpinBox(redColorNuageSpinBox,0.0,1.0,0.01,1.0,50);
    greenColorNuageSpinBox = new QDoubleSpinBox(nuageFrame);
    setSpinBox(greenColorNuageSpinBox,0.0,1.0,0.01,1.0,50);
    blueColorNuageSpinBox = new QDoubleSpinBox(nuageFrame);
    setSpinBox(blueColorNuageSpinBox,0.0,1.0,0.01,1.0,50);

    // Absorption controls layout
    QLabel *absorptionLabel = new QLabel("Coeff d'absorption:", nuageFrame);
    absorptionSlider = new QSlider(Qt::Horizontal, nuageFrame);
    setSlider(absorptionSlider,0,2500,900,100);
    absorptionSpinBox = new QDoubleSpinBox(nuageFrame);
    setSpinBox(absorptionSpinBox,0.0,25.0,0.1,9.0,50);

    nuageFrame->adjustSize();

    // Create a frame for "Light" section
    QFrame *lightFrame = new QFrame();
    lightFrame->setFrameShape(QFrame::StyledPanel);
    lightFrame->setFrameShadow(QFrame::Sunken);

    QLabel *lightLabel = new QLabel("Paramètres de la lumière:" );

    QLabel *positionLight = new QLabel("Position du soleil :", lightFrame);
    LightPosX = new QDoubleSpinBox(lightFrame);
    setSpinBox(LightPosX,-250.0,250.0,0.1,0.0,50);
    LightPosY = new QDoubleSpinBox(lightFrame);
    setSpinBox(LightPosY,-250.0,250.0,0.1,1.0,50);
    LightPosZ = new QDoubleSpinBox(lightFrame);
    setSpinBox(LightPosZ,-250.0,250.0,0.1,0.0,50);

    QLabel *colorLight = new QLabel("Couleur du soleil (R, G, B):", lightFrame);
    LightColorR = new QDoubleSpinBox(lightFrame);
    setSpinBox(LightColorR,0.0,1.0,0.01,1.0,50);
    LightColorG = new QDoubleSpinBox(lightFrame);
    setSpinBox(LightColorG,0.0,1.0,0.01,1.0,50);
    LightColorB = new QDoubleSpinBox(lightFrame);
    setSpinBox(LightColorB,0.0,1.0,0.01,1.0,50);

    QLabel *rayonSoleilVisuelLabel = new QLabel("Rayon du soleil:", lightFrame);
    rayonSoleilSlider = new QSlider(Qt::Horizontal,lightFrame);
    setSlider(rayonSoleilSlider,0,200,10,150);

    QLabel *absorptionLightLabel = new QLabel("Coeff d'absorption:", lightFrame);
    absorptionLightSlider = new QSlider(Qt::Horizontal, lightFrame);
    setSlider(absorptionLightSlider,0,2500,100,100);
    absorptionLightSpinBox = new QDoubleSpinBox(lightFrame);
    setSpinBox(absorptionLightSpinBox,0.0,25.0,0.1,1.0,50);

    // Add the light/color tab to the tab widget
    QBoxLayout *absorptionLightLayout = createLayout(NULL,false,{absorptionLightLabel, absorptionLightSlider,absorptionLightSpinBox},{});
    QBoxLayout *rayonSoleilVisuelLayout = createLayout(NULL,false,{rayonSoleilVisuelLabel, rayonSoleilSlider},{});
    QBoxLayout *lightposLayout = createLayout(NULL,false,{positionLight,LightPosX,LightPosY,LightPosZ},{});
    QBoxLayout *lightcolLayout = createLayout(NULL, false, {colorLight,LightColorR,LightColorG,LightColorB},{});
    QBoxLayout *lightInfoLayout = createLayout(lightFrame,true,{},{lightposLayout,lightcolLayout,rayonSoleilVisuelLayout,absorptionLightLayout});
    QBoxLayout *absorptionLayout = createLayout(NULL,false,{absorptionLabel,absorptionSlider,absorptionSpinBox},{});
    QBoxLayout *colorNuageLayout = createLayout(NULL,false,{colorNuageLabel,redColorNuageSpinBox,greenColorNuageSpinBox,blueColorNuageSpinBox},{});
    QBoxLayout *nuageLayout = createLayout(nuageFrame,true,{},{colorNuageLayout,absorptionLayout});
    QBoxLayout *lightLayout = createLayout(lightTab,true,{nuageLabel,nuageFrame,lightLabel,lightFrame},{});
    lightLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    tabWidget->addTab(lightTab, "Lumière/Couleur");

    //////////////////////////////////////////////////////////////////////////////////other TAB je sais pas si jamais sinon on suppr
    QWidget *otherTab = new QWidget();
    otherTab->setMaximumWidth(450);  // Set a maximum width
    otherTab->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    // Create a frame for "Nuage" section
    QFrame *echantillonFrame = new QFrame();
    echantillonFrame->setFrameShape(QFrame::StyledPanel);
    echantillonFrame->setFrameShadow(QFrame::Sunken);

    QLabel *echantillonLabel = new QLabel("Echantillons:");

    // Widgets pour le nb rayon nuage
    QLabel *SampleNuageLabel = new QLabel("Nuage :", echantillonFrame);
    NuageSampleSlider = new QSlider(Qt::Horizontal, echantillonFrame);
    setSlider(NuageSampleSlider,1,200,50,200);
    NuageSampleBox = new QSpinBox(echantillonFrame);
    setSpinBox(NuageSampleBox,1,200,1,50,50);

    // Widgets pour le nb rayon lumière
    QLabel *SampleLightLabel = new QLabel("Lumière :", echantillonFrame);
    LightSampleSlider = new QSlider(Qt::Horizontal, echantillonFrame);
    setSlider(LightSampleSlider,1,100,20,200);
    LightSampleBox = new QSpinBox(echantillonFrame);
    setSpinBox(LightSampleBox,1,100,1,20,50);

    echantillonFrame->adjustSize();

    QFrame *otherFrame = new QFrame();
    otherFrame->setFrameShape(QFrame::StyledPanel);
    otherFrame->setFrameShadow(QFrame::Sunken);
    QLabel *OtherLabel = new QLabel("Autres Paramètres:");
    QLabel *vitesseLabel = new QLabel("Vitesse du mouvement (fast to slow):",otherFrame);
    vitesseSlider = new QSlider(Qt::Horizontal, otherFrame);
    setSlider(vitesseSlider,1,5000,5000,100);

    QFrame *presetFrame = new QFrame();
    presetFrame->setFrameShape(QFrame::StyledPanel);
    presetFrame->setFrameShadow(QFrame::Sunken);

    QLabel *PresetGeneralLabel = new QLabel("Preset de nuage:");
    QPushButton *PresetButton1 = new QPushButton("Nuage 1",presetFrame);
    QPushButton *PresetButton2 = new QPushButton("Nuage 2",presetFrame);
    QPushButton *PresetButton3 = new QPushButton("Scene 1",presetFrame);
    QPushButton *PresetButton4 = new QPushButton("Scene 2",presetFrame);
    QPushButton *PresetButton5 = new QPushButton("Nuage 5",presetFrame);
    QPushButton *PresetButton6 = new QPushButton("Nuage 6",presetFrame);

    QBoxLayout *presetLayout2 = createLayout(NULL,false,{PresetButton4,PresetButton5,PresetButton6},{});
    QBoxLayout *presetLayout1 = createLayout(NULL,false,{PresetButton1,PresetButton2,PresetButton3},{});
    QBoxLayout *groupPresetLayout = createLayout(presetFrame,true,{},{presetLayout1,presetLayout2});
    QBoxLayout *vitesseLayout = createLayout(NULL,false,{vitesseLabel,vitesseSlider},{});
    QBoxLayout *otherParamLayout = createLayout(otherFrame,true,{},{vitesseLayout});
    QBoxLayout *lightEchLayout =createLayout(NULL,false,{SampleLightLabel,LightSampleSlider,LightSampleBox},{});
    QBoxLayout *nuageEchLayout = createLayout(NULL,false,{SampleNuageLabel,NuageSampleSlider,NuageSampleBox},{});
    QBoxLayout *groupBoxLayout = createLayout(echantillonFrame,true,{},{nuageEchLayout,lightEchLayout});
    QBoxLayout *otherLayout = createLayout(otherTab,true,{echantillonLabel,echantillonFrame,OtherLabel,otherFrame,PresetGeneralLabel,presetFrame},{});
    otherLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    tabWidget->addTab(otherTab, "Autres");
    // Add the tab widget to the main layout
    QBoxLayout * contentLayout = createLayout(contents,true,{tabWidget},{});
    contentLayout->addStretch(0);

    this->setWidget(contents);

    //bruit tab
    connect(xBBminSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &TextureDockWidget::xBBminValueChanged);
    connect(yBBminSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &TextureDockWidget::yBBminValueChanged);
    connect(zBBminSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &TextureDockWidget::zBBminValueChanged);

    connect(xBBmaxSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &TextureDockWidget::xBBmaxValueChanged);
    connect(yBBmaxSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &TextureDockWidget::yBBmaxValueChanged);
    connect(zBBmaxSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &TextureDockWidget::zBBmaxValueChanged);

    connect(xbruitworleySpinBox, QOverload<double>::of(&PowerOfTwoSpinBox::valueChanged), this, &TextureDockWidget::xResolutionBruitValueChanged);
    connect(ybruitworleySpinBox, QOverload<double>::of(&PowerOfTwoSpinBox::valueChanged), this, &TextureDockWidget::yResolutionBruitValueChanged);
    connect(zbruitworleySpinBox, QOverload<double>::of(&PowerOfTwoSpinBox::valueChanged), this, &TextureDockWidget::zResolutionBruitValueChanged);

    connect(rfreqWorleySpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &TextureDockWidget::rFreqBruitValueChanged);
    connect(gfreqWorleySpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &TextureDockWidget::gFreqBruitValueChanged);
    connect(bfreqWorleySpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &TextureDockWidget::bFreqBruitValueChanged);
    connect(afreqWorleySpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &TextureDockWidget::aFreqBruitValueChanged);

    connect(rfacteurWorleySpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &TextureDockWidget::rFacteurBruitValueChanged);
    connect(gfacteurWorleySpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &TextureDockWidget::gFacteurBruitValueChanged);
    connect(bfacteurWorleySpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &TextureDockWidget::bFacteurBruitValueChanged);
    connect(afacteurWorleySpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &TextureDockWidget::aFacteurBruitValueChanged);

    connect(xbruitCurlSpinBox, QOverload<double>::of(&PowerOfTwoSpinBox::valueChanged), this, &TextureDockWidget::xResolutionBruitCurlValueChanged);
    connect(ybruitCurlSpinBox, QOverload<double>::of(&PowerOfTwoSpinBox::valueChanged), this, &TextureDockWidget::yResolutionBruitCurlValueChanged);
    connect(rfreqCurlSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &TextureDockWidget::rFreqBruitCurlValueChanged);
    connect(gfreqCurlSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &TextureDockWidget::gFreqBruitCurlValueChanged);
    connect(bfreqCurlSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &TextureDockWidget::bFreqBruitCurlValueChanged);

    connect(buttonGroupForme, QOverload<int>::of(&QButtonGroup::buttonClicked), this, &TextureDockWidget::formeSelectionChanged);

    connect(attenuationSlider, &QSlider::valueChanged, this, &TextureDockWidget::attenuationSliderChangedSlot);
    connect(rSphereSlider, &QSlider::valueChanged, this, &TextureDockWidget::rSphereSliderChangedSlot);
    connect(rITorusSlider, &QSlider::valueChanged, this, &TextureDockWidget::rITorusSliderChangedSlot);
    connect(rETorusSlider, &QSlider::valueChanged, this, &TextureDockWidget::rETorusSliderChangedSlot);
    connect(lCapsuleSlider, &QSlider::valueChanged, this, &TextureDockWidget::lCapsuleSliderChangedSlot);
    connect(rCapsuleSlider, &QSlider::valueChanged, this, &TextureDockWidget::rCapsuleSliderChangedSlot);
    connect(tBarreCroixSlider, &QSlider::valueChanged, this, &TextureDockWidget::tBarreCroixSliderChangedSlot);

    //light tab
    connect(redColorNuageSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &TextureDockWidget::redNuageValueChanged);
    connect(greenColorNuageSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &TextureDockWidget::greenNuageValueChanged);
    connect(blueColorNuageSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &TextureDockWidget::blueNuageValueChanged);

    connect(absorptionSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &TextureDockWidget::absorptionValueChanged);
    connect(absorptionSlider, &QSlider::valueChanged, this, &TextureDockWidget::absorptionSliderChangedSlot);
    // Make the two-way connections
    connect(absorptionSlider, &QSlider::valueChanged, this, [this](int value) {
        absorptionSpinBox->setValue(value / 100.0);
    });
    connect(absorptionSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, [this](double value) {
        absorptionSlider->setValue(static_cast<int>(value * 100));
    });

    connect(LightPosX, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &TextureDockWidget::setlightposXValueChanged);
    connect(LightPosY, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &TextureDockWidget::setlightposYValueChanged);
    connect(LightPosZ, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &TextureDockWidget::setlightposZValueChanged);

    connect(LightColorR, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &TextureDockWidget::setlightcolRValueChanged);
    connect(LightColorG, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &TextureDockWidget::setlightcolGValueChanged);
    connect(LightColorB, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &TextureDockWidget::setlightcolBValueChanged);

    connect(rayonSoleilSlider, &QSlider::valueChanged, this, &TextureDockWidget::rayonSoleilSliderChangedSlot);

    connect(absorptionLightSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &TextureDockWidget::absorptionLightValueChanged);
    connect(absorptionLightSlider, &QSlider::valueChanged, this, &TextureDockWidget::absorptionLightSliderChangedSlot);
    // Make the two-way connections
    connect(absorptionLightSlider, &QSlider::valueChanged, this, [this](int value) {
        absorptionLightSpinBox->setValue(value / 100.0);
    });
    connect(absorptionLightSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, [this](double value) {
        absorptionLightSlider->setValue(static_cast<int>(value * 100));
    });

    //other tab
    connect(LightSampleSlider, &QSlider::valueChanged, LightSampleBox, &QSpinBox::setValue);
    connect(LightSampleBox, QOverload<int>::of(&QSpinBox::valueChanged), LightSampleSlider, &QSlider::setValue);
    connect(NuageSampleSlider, &QSlider::valueChanged, NuageSampleBox, &QSpinBox::setValue);
    connect(NuageSampleBox, QOverload<int>::of(&QSpinBox::valueChanged), NuageSampleSlider, &QSlider::setValue);
    connect(NuageSampleSlider, &QSlider::valueChanged, this, &TextureDockWidget::onNuageSliderChanged);
    connect(NuageSampleBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &TextureDockWidget::onNuageSpinBoxChanged);
    connect(LightSampleSlider, &QSlider::valueChanged, this, &TextureDockWidget::onLightSliderChanged);
    connect(LightSampleBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &TextureDockWidget::onLightSpinBoxChanged);

    connect(PresetButton1, &QPushButton::clicked, this, &TextureDockWidget::presetButton1ClickedSlot);
    connect(PresetButton2, &QPushButton::clicked, this, &TextureDockWidget::presetButton2ClickedSlot);
    connect(PresetButton3, &QPushButton::clicked, this, &TextureDockWidget::presetButton3ClickedSlot);
    connect(PresetButton4, &QPushButton::clicked, this, &TextureDockWidget::presetButton4ClickedSlot);
    connect(PresetButton5, &QPushButton::clicked, this, &TextureDockWidget::presetButton5ClickedSlot);
    connect(PresetButton6, &QPushButton::clicked, this, &TextureDockWidget::presetButton6ClickedSlot);

    connect(vitesseSlider, &QSlider::valueChanged, this, &TextureDockWidget::vitesseSliderChangedSlot);

}

///bruit

void TextureDockWidget::xBBminSpinBoxChangedSlot(float value){emit xBBminValueChanged(value) ;}
void TextureDockWidget::yBBminSpinBoxChangedSlot(float value){emit yBBminValueChanged(value) ;}
void TextureDockWidget::zBBminSpinBoxChangedSlot(float value){emit zBBminValueChanged(value);}

void TextureDockWidget::xBBmaxSpinBoxChangedSlot(float value){emit xBBmaxValueChanged(value) ;}
void TextureDockWidget::yBBmaxSpinBoxChangedSlot(float value){emit yBBmaxValueChanged(value) ;}
void TextureDockWidget::zBBmaxSpinBoxChangedSlot(float value){emit zBBmaxValueChanged(value);}

void TextureDockWidget::xResolutionBruitSpinBoxChangedSlot(float value){emit xResolutionBruitValueChanged(value) ;}
void TextureDockWidget::yResolutionBruitSpinBoxChangedSlot(float value){emit yResolutionBruitValueChanged(value) ;}
void TextureDockWidget::zResolutionBruitSpinBoxChangedSlot(float value){emit zResolutionBruitValueChanged(value);}

void TextureDockWidget::rFreqBruitSpinBoxChangedSlot(float value){emit rFreqBruitValueChanged(value) ;}
void TextureDockWidget::gFreqBruitSpinBoxChangedSlot(float value){emit gFreqBruitValueChanged(value) ;}
void TextureDockWidget::bFreqBruitSpinBoxChangedSlot(float value){emit bFreqBruitValueChanged(value);}
void TextureDockWidget::aFreqBruitSpinBoxChangedSlot(float value){emit aFreqBruitValueChanged(value) ;}

void TextureDockWidget::rFacteurBruitSpinBoxChangedSlot(float value){emit rFacteurBruitValueChanged(value) ;}
void TextureDockWidget::gFacteurBruitSpinBoxChangedSlot(float value){emit gFacteurBruitValueChanged(value) ;}
void TextureDockWidget::bFacteurBruitSpinBoxChangedSlot(float value){emit bFacteurBruitValueChanged(value);}
void TextureDockWidget::aFacteurBruitSpinBoxChangedSlot(float value){emit aFacteurBruitValueChanged(value) ;}

void TextureDockWidget::xResolutionBruitCurlSpinBoxChangedSlot(float value){emit xResolutionBruitCurlValueChanged(value) ;}
void TextureDockWidget::yResolutionBruitCurlSpinBoxChangedSlot(float value){emit yResolutionBruitCurlValueChanged(value) ;}
void TextureDockWidget::rFreqBruitCurlSpinBoxChangedSlot(float value){emit rFreqBruitCurlValueChanged(value) ;}
void TextureDockWidget::gFreqBruitCurlSpinBoxChangedSlot(float value){emit gFreqBruitCurlValueChanged(value) ;}
void TextureDockWidget::bFreqBruitCurlSpinBoxChangedSlot(float value){emit bFreqBruitCurlValueChanged(value);}

void TextureDockWidget::formeSelectionChangedSlot(int id){emit formeSelectionChanged(id);}

void TextureDockWidget::attenuationSliderChangedSlot(float value){emit attenuationSliderChanged(value/100.0) ;}
void TextureDockWidget::rSphereSliderChangedSlot(float value){emit rSphereSliderChanged(value/100.0) ;}
void TextureDockWidget::rITorusSliderChangedSlot(float value){emit rITorusSliderChanged(value/100.0) ;}
void TextureDockWidget::rETorusSliderChangedSlot(float value){emit rETorusSliderChanged(value/100.0) ;}
void TextureDockWidget::lCapsuleSliderChangedSlot(float value){emit lCapsuleSliderChanged(value/100.0) ;}
void TextureDockWidget::rCapsuleSliderChangedSlot(float value){emit rCapsuleSliderChanged(value/100.0) ;}
void TextureDockWidget::tBarreCroixSliderChangedSlot(float value){emit tBarreCroixSliderChanged(value/100.0) ;}

///Light
void TextureDockWidget::redNuageSpinBoxChangedSlot(float value){emit redNuageValueChanged(value) ;}
void TextureDockWidget::greenNuageSpinBoxChangedSlot(float value){emit greenNuageValueChanged(value) ;}
void TextureDockWidget::blueNuageSpinBoxChangedSlot(float value){emit blueNuageValueChanged(value);}

void TextureDockWidget::absorptionSpinBoxChangedSlot(float value){emit absorptionValueChanged(value) ;}
void TextureDockWidget::absorptionSliderChangedSlot(int i){emit absorptionValueChanged((float) i/(float)sliderAbsorptionMax);}

void TextureDockWidget::setlightposXSlot(float value){emit setlightposXValueChanged(value) ;}
void TextureDockWidget::setlightposYSlot(float value){emit setlightposYValueChanged(value) ;}
void TextureDockWidget::setlightposZSlot(float value){emit setlightposZValueChanged(value);}

void TextureDockWidget::setlightcolRSlot(float value){emit setlightcolRValueChanged(value) ;}
void TextureDockWidget::setlightcolGSlot(float value){emit setlightcolGValueChanged(value) ;}
void TextureDockWidget::setlightcolBSlot(float value){emit setlightcolBValueChanged(value);}

void TextureDockWidget::rayonSoleilSliderChangedSlot(int value) {emit rayonSoleilSliderChanged(value/10.0);}
void TextureDockWidget::vitesseSliderChangedSlot(int value) {emit vitesseSliderChanged(float(value));}

void TextureDockWidget::absorptionLightSpinBoxChangedSlot(float value){emit absorptionLightValueChanged(value) ;}
void TextureDockWidget::absorptionLightSliderChangedSlot(int i){emit absorptionLightValueChanged((float) i/(float)sliderAbsorptionMax);}

/// Ohter
void TextureDockWidget::onNuageSliderChangedSlot(int value) {emit onNuageSliderChanged(value);}
void TextureDockWidget::onNuageSpinBoxChangedSlot(int value){emit onNuageSpinBoxChanged(value);}

void TextureDockWidget::onLightSliderChangedSlot(int value){emit onLightSliderChanged(value);}
void TextureDockWidget::onLightSpinBoxChangedSlot(int value){emit onLightSpinBoxChanged(value);}

void TextureDockWidget::presetButton1ClickedSlot(){
    emit (setboolCameraOrbital(true)); //la camera devient orbital autour (0,0,0)
    // Résolutions du bruit
    xBBminSpinBox->setValue(-5.0f);
    yBBminSpinBox->setValue(-5.0f);
    zBBminSpinBox->setValue(-5.0f);

    xBBmaxSpinBox->setValue(5.0f);
    yBBmaxSpinBox->setValue(5.0f);
    zBBmaxSpinBox->setValue(5.0f);

    //ATTENTION DONNEE LA PUISSANCE DE DEUX PAS LA VALEUR DIRECT EX 2^7=128 donc donne 7
    xbruitworleySpinBox->setValue(7.0f);
    ybruitworleySpinBox->setValue(7.0f);
    zbruitworleySpinBox->setValue(7.0f);

    // Fréquences du bruit
    rfreqWorleySpinBox->setValue(2.0f);
    gfreqWorleySpinBox->setValue(6.0f);
    bfreqWorleySpinBox->setValue(12.0f);
    afreqWorleySpinBox->setValue(24.0f);

    // Facteurs du bruit
    rfacteurWorleySpinBox->setValue(-3.0f);
    gfacteurWorleySpinBox->setValue(0.33f);
    bfacteurWorleySpinBox->setValue(0.33f);
    afacteurWorleySpinBox->setValue(0.33f);

    // Résolutions du bruit Curl (puissance de 2)
    xbruitCurlSpinBox->setValue(7.0f);
    ybruitCurlSpinBox->setValue(7.0f);

    // Fréquences du bruit Curl
    rfreqCurlSpinBox->setValue(0.8f);
    gfreqCurlSpinBox->setValue(0.8f);
    bfreqCurlSpinBox->setValue(0.8f);

    // Lumière - Nuages
    redColorNuageSpinBox->setValue(1.0f);
    greenColorNuageSpinBox->setValue(1.0f);
    blueColorNuageSpinBox->setValue(1.0f);

    absorptionSpinBox->setValue(9.0f);
    absorptionLightSpinBox->setValue(1.0f);

    // Position et couleur de la lumière
    LightPosX->setValue(0.0f);
    LightPosY->setValue(5.0f);
    LightPosZ->setValue(5.0f);

    LightColorR->setValue(1.0f);
    LightColorG->setValue(1.0f);
    LightColorB->setValue(1.0f);

    // Soleil
    rayonSoleilSlider->setValue(1.0f);

    // Autres paramètres
    NuageSampleBox->setValue(50);

    LightSampleBox->setValue(20);

    vitesseSlider->setValue(5000);
    emit(setboolOnlyCloud(true));
    emit(setIsLightUtime(false));
    emit(setcamerapos(QVector3D(0 , 0 , -20) , QVector3D  (0 , 0 , 0)));


}

void TextureDockWidget::presetButton2ClickedSlot(){
    emit (setboolCameraOrbital(true)); //la camera devient orbital autour (0,0,0)
    // Résolutions du bruit

    xBBminSpinBox->setValue(-5.0f);
    yBBminSpinBox->setValue(-5.0f);
    zBBminSpinBox->setValue(-5.0f);

    xBBmaxSpinBox->setValue(5.0f);
    yBBmaxSpinBox->setValue(5.0f);
    zBBmaxSpinBox->setValue(5.0f);
    //ATTENTION DONNEE LA PUISSANCE DE DEUX PAS LA VALEUR DIRECT EX 2^7=128 donc donne 7
    xbruitworleySpinBox->setValue(5.0f);
    ybruitworleySpinBox->setValue(5.0f);
    zbruitworleySpinBox->setValue(5.0f);

    // Fréquences du bruit
    rfreqWorleySpinBox->setValue(2.0f);
    gfreqWorleySpinBox->setValue(6.0f);
    bfreqWorleySpinBox->setValue(12.0f);
    afreqWorleySpinBox->setValue(24.0f);

    // Facteurs du bruit
    rfacteurWorleySpinBox->setValue(1.0f);
    gfacteurWorleySpinBox->setValue(-0.33f);
    bfacteurWorleySpinBox->setValue(-0.33f);
    afacteurWorleySpinBox->setValue(-0.33f);

    // Résolutions du bruit Curl (puissance de 2)
    xbruitCurlSpinBox->setValue(7.0f);
    ybruitCurlSpinBox->setValue(7.0f);

    // Fréquences du bruit Curl
    rfreqCurlSpinBox->setValue(0.8f);
    gfreqCurlSpinBox->setValue(0.8f);
    bfreqCurlSpinBox->setValue(0.8f);

    // Lumière - Nuages
    redColorNuageSpinBox->setValue(1.0f);
    greenColorNuageSpinBox->setValue(1.0f);
    blueColorNuageSpinBox->setValue(1.0f);

    absorptionSpinBox->setValue(9.0f);
    absorptionLightSpinBox->setValue(1.0f);

    // Position et couleur de la lumière
    LightPosX->setValue(0.0f);
    LightPosY->setValue(0.0f);
    LightPosZ->setValue(0.0f);

    LightColorR->setValue(1.0f);
    LightColorG->setValue(1.0f);
    LightColorB->setValue(1.0f);

    // Soleil
    rayonSoleilSlider->setValue(1.0f);

    // Autres paramètres
    NuageSampleBox->setValue(50);

    LightSampleBox->setValue(20);

    vitesseSlider->setValue(1000);
    emit(setboolOnlyCloud(true));
    emit(setIsLightUtime(false));
    emit(setcamerapos(QVector3D(0 , 0 , -20) , QVector3D  (0 , 0 , 0)));
}

void TextureDockWidget::presetButton3ClickedSlot()
{
    xBBminSpinBox->setValue(-7.0f);
    yBBminSpinBox->setValue(1.0f);
    zBBminSpinBox->setValue(-7.0f);

    xBBmaxSpinBox->setValue(7.0f);
    yBBmaxSpinBox->setValue(11.0f);
    zBBmaxSpinBox->setValue(7.0f);

    LightPosX->setValue(0.0f);
    LightPosY->setValue(80.0f);
    LightPosZ->setValue(-30.0f);

    NuageSampleBox->setValue(50);

    LightSampleBox->setValue(20);

    emit (setboolCameraOrbital(true));
    emit(setboolOnlyCloud(false));
    emit(setIsLightUtime(true));
    emit(setcamerapos(QVector3D(-15 , 5 , -10) , QVector3D  (-2 , 4 , 0) ) );
}
void TextureDockWidget::presetButton4ClickedSlot()
{
    xBBminSpinBox->setValue(-7.0f);
    yBBminSpinBox->setValue(0.0f);
    zBBminSpinBox->setValue(-7.0f);

    xBBmaxSpinBox->setValue(7.0f);
    yBBmaxSpinBox->setValue(11.0f);
    zBBmaxSpinBox->setValue(7.0f);

    LightPosX->setValue(0.0f);
    LightPosY->setValue(80.0f);
    LightPosZ->setValue(-30.0f);

    NuageSampleBox->setValue(50);

    LightSampleBox->setValue(20);
    emit (setboolCameraOrbital(false));
    emit(setboolOnlyCloud(false));
    emit(setIsLightUtime(true));
    emit(setcamerapos(QVector3D(-2 , 1 , -3) , QVector3D  (-1 , 1 , 0) ) );
}
void TextureDockWidget::presetButton5ClickedSlot(){
     emit (setboolCameraOrbital(true)); //la camera devient orbital autour (0,0,0)
    // Résolutions du bruit

    xBBminSpinBox->setValue(-25.0f);
    yBBminSpinBox->setValue(-25.0f);
    zBBminSpinBox->setValue(-25.0f);

    xBBmaxSpinBox->setValue(25.0f);
    yBBmaxSpinBox->setValue(25.0f);
    zBBmaxSpinBox->setValue(25.0f);
    //ATTENTION DONNEE LA PUISSANCE DE DEUX PAS LA VALEUR DIRECT EX 2^7=128 donc donne 7
    xbruitworleySpinBox->setValue(7.0f);
    ybruitworleySpinBox->setValue(7.0f);
    zbruitworleySpinBox->setValue(7.0f);

    // Fréquences du bruit
    rfreqWorleySpinBox->setValue(2.0f);
    gfreqWorleySpinBox->setValue(6.0f);
    bfreqWorleySpinBox->setValue(12.0f);
    afreqWorleySpinBox->setValue(24.0f);

    // Facteurs du bruit
    rfacteurWorleySpinBox->setValue(-2.0f);
    gfacteurWorleySpinBox->setValue(1.33f);
    bfacteurWorleySpinBox->setValue(1.33f);
    afacteurWorleySpinBox->setValue(1.33f);

    // Résolutions du bruit Curl (puissance de 2)
    xbruitCurlSpinBox->setValue(7.0f);
    ybruitCurlSpinBox->setValue(7.0f);

    // Fréquences du bruit Curl
    rfreqCurlSpinBox->setValue(0.8f);
    gfreqCurlSpinBox->setValue(0.8f);
    bfreqCurlSpinBox->setValue(0.8f);

    // Lumière - Nuages
    redColorNuageSpinBox->setValue(1.0f);
    greenColorNuageSpinBox->setValue(1.0f);
    blueColorNuageSpinBox->setValue(1.0f);

    absorptionSpinBox->setValue(9.0f);
    absorptionLightSpinBox->setValue(1.0f);

    // Position et couleur de la lumière
    LightPosX->setValue(0.0f);
    LightPosY->setValue(50.0f);
    LightPosZ->setValue(-50.0f);

    LightColorR->setValue(1.0f);
    LightColorG->setValue(1.0f);
    LightColorB->setValue(1.0f);

    // Soleil
    rayonSoleilSlider->setValue(1.0f);

    // Autres paramètres
    NuageSampleBox->setValue(80);

    LightSampleBox->setValue(20);

    vitesseSlider->setValue(7000);
    emit(setboolOnlyCloud(true));
    emit(setIsLightUtime(false));
}
void TextureDockWidget::presetButton6ClickedSlot(){
    emit (setboolCameraOrbital(true)); //la camera devient orbital autour (0,0,0)
    // Résolutions du bruit

    xBBminSpinBox->setValue(-25.0f);
    yBBminSpinBox->setValue(-25.0f);
    zBBminSpinBox->setValue(-25.0f);

    xBBmaxSpinBox->setValue(25.0f);
    yBBmaxSpinBox->setValue(25.0f);
    zBBmaxSpinBox->setValue(25.0f);
    //ATTENTION DONNEE LA PUISSANCE DE DEUX PAS LA VALEUR DIRECT EX 2^7=128 donc donne 7
    xbruitworleySpinBox->setValue(7.0f);
    ybruitworleySpinBox->setValue(7.0f);
    zbruitworleySpinBox->setValue(7.0f);

    // Fréquences du bruit
    rfreqWorleySpinBox->setValue(2.0f);
    gfreqWorleySpinBox->setValue(6.0f);
    bfreqWorleySpinBox->setValue(12.0f);
    afreqWorleySpinBox->setValue(24.0f);

    // Facteurs du bruit
    rfacteurWorleySpinBox->setValue(5.0f);
    gfacteurWorleySpinBox->setValue(-1.33f);
    bfacteurWorleySpinBox->setValue(2.0f);
    afacteurWorleySpinBox->setValue(-0.9f);

    // Résolutions du bruit Curl (puissance de 2)
    xbruitCurlSpinBox->setValue(7.0f);
    ybruitCurlSpinBox->setValue(7.0f);

    // Fréquences du bruit Curl
    rfreqCurlSpinBox->setValue(0.8f);
    gfreqCurlSpinBox->setValue(0.8f);
    bfreqCurlSpinBox->setValue(0.8f);

    // Lumière - Nuages
    redColorNuageSpinBox->setValue(1.0f);
    greenColorNuageSpinBox->setValue(1.0f);
    blueColorNuageSpinBox->setValue(1.0f);

    absorptionSpinBox->setValue(9.0f);
    absorptionLightSpinBox->setValue(1.0f);

    // Position et couleur de la lumière
    LightPosX->setValue(0.0f);
    LightPosY->setValue(50.0f);
    LightPosZ->setValue(-50.0f);

    LightColorR->setValue(1.0f);
    LightColorG->setValue(1.0f);
    LightColorB->setValue(1.0f);

    // Soleil
    rayonSoleilSlider->setValue(1.0f);

    // Autres paramètres
    NuageSampleBox->setValue(100);

    LightSampleBox->setValue(33);

    vitesseSlider->setValue(4500);
    emit(setboolOnlyCloud(true));
    emit(setIsLightUtime(false));
}
