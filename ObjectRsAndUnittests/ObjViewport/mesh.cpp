#include "mesh.h"

QVector<int> MeshTools::buildTriangleVertexIndices(QVector<int> &polygonVertexIndices, QVector<int> &startPolygon)
{
    QVector<int> triangleIndices;
    for(int indVertexStart = 0; indVertexStart < startPolygon.size() - 1; ++indVertexStart)
    {
        int startVertexIndex = startPolygon[indVertexStart];
        int polygonVertexCount = startPolygon[indVertexStart + 1] - startVertexIndex;
        triangleIndices.push_back(polygonVertexIndices[startVertexIndex] - 1);
        triangleIndices.push_back(polygonVertexIndices[startVertexIndex + 1] - 1);
        triangleIndices.push_back(polygonVertexIndices[startVertexIndex + 2] - 1);
        if(polygonVertexCount > 3)
        {
            for(int i = 1; i <= polygonVertexCount - 3; ++i)
            {
                triangleIndices.push_back(polygonVertexIndices[startVertexIndex] - 1);
                triangleIndices.push_back(polygonVertexIndices[startVertexIndex + 1] - 1);
                triangleIndices.push_back(polygonVertexIndices[startVertexIndex + 2 + i] - 1);
            }
        }
    }
    return triangleIndices;
}

QVector<float> MeshTools::packTriangleVertexCoords(QVector<QVector3D> &vertices, QVector<int> &polygonTriangleVertexIndices)
{
    QVector<float> triagnleVertexCoords;
    for(int i = 0; i < polygonTriangleVertexIndices.size(); ++i)
    {
        for(int j = 0; j < 3; ++j)
        {
            triagnleVertexCoords.push_back(vertices[polygonTriangleVertexIndices[i]][j]);
        }
    }
    return triagnleVertexCoords;
}

bool MeshTools::getBoundingBox(QVector<QVector3D> &vertices, QVector3D &maxXYZ, QVector3D &minXYZ)
{
    if(vertices.isEmpty())
        return false;
    maxXYZ = vertices.first();
    minXYZ = vertices.first();
    for(QVector3D vertex: vertices)
    {
        for(int i = 0; i < 3; ++i)
        {
            if(vertex[i] > maxXYZ[i])
                maxXYZ[i] = vertex[i];

            if(vertex[i] < minXYZ[i])
                minXYZ[i] = vertex[i];
        }
    }
    return true;
}

float MeshTools::getRadiusBoundingBox(QVector3D maxXYZ, QVector3D minXYZ)
{
    QVector3D centre = getCentreBoundingBox(maxXYZ, minXYZ);
    QVector3D maxModuloOfBoundingPoints(fmax(fabs(maxXYZ.x()), fabs(minXYZ.x())),
                                            fmax(fabs(maxXYZ.y()), fabs(minXYZ.y())),
                                            fmax(fabs(maxXYZ.z()), fabs(minXYZ.z())));
    return centre.distanceToPoint(maxModuloOfBoundingPoints);
}

QVector3D MeshTools::getCentreBoundingBox(QVector3D maxXYZ, QVector3D minXYZ)
{
    return QVector3D((maxXYZ[0] + minXYZ[0]) / 2, (maxXYZ[1] + minXYZ[1]) / 2, (maxXYZ[2] + minXYZ[2]) / 2);
}

QVector<float> MeshTools::buildAndPackTriangleNormalsCoords(QVector<QVector3D> &vertices, QVector<int> &polygonTriangleVertexIndices)
{
    QVector<float> triagnleNormalsCoords;
    for(int vIndInd = 0; vIndInd < polygonTriangleVertexIndices.size(); vIndInd += 3)
    {
        QVector3D triangleNormal = QVector3D::normal(vertices[polygonTriangleVertexIndices[vIndInd]],
                                                     vertices[polygonTriangleVertexIndices[vIndInd + 1]],
                                                     vertices[polygonTriangleVertexIndices[vIndInd + 2]]);
        for(int j = 0; j < 3; ++j)
            for(int i = 0; i < 3; ++i)
                triagnleNormalsCoords.push_back(triangleNormal[i]);
    }
    return triagnleNormalsCoords;
}

QVector3D MeshTools::makeVector3D(QVector2D point, int zeroCoordIndex)
{
    switch (zeroCoordIndex) {
    case 0:
        return QVector3D(0, point.x(), point.y());
    case 1:
        return QVector3D(point.x(), 0, point.y());
    case 2:
        return QVector3D(0, point.x(), point.y());
    }
    return QVector3D();
}

QVector<float> MeshTools::pack2DLinesTo3D(QVector<QVector2D> lines, int zeroCoordIndex)
{
    // lines vector must be in format {startPointLine1, endPointLine1, startPointLine2, endPointLine2, ..., startPointLineN, endPointLineN}
    QVector<float> packedLines;
    for(int i = 0; i < lines.size(); ++i)
    {
        QVector3D point = makeVector3D(lines[i], zeroCoordIndex);
        for(int j = 0; j < 3; ++j)
            packedLines.append(point[j]);
    }
    return packedLines;
}
