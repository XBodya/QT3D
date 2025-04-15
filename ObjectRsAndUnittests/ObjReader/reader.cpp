#include "reader.h"

#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QVector3D>
#include <QVector2D>

bool ObjReadingTools::parseObjLine(QString line, QString &token, QString &body, QString &errorMsg)
{
    int firstSpaceInd = line.indexOf(' ');
    if((firstSpaceInd == -1) or (firstSpaceInd == 0)){
        errorMsg = "ParseObjLineError. Line is empty or Token mising";
        return false;
    }
    token = line.left(firstSpaceInd);
    body = line.mid(firstSpaceInd + 1);
    if(body.isEmpty())
    {
        errorMsg += "ParseObjLineError. Line body is empty";
        return false;
    }
    return true;
}

bool ObjReadingTools::parseVector3D(QString vertexString, QVector3D &vertex, QString &errorMsg){
    if(vertexString.isEmpty())
    {
        errorMsg = "ParseVector3DError. Body is empty";
        return false;
    }
    QStringList blocks = vertexString.split(' ', Qt::SkipEmptyParts);
    if(blocks.size() != 3)
    {
        errorMsg = "ParseVector3DError. Block counts is not equal to 3";
        return false;
    }
    bool isFloat = false;
    float coords[3];
    for(int coordInd = 0; coordInd < 3; ++coordInd)
    {
        coords[coordInd] = blocks[coordInd].toFloat(&isFloat);
        if(!isFloat)
        {
            errorMsg = "ParseVector3DError. Vertex/normal QVector3D component must be float";
            return false;
        }
    }
    vertex = QVector3D(coords[0], coords[1], coords[2]);
    return true;
}

bool ObjReadingTools::parseVector2D(QString vertexString, QVector2D &texture, QString &errorMsg)
{
    if(vertexString.isEmpty())
    {
        errorMsg = "ParseVector2DError. Body is empty";
        return false;
    }
    QStringList blocks = vertexString.split(' ', Qt::SkipEmptyParts);
    if(blocks.size() != 2)
    {
        errorMsg = "ParseVector2DError. Block counts is not equal to 2";
        return false;
    }
    bool isFloat = false;
    float coords[2];
    for(int coordInd = 0; coordInd < 2; ++coordInd)
    {
        coords[coordInd] = blocks[coordInd].toFloat(&isFloat);
        if(!isFloat)
        {
            errorMsg = "ParseVector2DError. texture QVector2D component must be float";
            return false;
        }
    }
    texture = QVector2D(coords[0], coords[1]);
    return true;
}

bool ObjReadingTools::parseInt(QString string, int &value)
{
    bool isInt = true;
    value = string.toInt(&isInt);
    return isInt;
}

bool ObjReadingTools::parseFaceElement(QString faceBody, int &vInd, int &vtInd, int &vnInd, QString &errorMsg){
    QStringList face = faceBody.split('/');
    if(face.size() > 3)
    {
        errorMsg = "ParseFaceElementError. Wrong face format";
        return false;
    }
    if(!ObjReadingTools::parseInt(face[0], vInd))
    {
        errorMsg = "ParseFaceElementError. Vertex index must in face body must be integer";
        return false;
    }
    if(face.size() == 1){
        return true;
    }
    if(face[1] == "")
    {
        vtInd = 0;
    }
    else if(!ObjReadingTools::parseInt(face[1], vtInd))
    {
        errorMsg = "ParseFaceElementError. Vertex texture index must in face body must be integer";
        return false;
    }
    if(face.size() == 2){
        return true;
    }
    if(face[2] == ""){
        vnInd = 0;
    }
    else if(!ObjReadingTools::parseInt(face[2], vnInd))
    {
        errorMsg = "ParseFaceElementError. Vertex normal index must in face body must be integer";
        return false;
    }
    return true;

}

bool ObjReadingTools::parsePolygonBlock(QString stringBlock, ObjData &objData, QString &errorMsg)
{
    // this method must be apppend all 9 elements OR not be append nothing
    if(stringBlock.isEmpty())
    {
        errorMsg = "ParsePolygonBlockError. Body block is empty";
        return false;
    }
    QStringList blocks = stringBlock.split(' ', Qt::SkipEmptyParts);
    bool haveVt = false, haveVn = false;

    QVector<int> polygonVertexIndices;
    QVector<int> polygonTexCoordIndices;
    QVector<int> polygonNormalIndices;
    QVector<int> startPolygonVertexIndices;
    QVector<int> startPolygonTexCoordIndices;
    QVector<int> startPolygonNormalIndices;
    QVector<bool> polygonVertexHasTexture;
    QVector<bool> polygonVertexHasNormal;

    for(int blockInd = 0; blockInd < blocks.size(); ++blockInd){
        int vInd = 0, vtInd = 0, vnInd = 0;
        if(!ObjReadingTools::parseFaceElement(blocks[blockInd], vInd, vtInd, vnInd, errorMsg))
        {
            return false;
        }
        if(blockInd == 0)
        {
            haveVt = vtInd != 0;
            haveVn = vnInd != 0;
        }
        else if((haveVn != (vnInd != 0)) or (haveVt != (vtInd != 0)))
        {
            errorMsg = "ParsePolygonBlockError. Faces blocks must be in format v, v/vt, v//vn, v/vt/vn";
            return false;
        }
        if(blockInd == 0)
        {
            startPolygonVertexIndices.push_back(objData.m_polygonVertexIndices.size());
            startPolygonTexCoordIndices.push_back(((haveVt) ? (objData.m_polygonTexCoordIndices.size()) : (-1)));
            startPolygonNormalIndices.push_back(((haveVn) ? (objData.m_polygonNormalIndices.size()) : (-1)));
            polygonVertexHasTexture.push_back(haveVt);
            polygonVertexHasNormal.push_back(haveVn);
        }
        polygonVertexIndices.push_back(vInd);
        if(haveVt)
        {
            polygonTexCoordIndices.push_back(vtInd);
        }
        if(haveVn){
            polygonNormalIndices.push_back(vnInd);
        }
    }
    objData.m_startPolygonVertexIndices.pop_back();
    objData.m_polygonVertexIndices.append(polygonVertexIndices);
    objData.m_startPolygonVertexIndices.append(startPolygonVertexIndices);
    objData.m_startPolygonVertexIndices.push_back(objData.m_polygonVertexIndices.size());
    if(haveVt)
    {
        objData.m_startPolygonTexCoordIndices.pop_back();
        objData.m_polygonTexCoordIndices.append(polygonTexCoordIndices);
        objData.m_startPolygonTexCoordIndices.append(startPolygonTexCoordIndices);
        objData.m_startPolygonTexCoordIndices.push_back(objData.m_polygonTexCoordIndices.size());
    }
    if(haveVn)
    {
        objData.m_startPolygonNormalIndices.pop_back();
        objData.m_polygonNormalIndices.append(polygonNormalIndices);
        objData.m_startPolygonNormalIndices.append(startPolygonNormalIndices);
        objData.m_startPolygonNormalIndices.push_back(objData.m_polygonNormalIndices.size());
    }
    objData.m_polygonVertexHasTexture.append(polygonVertexHasTexture);
    objData.m_polygonVertexHasNormal.append(polygonVertexHasNormal);
    return true;
}

bool ObjReadingTools::read(QTextStream &stream, ObjData &objData, QString &errorMsg)
{
    while(!stream.atEnd()){
        QString line = stream.readLine(), token, body;
        if(!ObjReadingTools::parseObjLine(line, token, body, errorMsg))
        {
            continue;
        }
        if(token == VERTEX_TOKEN)
        {
            QVector3D vertex(0, 0, 0);
            ObjReadingTools::parseVector3D(body, vertex, errorMsg);
            objData.m_vertices.push_back(vertex);
        }
        else if(token == TEXTURE_TOKEN)
        {
            QVector2D texture(0, 0);
            ObjReadingTools::parseVector2D(body, texture, errorMsg);
            objData.m_texCoords.push_back(texture);
        }
        else if(token == NORMAL_TOKEN)
        {
            QVector3D normal(0, 0, 0);
            ObjReadingTools::parseVector3D(body, normal, errorMsg);
            objData.m_normals.push_back(normal);
        }
        else if(token == FACE_TOKEN)
        {
            ObjReadingTools::parsePolygonBlock(body, objData, errorMsg);
        }
    }
    return true;
}

bool ObjReadingTools::readFile(QString filename, ObjData &objData, QString &errorMsg){
    QFile objFile(filename);
    objFile.open(QFile::ReadOnly);
    QTextStream stream(&objFile);
    ObjReadingTools::read(stream, objData, errorMsg);
    return true;
}
