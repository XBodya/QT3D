#ifndef BASETYPES_H
#define BASETYPES_H

#ifndef QVECTORS_HAVE_INCLUDE
#define QVECTORS_HAVE_INCLUDE
#include <QVector>
#include <qobject.h>
#endif


#define VERTEX_TOKEN "v"
#define TEXTURE_TOKEN "vt"
#define NORMAL_TOKEN "vn"
#define FACE_TOKEN "f"
#define GROUP_TOKEN "g"
#define OBJECT_TOKEN "o"
#define LINE_TOKEN "l"

class ObjData
{
public:
    QVector<QVector3D> m_vertices;
    QVector<QVector2D> m_texCoords;
    QVector<QVector3D> m_normals;

    QVector<int> m_polygonVertexIndices;
    QVector<int> m_polygonTexCoordIndices;
    QVector<int> m_polygonNormalIndices;

    QVector<int> m_startPolygonVertexIndices = {0};
    QVector<int> m_startPolygonTexCoordIndices = {0};
    QVector<int> m_startPolygonNormalIndices = {0};

    QVector<bool> m_polygonVertexHasTexture;
    QVector<bool> m_polygonVertexHasNormal;

};

#endif // BASETYPES_H
