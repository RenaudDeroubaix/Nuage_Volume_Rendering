#ifndef TEXTURE_H
#define TEXTURE_H

#include "Vec3D.h"

#include <QString>
#include <QColor>
#include <QVector>
#include <QVector3D>
#include <QGLViewer/vec.h>
#include <QOpenGLContext>
#include <QOpenGLExtraFunctions>
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

    unsigned int n[3];
    float d[3];
    unsigned int gridSize;

    float minD;

    QVector3D sizeTexture3D; //longueur largeur profondeur

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

    QVector3D LightPos;
    QVector3D LightColor;

    QVector<Plan> plans;


    bool textureCreated;

public:

    void init();

    void clear();

    void initTexture();
    void computePass();

    void deleteTexture();

    void draw( const qglviewer::Camera * camera );
    void drawCube();
    void drawBoundingBox(bool fill = false);


    GLuint getTextureId(){return textureId;}

    float getGridStep(){return minD;}



    bool printShaderErrors(GLuint shader);
    bool printProgramErrors(int program);
    bool checkOpenGLError();
    std::string readShaderSource(std::string filename);

    GLuint cShader, vShader, gShader, fShader, programID , computeID;
    QOpenGLContext* glContext;
    QOpenGLExtraFunctions* glFunctions;

    static void /*GLAPIENTRY */MessageCallback( GLenum source, GLenum type,
                                                GLuint id, GLenum severity,
                                                GLsizei length, const GLchar* message,
                                                const void* userParam );

    void initGLSL();

public slots:
    void setNuageEch(int value);
    void setLightEch(int value);

    void setRedNuageDisplay(float _r);
    void setGreenNuageDisplay(float _g);
    void setBlueNuageDisplay(float _b);
    void setXlightposDisplay(float _x);
    void setYlightposDisplay(float _y);
    void setZlightposDisplay(float _z);
    void setRlightcolDisplay(float _r);
    void setGlightcolDisplay(float _g);
    void setBlightcolDisplay(float _b);
    void setAbsorptionNuageDisplay(float _a);
    void recompileShaders();

};

#endif // TEXTURE_H
