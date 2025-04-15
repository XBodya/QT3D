#include <QCoreApplication>
#include "reader.h"
#include "writer.h"

#define _DEBUG

int main(int argc, char *argv[])
{
#ifdef _DEBUG
    QString errorMsg;
    ObjData objData;
    ObjData exceptedObjData;
    // // exceptedObjData.m_polygonVertexIndices = {6, 3, 7};
    // // exceptedObjData.m_polygonTexCoordIndices = {4, 5, 6};
    // // exceptedObjData.m_polygonNormalIndices = {1, 3, 5};
    // // exceptedObjData.m_startPolygonVertexIndices = {0, 3};
    // // exceptedObjData.m_startPolygonTexCoordIndices = {0, 3};
    // // exceptedObjData.m_startPolygonNormalIndices = {0, 3};
    // // ObjReadingTools::parsePolygonBlock("6/4/1 3/5/3 7/6/5", objData, errorMsg);
    // // QTextStream output(stdout);
    // // output << errorMsg << '\n';

    // //ObjReadingTools::readFile("C:\\Users\\games\\gits\\r3ds\\ObjectRsAndUnittests\\ObjReader\\airboat.obj", objData, errorMsg);
    //output << errorMsg << '\n';
    //output << ObjSavingTools::toString(objData);
    //QVector3D www(1, 2, 3);
    //output << QString("v %1 %2 %3\n").arg(www[0]).arg(www[1]).arg(www[2]);
    QString objStr =
            "v 1 2 3\n"
            "v 2 2 3\n"
            "v 1 2 4\n"
            "v 5 2 3\n"
            "v 1 15 3\n"
            "v 1 12 3\n"
            "f 1/1 2/2 3/5\n"
            "f 1 2 3\n"
            "f 3/1 4/2 5/3\n"
            "f 6/4/1 3/5/3 7/6/5\n"
            "f 7//1 8//2 9//3\n";
    QTextStream objFile(&objStr);
    QTextStream out(stdout);
    ObjReadingTools::read(objFile, objData, errorMsg);
    ObjSavingTools::saveToFile("C:\\Users\\games\\gits\\r3ds\\ObjectRsAndUnittests\\ObjReader\\QIOJQF.obj", objData, errorMsg);
    out << errorMsg;
    // QTextStream ss(&objStr);
    // ObjReadingTools::read(ss, objData, errorMsg);
    // output << ObjSavingTools::toString(objData) << '\n' << errorMsg;
    // ObjData objData;
    // QString errorMsg;
    // QString objStr =
    //     "f 1/1 2/2 3/5\n"
    //     "f 1 2 3\n"
    //     "f 3/1 4/2 5/3\n"
    //     "f 6/4/1 3/5/3 7/6/5\n"
    //     "f 7//1 8//2 9//3\n";
    // QTextStream objStream(&objStr);
    // QTextStream output(stdout);
    // QString textResult;
    // ObjReadingTools::read(objStream, objData, errorMsg);
    // output << ObjSavingTools::toString(objData) << '\n';
    // ObjSavingTools::toStringBlockFaces(textResult, objData.m_polygonVertexIndices,
    //                                    objData.m_polygonTexCoordIndices,
    //                                    objData.m_polygonNormalIndices,
    //                                    objData.m_startPolygonVertexIndices,
    //                                    objData.m_startPolygonTexCoordIndices,
    //                                    objData.m_startPolygonNormalIndices,
    //                                    objData.m_polygonVertexHasTexture,
    //                                    objData.m_polygonVertexHasNormal, errorMsg );
    // output << textResult << '\n';
#endif
    return 0;
}
