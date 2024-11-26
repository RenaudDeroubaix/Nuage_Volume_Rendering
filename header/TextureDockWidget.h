#ifndef TEXTUREDOCKWINDOW_H
#define TEXTUREDOCKWINDOW_H

#include "TextureViewer.h"

#include <QCheckBox>
#include <QDockWidget>
#include <QPushButton>
#include <QWidget>
#include <QListWidget>
#include <QDoubleSpinBox>
#include "PowerOfTwoSpinBox.h"

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

    QDoubleSpinBox *rfreqWorleySpinBox;
    QDoubleSpinBox *gfreqWorleySpinBox;
    QDoubleSpinBox *bfreqWorleySpinBox;
    QDoubleSpinBox *afreqWorleySpinBox;

    QDoubleSpinBox *LightPosX;
    QDoubleSpinBox *LightPosY;
    QDoubleSpinBox *LightPosZ;

    QDoubleSpinBox *LightColorR;
    QDoubleSpinBox *LightColorG;
    QDoubleSpinBox *LightColorB;

    PowerOfTwoSpinBox *xbruitworleySpinBox;
    PowerOfTwoSpinBox *ybruitworleySpinBox;
    PowerOfTwoSpinBox *zbruitworleySpinBox;

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

    void rFreqBruitSpinBoxChangedSlot(float value);
    void gFreqBruitSpinBoxChangedSlot(float value);
    void bFreqBruitSpinBoxChangedSlot(float value);
    void aFreqBruitSpinBoxChangedSlot(float value);


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

    void rFreqBruitValueChanged(float rValue);
    void gFreqBruitValueChanged(float gValue);
    void bFreqBruitValueChanged(float bValue);
    void aFreqBruitValueChanged(float aValue);


};

#endif // TEXTUREDOCKWINDOW_H
