#ifndef LIGHT_H
#define LIGHT_H

#include "GLFunc.h"
#include <QString>
#include <QColor>
#include <QVector>
#include <QVector3D>
#include <QVector4D>
#include <QGLViewer/vec.h>
#include <QOpenGLContext>
#include <QOpenGLExtraFunctions>
#include <QGLViewer/camera.h>

class Light
{
    float rayon;

public:

    Light( QOpenGLContext* context);

    ~Light();
    void init();
    void initLightShader();

    void draw(const qglviewer::Camera * camera );
    void drawSquareForLight();
    QVector3D LightPos;
    QVector3D LightColor;
    GLuint  vShader, fShader , LightID;
    QOpenGLContext* glContext;
    QOpenGLExtraFunctions* glFunctions;

    QVector3D & getpos() {return LightPos;}
    QVector3D & getcol() {return LightColor;}

    void setXlightposDisplay(float _x);
    void setYlightposDisplay(float _y);
    void setZlightposDisplay(float _z);
    void setRlightcolDisplay(float _r);
    void setGlightcolDisplay(float _g);
    void setBlightcolDisplay(float _b);

};

#endif // LIGHT_H
