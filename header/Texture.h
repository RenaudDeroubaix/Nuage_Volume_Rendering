#ifndef TEXTURE_H
#define TEXTURE_H

#include "Vec3D.h"
#include "GLFunc.h"
#include <QString>
#include <QColor>
#include <QVector>
#include <QVector3D>
#include <QVector4D>
#include <QGLViewer/vec.h>
#include <QOpenGLContext>
#include <QGLViewer/camera.h>
#include <QElapsedTimer>

struct Plan{

    QVector3D point;
    QVector3D normale;
    QVector3D up_vect;
    QVector3D right_vect;
    Plan( QVector3D p ,QVector3D n , QVector3D up , QVector3D right )
    {
        point = p;
        normale = n;
        up_vect = up;
        right_vect = right;
    }

};

class Texture
{

public:
    Texture( QOpenGLContext* context);

    ~Texture();
private :

    GLuint textureId = 0;

    unsigned char * rgbTexture;

    QVector3D sizeTexture3D; //longueur largeur profondeur
    QVector3D resolutionBruit;
    QVector4D freqBruit;

    double xMax;
    double yMax;
    double zMax;

    QElapsedTimer timer;

    QVector3D BBmin;
    QVector3D BBmax;

    double absorptionNuage;
    QVector3D couleurNuage;

    int LightEch;
    int NuageEch;

    QVector<Plan> plans;

    bool textureCreated;

public:

    void init();
    void clear();

    void initGLSL();
    void initTexture();
    void computePass();
    void deleteTexture();

    void draw( QVector3D & LightPos ,  QVector3D & LightCol  ,  const qglviewer::Camera * camera );
    void drawCube();

    GLuint getTextureId(){return textureId;}

    GLuint cShader, vShader, gShader, fShader, programID , computeID;

    QOpenGLContext* glContext;
    QOpenGLExtraFunctions* glFunctions;




public slots:
    void setNuageEch(int value);
    void setLightEch(int value);

    void setRedNuageDisplay(float _r);
    void setGreenNuageDisplay(float _g);
    void setBlueNuageDisplay(float _b);

    void setAbsorptionNuageDisplay(float _a);
    void recompileShaders();
    void setResolutionBruitX(float _x);
    void setResolutionBruitY(float _y);
    void setResolutionBruitZ(float _z);
    void setFreqBruitR(float _r);
    void setFreqBruitG(float _g);
    void setFreqBruitB(float _b);
    void setFreqBruitA(float _a);
};

#endif // TEXTURE_H
