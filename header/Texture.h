#ifndef TEXTURE_H
#define TEXTURE_H

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

    GLuint textureId = 0, textureId_2 = 0;

    unsigned char * rgbTexture;

    QVector3D sizeTexture3D; //longueur largeur profondeur
    QVector3D resolutionBruit;
    QVector4D freqBruit;
    QVector4D facteurBruit;

    QVector2D resolutionBruitCurl;
    QVector3D freqBruitCurl;

    double xMax;
    double yMax;
    double zMax;

    QElapsedTimer timer;
    float vitesse;

    QVector3D BBmin;
    QVector3D BBmax;

    double absorptionNuage;
    double absorptionLight;
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
    void updateTextureData();
    void computePass();
    void deleteTexture();

    void draw( QVector3D & LightPos ,  QVector3D & LightCol  ,  const qglviewer::Camera * camera );
    void drawPlaneInFrontOfCamera(const qglviewer::Camera *camera, float distance);
    void drawCube();

    GLuint getTextureId(){return textureId;}

    GLuint cShader3D, cShader2D, vShader, gShader, fShader, programID , computeID, computeID_tex2D;

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
    void setAbsorptionLightDisplay(float _a);
    void setResolutionBruitCurlX(float _x);
    void setResolutionBruitCurlY(float _y);
    void setFreqBruitCurlR(float _r);
    void setFreqBruitCurlG(float _g);
    void setFreqBruitCurlB(float _b);
    void setFacteurBruitR(float _r);
    void setFacteurBruitG(float _g);
    void setFacteurBruitB(float _b);
    void setFacteurBruitA(float _a);
    void setVitesse( float _v);

};

#endif // TEXTURE_H
