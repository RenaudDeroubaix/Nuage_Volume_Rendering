#ifndef TEXTUREVIEWER_H
#define TEXTUREVIEWER_H

#include <QKeyEvent>
#include <QGLViewer/qglviewer.h>
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <algorithm>
#include "light.h"
#include "Texture.h"
#include "skybox.h"

class TextureViewer : public QGLViewer
{
    Q_OBJECT

public :
    TextureViewer(QWidget *parent = nullptr);

    //void open3DImage(const QString & fileName);
    void openOffMesh(const QString & fileName);



protected :
    Texture * texture;
    Light * light;
    //Mesh * mesh;
    SkyBox * skybox;
    virtual void draw();

    virtual void init();

    virtual QString helpString() const;
    virtual void keyPressEvent(QKeyEvent *e);

    void drawMesh();
    void clear();
    void updateCamera(const qglviewer::Vec & center, float radius);

    std::vector<qglviewer::Vec> vertices;
    std::vector<std::array<size_t,3>> triangles;


public slots:
    void onNuageSliderChanged(int value);
    void onNuageSpinBoxChanged(int value);
    void onLightSliderChanged(int value);
    void onLightSpinBoxChanged(int value);
    void setRedNuage(float _r);
    void setGreenNuage(float _g);
    void setBlueNuage(float _b);
    void setXlightpos(float _x);
    void setYlightpos(float _y);
    void setZlightpos(float _z);
    void setRlightcol(float _r);
    void setGlightcol(float _g);
    void setBlightcol(float _b);
    void setAbsorptionNuage(float _a);
    void recompileShaders();
    void setResolutionBruitX(float _x);
    void setResolutionBruitY(float _y);
    void setResolutionBruitZ(float _z);
    void setFreqBruitR(float _r);
    void setFreqBruitG(float _g);
    void setFreqBruitB(float _b);
    void setFreqBruitA(float _a);
    void setRayonSoleil(float _rayonValue);
    void setAbsorptionLight(float _a);
    void setResolutionBruitCurlX(float _x);
    void setResolutionBruitCurlY(float _y);
    void setFreqBruitCurlR(float _r);
    void setFreqBruitCurlG(float _g);
    void setFreqBruitCurlB(float _b);
    void setFacteurBruitR(float _r);
    void setFacteurBruitG(float _g);
    void setFacteurBruitB(float _b);
    void setFacteurBruitA(float _a);
    void setVitesse(float _v);

signals:

};

#endif
