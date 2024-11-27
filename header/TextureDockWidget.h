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

    QDoubleSpinBox *absorptionLightSpinBox;
    QSlider *absorptionLightSlider;

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

    PowerOfTwoSpinBox *xbruitCurlSpinBox;
    PowerOfTwoSpinBox *ybruitCurlSpinBox;
    QDoubleSpinBox *rfreqCurlSpinBox;
    QDoubleSpinBox *gfreqCurlSpinBox;
    QDoubleSpinBox *bfreqCurlSpinBox;

    QDoubleSpinBox *rfacteurWorleySpinBox;
    QDoubleSpinBox *gfacteurWorleySpinBox;
    QDoubleSpinBox *bfacteurWorleySpinBox;
    QDoubleSpinBox *afacteurWorleySpinBox;

private slots:
///bruit
    void xResolutionBruitSpinBoxChangedSlot(float value);
    void yResolutionBruitSpinBoxChangedSlot(float value);
    void zResolutionBruitSpinBoxChangedSlot(float value);

    void rFreqBruitSpinBoxChangedSlot(float value);
    void gFreqBruitSpinBoxChangedSlot(float value);
    void bFreqBruitSpinBoxChangedSlot(float value);
    void aFreqBruitSpinBoxChangedSlot(float value);

    void rFacteurBruitSpinBoxChangedSlot(float value);
    void gFacteurBruitSpinBoxChangedSlot(float value);
    void bFacteurBruitSpinBoxChangedSlot(float value);
    void aFacteurBruitSpinBoxChangedSlot(float value);

    void xResolutionBruitCurlSpinBoxChangedSlot(float value);
    void yResolutionBruitCurlSpinBoxChangedSlot(float value);
    void rFreqBruitCurlSpinBoxChangedSlot(float value);
    void gFreqBruitCurlSpinBoxChangedSlot(float value);
    void bFreqBruitCurlSpinBoxChangedSlot(float value);

///Light
    void redNuageSpinBoxChangedSlot(float value);
    void greenNuageSpinBoxChangedSlot(float value);
    void blueNuageSpinBoxChangedSlot(float value);

    void absorptionSpinBoxChangedSlot(float value);
    void absorptionSliderChangedSlot(int i);

    void setlightposXSlot(float value);
    void setlightposYSlot(float value);
    void setlightposZSlot(float value);

    void setlightcolRSlot(float value);
    void setlightcolGSlot(float value);
    void setlightcolBSlot(float value);

    void rayonSoleilSliderChangedSlot(int value);

    void absorptionLightSpinBoxChangedSlot(float value);
    void absorptionLightSliderChangedSlot(int i);

///other
    void onNuageSliderChangedSlot(int value);
    void onNuageSpinBoxChangedSlot(int value);
    void onLightSliderChangedSlot(int value);
    void onLightSpinBoxChangedSlot(int value);


public slots:

signals:
///Bruit
    void xResolutionBruitValueChanged(float xValue);
    void yResolutionBruitValueChanged(float yValue);
    void zResolutionBruitValueChanged(float zValue);

    void rFreqBruitValueChanged(float rValue);
    void gFreqBruitValueChanged(float gValue);
    void bFreqBruitValueChanged(float bValue);
    void aFreqBruitValueChanged(float aValue);

    void rFacteurBruitValueChanged(float rValue);
    void gFacteurBruitValueChanged(float gValue);
    void bFacteurBruitValueChanged(float bValue);
    void aFacteurBruitValueChanged(float aValue);

    void xResolutionBruitCurlValueChanged(float xValue);
    void yResolutionBruitCurlValueChanged(float yValue);
    void rFreqBruitCurlValueChanged(float rValue);
    void gFreqBruitCurlValueChanged(float gValue);
    void bFreqBruitCurlValueChanged(float bValue);

///Light
    void redNuageValueChanged(float redValue);
    void greenNuageValueChanged(float greenValue);
    void blueNuageValueChanged(float blueValue);

    void absorptionValueChanged(float absorptionValue);

    void setlightposXValueChanged(float x);
    void setlightposYValueChanged(float y);
    void setlightposZValueChanged(float z);

    void setlightcolRValueChanged(float r);
    void setlightcolGValueChanged(float g);
    void setlightcolBValueChanged(float b);

    void rayonSoleilSliderChanged(int rayonValue);

    void absorptionLightValueChanged(float absorptionValue);

/// other
    void onNuageSliderChanged(int value);
    void onNuageSpinBoxChanged(int value);
    void onLightSliderChanged(int value);
    void onLightSpinBoxChanged(int value);

};

#endif // TEXTUREDOCKWINDOW_H
