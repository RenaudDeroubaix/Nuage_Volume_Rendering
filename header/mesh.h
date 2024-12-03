#ifndef MESH_H
#define MESH_H
#include <QVector>
#include <QVector2D>
#include <QVector3D>
#include <QGLViewer/qglviewer.h>
#include <QOpenGLContext>
#include "GLFunc.h"

struct Face {
    QVector<int> vertexIndices;
    QVector<int> textureIndices;
    QVector<int> normalIndices;
};

class Mesh
{
public:
    Mesh(QOpenGLContext* context);

    QVector<QVector3D> vertices;
    QVector<QVector3D> normals;
    QVector<QVector2D> textureCoords;
    QVector<Face> faces;
    GLuint textureId = 0;

    void draw(QVector3D & LightPos ,  QVector3D & LightCol, const qglviewer::Camera * camera );

    void initGLSL();
    void initializeGL();
    void initTexture(const QString &fileName);
    void deleteTexture();

   GLuint   vShader, fShader, programID, vao, vbo ;

   QOpenGLContext* glContext;
   QOpenGLExtraFunctions* glFunctions;
};

#endif // MESH_H
