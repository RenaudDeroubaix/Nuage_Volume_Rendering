#ifndef SKYBOX_H
#define SKYBOX_H

#include "Vec3D.h"
#include "GLFunc.h"
#include <QString>
#include <QColor>
#include <QVector>
#include <QVector2D>
#include <QVector3D>
#include <QVector4D>
#include <QGLViewer/vec.h>
#include <QOpenGLContext>
#include <QGLViewer/camera.h>
#include <QElapsedTimer>

class SkyBox
{
private:

    QVector3D BBmin;
    QVector3D BBmax;


public:
    SkyBox( QOpenGLContext* context);

    ~SkyBox();
    void init();
    void initShader();

    void draw(QVector3D & LightPos ,  QVector3D & LightCol  , QVector3D & LightDir,  const qglviewer::Camera * camera );

    void drawCube();

    GLuint vShader, fShader , ProgramID;
    QOpenGLContext* glContext;
    QOpenGLExtraFunctions* glFunctions;



};

#endif // SKYBOX_H
