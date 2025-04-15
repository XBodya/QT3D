#ifndef WRITER_H
#define WRITER_H

#include <QString>
#include "basetypes.h"

namespace ObjSavingTools{
    bool write(QTextStream &stream, ObjData &objData, QString &errorMsg);
    QString toString(ObjData &objData);
    QString toString(QVector3D vertex);
    QString toString(QVector2D texture);
    QString toStringFace(int vInd, int vtInd, int vnInd);
    bool toStringBlockFaces(QString &string, QVector<int> &polygonVertexIndices,
                       QVector<int> &polygonTexCoordIndices,
                       QVector<int> &polygonNormalIndices,
                       QVector<int> &startPolygonVertexIndices,
                       QVector<int> &startPolygonTexCoordIndices,
                       QVector<int> &startPolygonNormalIndices,
                       QVector<bool> &polygonVertexHasTexture,
                       QVector<bool> &polygonVertexHasNormal, QString &errorMsg);
    bool saveToFile(QString filename, ObjData &objData, QString &errorMsg);
}

#endif // WRITER_H
