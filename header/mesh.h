#ifndef MESH_H
#define MESH_H
#include <QVector>
#include <QVector2D>
#include <QVector3D>


struct Face {
    QVector<int> vertexIndices;
    QVector<int> textureIndices;
    QVector<int> normalIndices;
};

class Mesh
{
public:
    Mesh();

    QVector<QVector3D> vertices;
    QVector<QVector3D> normals;
    QVector<QVector2D> textureCoords;
    QVector<Face> faces;

    void draw();
};

#endif // MESH_H
