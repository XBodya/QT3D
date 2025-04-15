#include "writer.h"

#include <QVector3D>
#include <QFile>
#include <QTextStream>
#include "basetypes.h"

QString ObjSavingTools::toString(ObjData &objData)
{
    QString result = "";

    for(int vInd = 0; vInd < objData.m_vertices.size(); ++vInd)
    {
        result += QString(QString(VERTEX_TOKEN) + " %1 %2 %3\n").arg(objData.m_vertices[vInd][0]).arg(objData.m_vertices[vInd][1]).arg(objData.m_vertices[vInd][2]);
    }

    for(int vtInd = 0; vtInd < objData.m_texCoords.size(); ++vtInd)
    {
        result += QString(QString(TEXTURE_TOKEN) + " %1 %2\n").arg(objData.m_texCoords[vtInd][0]).arg(objData.m_texCoords[vtInd][1]);
    }

    for(int vnInd = 0; vnInd < objData.m_normals.size(); ++vnInd)
    {
        result += QString(QString(NORMAL_TOKEN) + " %1 %2 %3\n").arg(objData.m_normals[vnInd][0]).arg(objData.m_normals[vnInd][1]).arg(objData.m_normals[vnInd][2]);
    }

    result += "vPolygonIndices: ";
    for(auto vPolygonInd: objData.m_polygonVertexIndices){
        result += QString::number(vPolygonInd) + " ";
    }
    result += "\n";

    result += "vtPolygonIndices: ";
    for(auto vtPolygonInd: objData.m_polygonTexCoordIndices){
        result += QString::number(vtPolygonInd) + " ";
    }
    result += "\n";

    result += "vnPolygonIndices: ";
    for(auto vnPolygonInd: objData.m_polygonNormalIndices){
        result += QString::number(vnPolygonInd) + " ";
    }
    result += "\n";

    result += "vStartPolygonIndices: ";
    for(auto vstartPolygonInd: objData.m_startPolygonVertexIndices){
        result += QString::number(vstartPolygonInd) + " ";
    }
    result += "\n";

    result += "vtStartPolygonIndices: ";
    for(auto vtstartPolygonInd: objData.m_startPolygonTexCoordIndices){
        result += QString::number(vtstartPolygonInd) + " ";
    }
    result += "\n";

    result += "vnStartPolygonIndices: ";
    for(auto vnstartPolygonInd: objData.m_startPolygonNormalIndices){
        result += QString::number(vnstartPolygonInd) + " ";
    }
    result += "\n";

    result += "vPolygonHasTexture: ";
    for(auto vPolygonHasTexture: objData.m_polygonVertexHasTexture){
        result += QString::number(vPolygonHasTexture) + " ";
    }
    result += "\n";

    result += "vPolygonHasNormal: ";
    for(auto vPolygonHasNormal: objData.m_polygonVertexHasNormal){
        result += QString::number(vPolygonHasNormal) + " ";
    }
    result += "\n";
    return result;
}

bool ObjSavingTools::saveToFile(QString filename, ObjData &objData, QString &errorMsg)
{
    QFile saveFile(filename);
    if(!saveFile.open(QFile::WriteOnly | QFile::Text))
    {
        errorMsg = "SaveToFileError. FileOpen is failed";
        return false;
    }
    QTextStream stream(&saveFile);
    ObjSavingTools::write(stream, objData, errorMsg);
    saveFile.close();
    return true;
}

QString ObjSavingTools::toString(QVector3D vertex)
{
    return QString("%1 %2 %3").arg(vertex[0]).arg(vertex[1]).arg(vertex[2]);
}
QString ObjSavingTools::toString(QVector2D texture)
{
    return QString("%1 %2").arg(texture[0]).arg(texture[1]);
}

QString ObjSavingTools::toStringFace(int vInd, int vtInd, int vnInd)
{
    QString faceBody = QString::number(vInd);
    if(vtInd != 0)
    {
        faceBody += "/" + QString::number(vtInd);
    }
    if(vnInd != 0)
    {
        faceBody += ((vtInd == 0) ? ("//"): ("/")) + QString::number(vnInd);
    }
    return faceBody;
}

bool ObjSavingTools::toStringBlockFaces(QString &string,
                                    QVector<int> &polygonVertexIndices,
                                    QVector<int> &polygonTexCoordIndices,
                                    QVector<int> &polygonNormalIndices,
                                    QVector<int> &startPolygonVertexIndices,
                                    QVector<int> &startPolygonTexCoordIndices,
                                    QVector<int> &startPolygonNormalIndices,
                                    QVector<bool> &polygonVertexHasTexture,
                                    QVector<bool> &polygonVertexHasNormal, QString &errorMsg)
{
    for(int i = 0, ivt = 0, ivn = 0; i < startPolygonVertexIndices.size() - 1; ++i)
    {
        int curStartVertexInd = startPolygonVertexIndices[i];
        int faceLen = startPolygonVertexIndices[i + 1] - startPolygonVertexIndices[i];
        QString faceBody = "";
        for(int j = 0; j < faceLen; ++j)
        {
            int vInd = 0, vtInd = 0, vnInd = 0;
            vInd = polygonVertexIndices[curStartVertexInd + j];
            if(polygonVertexHasTexture[i])
            {
                vtInd = polygonTexCoordIndices[startPolygonTexCoordIndices[ivt] + j];
            }
            if(polygonVertexHasNormal[i])
            {
                vnInd = polygonNormalIndices[startPolygonNormalIndices[ivn] + j];
            }
            faceBody += " " + ObjSavingTools::toStringFace(vInd, vtInd, vnInd);
        }
        if(polygonVertexHasTexture[i])
        {
            ivt++;
        }
        if(polygonVertexHasNormal[i])
        {
            ivn++;
        }
        string += FACE_TOKEN + faceBody + '\n';
    }
    return true;
}

bool ObjSavingTools::write(QTextStream &stream, ObjData &objData, QString &errorMsg)
{
    QString facesBlocks = "";
    if(!ObjSavingTools::toStringBlockFaces(facesBlocks,
                                            objData.m_polygonVertexIndices,
                                            objData.m_polygonTexCoordIndices,
                                            objData.m_polygonNormalIndices,
                                            objData.m_startPolygonVertexIndices,
                                            objData.m_startPolygonTexCoordIndices,
                                            objData.m_startPolygonNormalIndices,
                                            objData.m_polygonVertexHasTexture,
                                            objData.m_polygonVertexHasNormal, errorMsg))
    {
        errorMsg = "SaveToFileError. ObjSavingTools toStringBlockFaces is failed!";
        return false;
    }

    for(auto vertex: objData.m_vertices)
    {
        stream << VERTEX_TOKEN << " " << ObjSavingTools::toString(vertex) << '\n';
    }
    stream << '\n';

    for(auto texture: objData.m_texCoords)
    {
        stream << TEXTURE_TOKEN << " " << ObjSavingTools::toString(texture) << '\n';
    }

    stream << '\n';
    for(auto normal: objData.m_normals)
    {
        stream << NORMAL_TOKEN << " " << ObjSavingTools::toString(normal) << '\n';
    }
    stream << '\n';
    stream << facesBlocks;
    return true;
}
