#ifndef MESH_H
#define MESH_H

#include <QVector>
#include <QVector3D>

namespace MeshTools
{
    QVector<int> buildTriangleVertexIndices(QVector<int> &polygonVertexIndices, QVector<int> &startPolygon);
    QVector<float> packTriangleVertexCoords(QVector<QVector3D> &vertices, QVector<int> &polygonTriangleVertexIndices);
    QVector<float> buildAndPackTriangleNormalsCoords(QVector<QVector3D> &vertices, QVector<int> &polygonTriangleVertexIndices);
    bool getBoundingBox(QVector<QVector3D> &vertices, QVector3D &maxXYZ, QVector3D &minXYZ);
    float getRadiusBoundingBox(QVector3D maxXYZ, QVector3D minXYZ);
    QVector3D getCentreBoundingBox(QVector3D maxXYZ, QVector3D minXYZ);
    QVector<float> pack2DLinesTo3D(QVector<QVector2D> lines, int zeroCoordIndex);
    QVector3D makeVector3D(QVector2D point, int zeroCoordIndex);
    QVector<QVector3D> createNormalsForVertices(const QVector<QVector3D> &vertices, const QVector<int> &polygonTriangleVertexIndices);
}

#endif // MESH_H
