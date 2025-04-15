#ifndef READER_H
#define READER_H

#include <QString>
//#include <qobject.h>
#include "basetypes.h"
#include <QVector2D>
#include <QVector3D>

class Reader
{
public:
    Reader();
};

namespace ObjReadingTools
{
bool read(QTextStream &stream, ObjData &objData, QString &errorMsg);
bool readFile(QString filename, ObjData &objData, QString &errorMsg);
bool parseObjLine(QString line, QString &token, QString &body, QString &errorMsg);
bool parseVector3D(QString vertexString, QVector3D &vertex, QString &errorMsg);
bool parseVector2D(QString vertexString, QVector2D &vertex, QString &errorMsg);
bool parsePolygonBlock(QString stringBlock, ObjData &objData, QString &errorMsg);
bool parseInt(QString string, int &value);
bool parseFaceElement(QString faceBody, int &vInd, int &vtInd, int &vpInd, QString &errorMsg);
}

#endif // READER_H
