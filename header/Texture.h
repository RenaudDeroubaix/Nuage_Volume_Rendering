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

    QVector3D BBmin;
    QVector3D BBmax;

    double absorptionNuage;
    QVector3D couleurNuage;
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

    void build(const std::vector<unsigned char> & data, const std::vector<unsigned char> & labesl,
               unsigned int & nx , unsigned int & ny , unsigned int & nz,
               float & dx , float & dy , float & dz,
               std::map<unsigned char, QColor> & colorMap );

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
    void recompileShaders();

};

#endif // TEXTURE_H
