#ifndef TEXTUREDOCKWINDOW_H
#define TEXTUREDOCKWINDOW_H

#include "TextureViewer.h"

#include <QCheckBox>
#include <QDockWidget>
#include <QPushButton>
#include <QWidget>
#include <QListWidget>
#include <QDoubleSpinBox>

class TextureDockWidget : public QDockWidget
{
    Q_OBJECT
    int sliderAbsorptionMax = 200;
public:
    TextureDockWidget(QWidget * parent );

    QDoubleSpinBox *redColorNuageSpinBox;
    QDoubleSpinBox *greenColorNuageSpinBox;
    QDoubleSpinBox *blueColorNuageSpinBox;
    QDoubleSpinBox *absorptionSpinBox;
    QSlider *absorptionSlider;

    QDoubleSpinBox *LightPosX;
    QDoubleSpinBox *LightPosY;
    QDoubleSpinBox *LightPosZ;

    QDoubleSpinBox *LightColorR;
    QDoubleSpinBox *LightColorG;
    QDoubleSpinBox *LightColorB;

    QDoubleSpinBox *xResolutionBruitSpinBox;
    QDoubleSpinBox *yResolutionBruitSpinBox;
    QDoubleSpinBox *zResolutionBruitSpinBox;

    QDoubleSpinBox *xbruitworleySpinBox;
    QDoubleSpinBox *ybruitworleySpinBox;
    QDoubleSpinBox *zbruitworleySpinBox;


    // QSlider *xHSlider;
    // QSlider *yHSlider;
    // QSlider *zHSlider;
    // QPushButton *invertXPushButton;
    // QPushButton *invertYPushButton;
    // QPushButton *invertZPushButton;
    // QCheckBox *displayXCut;
    // QCheckBox *displayYCut;
    // QCheckBox *displayZCut;

private slots:
    void onNuageSliderChangedSlot(int value);
    void onNuageSpinBoxChangedSlot(int value);
    void onLightSliderChangedSlot(int value);
    void onLightSpinBoxChangedSlot(int value);

    void redNuageSpinBoxChangedSlot(float value);
    void greenNuageSpinBoxChangedSlot(float value);
    void blueNuageSpinBoxChangedSlot(float value);

    void setlightposXSlot(float value);
    void setlightposYSlot(float value);
    void setlightposZSlot(float value);

    void setlightcolRSlot(float value);
    void setlightcolGSlot(float value);
    void setlightcolBSlot(float value);

    void absorptionSpinBoxChangedSlot(float value);
    void absorptionSliderChangedSlot(int i);

    void xResolutionBruitSpinBoxChangedSlot(float value);
    void yResolutionBruitSpinBoxChangedSlot(float value);
    void zResolutionBruitSpinBoxChangedSlot(float value);

//    void xSliderChangedSlot(int i);
//    void ySliderChangedSlot(int i);
//    void zSliderChangedSlot(int i);
//    void xInvertPlaneSlot();
//    void yInvertPlaneSlot();
//    void zInvertPlaneSlot();
//    void xDisplaySlot(bool v);
//    void yDisplaySlot(bool v);
//    void zDisplaySlot(bool v);



public slots:
//    void setMaxCutPlanes(int _xMax,int _yMax,int _zMax);

signals:

    void onNuageSliderChanged(int value);
    void onNuageSpinBoxChanged(int value);
    void onLightSliderChanged(int value);
    void onLightSpinBoxChanged(int value);

    void redNuageValueChanged(float redValue);
    void greenNuageValueChanged(float greenValue);
    void blueNuageValueChanged(float blueValue);

    void setlightposXValueChanged(float x);
    void setlightposYValueChanged(float y);
    void setlightposZValueChanged(float z);

    void setlightcolRValueChanged(float r);
    void setlightcolGValueChanged(float g);
    void setlightcolBValueChanged(float b);


    void absorptionValueChanged(float absorptionValue);

    void xResolutionBruitValueChanged(float xValue);
    void yResolutionBruitValueChanged(float yValue);
    void zResolutionBruitValueChanged(float zValue);
//    void xValueChanged(float i);
//    void yValueChanged(float i);
//    void zValueChanged(float i);

//    void xInvert();
//    void yInvert();
//    void zInvert();

//    void xDisplay(bool v);
//    void yDisplay(bool v);
//    void zDisplay(bool v);

};

#endif // TEXTUREDOCKWINDOW_H
