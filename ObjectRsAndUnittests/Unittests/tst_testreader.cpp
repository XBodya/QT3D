#include <QTest>

// add necessary includes here
#include "../ObjReader/reader.h"
#include "../ObjReader/basetypes.h"
#include "../ObjReader/writer.h"
#include <QVector3D>
#include <QVector2D>

class TestReader : public QObject
{
    Q_OBJECT

public:
    TestReader();
    ~TestReader();

private slots:
    void initTestCase();
    void cleanupTestCase();

    void testParseVector3D();
    void testRead();
    void testParseObjLine();
    void testParseVector2D();
    void testParsePolygonBlock();
    void testParseInt();
    void testParseFaceElement();
    void testConvert();

    void testToStringFace();
    void testToStringBlockFaces();
    void testWrite();

};

TestReader::TestReader() {}

TestReader::~TestReader() {}

void TestReader::initTestCase() {}

void TestReader::cleanupTestCase() {}


void TestReader::testConvert()
{
    QString strobj =
        "v 0 0 0\n"
        "v 1 0 0\n"
        "v 2 0 0\n"
        "v 2 1 0\n"
        "v 1 1 0\n"
        "f 1 2 5\n"
        "f 2 3 4 5\n";
    QTextStream strobjstream(&strobj);
    QString test = "", errorMsg;
    ObjData objData2;
    ObjReadingTools::read(strobjstream, objData2, errorMsg);

    QVector<QVector3D> vertices = {{0, 0, 0}, {1, 0, 0}, {2, 0, 0}, {2, 1, 0}, {1, 1, 0}};
    QVector<int> indices = {0, 1, 4, 1, 2, 3, 4};
    QVector<int> starts = {0, 3, 7};

    ObjData objData;
    objData.m_vertices = vertices;
    objData.m_polygonVertexIndices = indices;
    objData.m_startPolygonVertexIndices = starts;

    QTextStream out(stdout), stream(&test);
    ObjSavingTools::write(stream, objData2, errorMsg);
    //out << ObjSavingTools::toString(objData2);
}

void TestReader::testWrite()
{
    QString objStr =
        "v 1 2 3\n"
        "v 2 2 3\n"
        "v 1 2 4\n"
        "v 5 2 3\n"
        "v 1 15 3\n"
        "v 1 12 3\n"
        "\n"
        "\n"
        "\n"
        "f 1/1 2/2 3/5\n"
        "f 1 2 3\n"
        "f 3/1 4/2 5/3\n"
        "f 6/4/1 3/5/3 7/6/5\n"
        "f 7//1 8//2 9//3\n";
    QString strRes = "";
    QString errorMsg;
    ObjData objData;
    QTextStream data(&objStr), excepted(&objStr), result(&strRes);

    QVERIFY(ObjReadingTools::read(data, objData, errorMsg));
    QVERIFY(ObjSavingTools::write(result, objData, errorMsg));

    data.reset();
    result.reset();
    QString resLine = "", exceptedLine = "";
    while(!data.atEnd() and !result.atEnd())
    {
        if(resLine.isEmpty() or resLine == "\n")
        {
            resLine = result.readLine();
            continue;
        }
        if(exceptedLine.isEmpty() or exceptedLine == "\n")
        {
            exceptedLine = data.readLine();
            continue;
        }
        QCOMPARE(resLine, exceptedLine);
        resLine = "";
        exceptedLine = "";
    }

    // SKIPING \n and empty lines
    while(!data.atEnd())
    {
        exceptedLine = data.readLine();
        if(exceptedLine.isEmpty() or exceptedLine == "\n")
        {
            //exceptedLine = data.readLine();
            continue;
        }
    }

    while(!result.atEnd())
    {
        resLine = result.readLine();
        if(resLine.isEmpty() or resLine == "\n")
        {
            //resLine = result.readLine();
            continue;
        }
    }

    QCOMPARE(data.atEnd(), result.atEnd());
}

void TestReader::testToStringBlockFaces()
{
    ObjData objData;
    QString errorMsg;
    QString exceptedFaceStr =
        "f 1/1 2/2 3/5\n"
        "f 1 2 3\n"
        "f 3/1 4/2 5/3\n"
        "f 6/4/1 3/5/3 7/6/5\n"
        "f 7//1 8//2 9//3\n";
    QTextStream objStream(&exceptedFaceStr);
    QTextStream output(stdout);
    QString textResult;
    ObjReadingTools::read(objStream, objData, errorMsg);
    //output << ObjSavingTools::toString(objData) << '\n';
    ObjSavingTools::toStringBlockFaces(textResult, objData.m_polygonVertexIndices,
                                                    objData.m_polygonTexCoordIndices,
                                                    objData.m_polygonNormalIndices,
                                                    objData.m_startPolygonVertexIndices,
                                                    objData.m_startPolygonTexCoordIndices,
                                                    objData.m_startPolygonNormalIndices,
                                                    objData.m_polygonVertexHasTexture,
                                                    objData.m_polygonVertexHasNormal, errorMsg );
    //output << textResult << '\n';
    QCOMPARE(textResult, exceptedFaceStr);
}

void TestReader::testToStringFace()
{
    int vInd = 1, vtInd = 0, vnInd = 0;
    QCOMPARE(ObjSavingTools::toStringFace(vInd, vtInd, vnInd), "1");

    vInd = 1, vtInd = 2, vnInd = 0;
    QCOMPARE(ObjSavingTools::toStringFace(vInd, vtInd, vnInd), "1/2");

    vInd = 1, vtInd = 2, vnInd = 5;
    QCOMPARE(ObjSavingTools::toStringFace(vInd, vtInd, vnInd), "1/2/5");

    vInd = 1, vtInd = 0, vnInd = 5;
    QCOMPARE(ObjSavingTools::toStringFace(vInd, vtInd, vnInd), "1//5");
}

void TestReader::testParseVector3D() {
    QVector3D vertex;
    QString errorMsg;
    QVERIFY(ObjReadingTools::parseVector3D("12 12 12", vertex, errorMsg));
    QCOMPARE(vertex, QVector3D(12, 12, 12));

    QVERIFY(!ObjReadingTools::parseVector3D("", vertex, errorMsg));

    QVERIFY(ObjReadingTools::parseVector3D("12.1231 12 12", vertex, errorMsg));
    QCOMPARE(vertex, QVector3D(12.1231, 12, 12));

    QVERIFY(ObjReadingTools::parseVector3D("12.1231 12.132 1213", vertex, errorMsg));
    QCOMPARE(vertex, QVector3D(12.1231, 12.132, 1213));

    QVERIFY(ObjReadingTools::parseVector3D("12.1231 12.132 1213.132", vertex, errorMsg));
    QCOMPARE(vertex, QVector3D(12.1231, 12.132, 1213.132));
}

void TestReader::testRead()
{
    QString stringObj =
                    "v 0 0 0\n"
                    "v 1 0 0\n"
                    "v 2 0 0\n"
                    "v 2 1 0\n"
                    "v 1 1 0\n"
                    "f 1 2 5\n"
                    "f 2 3 4 5\n";

    ObjData objData, exceptedData;
    QString errorMsg;
    QTextStream stream(&stringObj);
    QVERIFY(ObjReadingTools::read(stream, objData, errorMsg));

    exceptedData.m_vertices = {
        {0, 0, 0},
        {1, 0, 0},
        {2, 0, 0},
        {2, 1, 0},
        {1, 1, 0},
        };
    exceptedData.m_polygonVertexIndices = {
        1, 2, 5, 2, 3, 4, 5
    };
    exceptedData.m_startPolygonVertexIndices = {
        0, 3, 7
    };

    // QTextStream output(stdout);
    // output << errorMsg << '\n';
    QCOMPARE(objData.m_vertices, exceptedData.m_vertices);
    QCOMPARE(objData.m_polygonVertexIndices, exceptedData.m_polygonVertexIndices);
    QCOMPARE(objData.m_startPolygonVertexIndices, exceptedData.m_startPolygonVertexIndices);
}

void TestReader::testParseObjLine()
{
    QString token, body;
    QString errorMsg;


    QVERIFY(ObjReadingTools::parseObjLine("v 2.712726 -2.398764 -2.492640", token, body, errorMsg));
    QCOMPARE(token, VERTEX_TOKEN);
    QCOMPARE(body, "2.712726 -2.398764 -2.492640");

    QVERIFY(ObjReadingTools::parseObjLine("v 2.712726 2.398764 -2.492640", token, body, errorMsg));
    QCOMPARE(token, VERTEX_TOKEN);
    QCOMPARE(body, "2.712726 2.398764 -2.492640");

    QVERIFY(ObjReadingTools::parseObjLine("v 2.712726 2.398764 2.492640", token, body, errorMsg));
    QCOMPARE(token, VERTEX_TOKEN);
    QCOMPARE(body, "2.712726 2.398764 2.492640");

    QVERIFY(ObjReadingTools::parseObjLine("vt 2.712726 -2.398764", token, body, errorMsg));
    QCOMPARE(token, TEXTURE_TOKEN);
    QCOMPARE(body, "2.712726 -2.398764");

    QVERIFY(ObjReadingTools::parseObjLine("vn +2.712726 -2.398764 -2.492640", token, body, errorMsg));
    QCOMPARE(token, NORMAL_TOKEN);
    QCOMPARE(body, "+2.712726 -2.398764 -2.492640");

    QVERIFY(!ObjReadingTools::parseObjLine("", token, body, errorMsg));

    QVERIFY(!ObjReadingTools::parseObjLine("vn ", token, body, errorMsg));
}

void TestReader::testParseVector2D()
{
    QVector2D texture;
    QString errorMsg;

    QVERIFY(ObjReadingTools::parseVector2D("12 12", texture, errorMsg));
    QCOMPARE(texture, QVector2D(12, 12));

    QVERIFY(!ObjReadingTools::parseVector2D("", texture, errorMsg));

    QVERIFY(ObjReadingTools::parseVector2D("12.1231 12", texture, errorMsg));
    QCOMPARE(texture, QVector2D(12.1231, 12));

    QVERIFY(ObjReadingTools::parseVector2D("12.1231 12.132", texture, errorMsg));
    QCOMPARE(texture, QVector2D(12.1231, 12.132));

    QVERIFY(ObjReadingTools::parseVector2D("12.1231 12.132 ", texture, errorMsg));
    QCOMPARE(texture, QVector2D(12.1231, 12.132));
}

void TestReader::testParsePolygonBlock()
{
    QString errorMsg;
    ObjData objData;
    ObjData exceptedObjData;
    exceptedObjData.m_polygonVertexIndices = {6, 3, 7};
    exceptedObjData.m_polygonTexCoordIndices = {4, 5, 6};
    exceptedObjData.m_polygonNormalIndices = {1, 3, 5};
    exceptedObjData.m_startPolygonVertexIndices = {0, 3};
    exceptedObjData.m_startPolygonTexCoordIndices = {0, 3};
    exceptedObjData.m_startPolygonNormalIndices = {0, 3};
    QVERIFY(ObjReadingTools::parsePolygonBlock("6/4/1 3/5/3 7/6/5", objData, errorMsg));
    QTextStream output(stdout);
    //output << errorMsg << '\n';
    QCOMPARE(objData.m_polygonVertexIndices, exceptedObjData.m_polygonVertexIndices);
    QCOMPARE(objData.m_polygonTexCoordIndices, exceptedObjData.m_polygonTexCoordIndices);
    QCOMPARE(objData.m_polygonNormalIndices, exceptedObjData.m_polygonNormalIndices);
    QCOMPARE(objData.m_startPolygonVertexIndices, exceptedObjData.m_startPolygonVertexIndices);
    QCOMPARE(objData.m_startPolygonTexCoordIndices, exceptedObjData.m_startPolygonTexCoordIndices);
    QCOMPARE(objData.m_startPolygonNormalIndices, exceptedObjData.m_startPolygonNormalIndices);

    exceptedObjData.m_polygonVertexIndices = {6, 3, 7, 6, 3, 7};
    exceptedObjData.m_polygonTexCoordIndices = {4, 5, 6};
    exceptedObjData.m_polygonNormalIndices = {1, 3, 5, 1, 3, 5};
    exceptedObjData.m_startPolygonVertexIndices = {0, 3, 6};
    exceptedObjData.m_startPolygonTexCoordIndices = {0, 3};
    exceptedObjData.m_startPolygonNormalIndices = {0, 3, 6};
    QVERIFY(ObjReadingTools::parsePolygonBlock("6//1 3//3 7//5", objData, errorMsg));
    QCOMPARE(objData.m_polygonVertexIndices, exceptedObjData.m_polygonVertexIndices);
    QCOMPARE(objData.m_polygonTexCoordIndices, exceptedObjData.m_polygonTexCoordIndices);
    QCOMPARE(objData.m_polygonNormalIndices, exceptedObjData.m_polygonNormalIndices);
    QCOMPARE(objData.m_startPolygonVertexIndices, exceptedObjData.m_startPolygonVertexIndices);
    QCOMPARE(objData.m_startPolygonTexCoordIndices, exceptedObjData.m_startPolygonTexCoordIndices);
    QCOMPARE(objData.m_startPolygonNormalIndices, exceptedObjData.m_startPolygonNormalIndices);

    QVERIFY(!ObjReadingTools::parsePolygonBlock("124thrthrt/124/213erg1 3//3 7//5", objData, errorMsg));
    QCOMPARE(objData.m_polygonVertexIndices, exceptedObjData.m_polygonVertexIndices);
    QCOMPARE(objData.m_polygonTexCoordIndices, exceptedObjData.m_polygonTexCoordIndices);
    QCOMPARE(objData.m_polygonNormalIndices, exceptedObjData.m_polygonNormalIndices);
    QCOMPARE(objData.m_startPolygonVertexIndices, exceptedObjData.m_startPolygonVertexIndices);
    QCOMPARE(objData.m_startPolygonTexCoordIndices, exceptedObjData.m_startPolygonTexCoordIndices);
    QCOMPARE(objData.m_startPolygonNormalIndices, exceptedObjData.m_startPolygonNormalIndices);



}

void TestReader::testParseInt()
{
    int value;

    QVERIFY(ObjReadingTools::parseInt("12412", value));
    QCOMPARE(value, 12412);

    QVERIFY(!ObjReadingTools::parseInt("asg", value));

    QVERIFY(ObjReadingTools::parseInt("2", value));
    QCOMPARE(value, 2);

    QVERIFY(!ObjReadingTools::parseInt("124112312312", value));

    QVERIFY(!ObjReadingTools::parseInt("", value));
}

void TestReader::testParseFaceElement()
{
    // QString s = "6/4/1";
    int v, vt, vn;
    QString errorMsg;

    QVERIFY(ObjReadingTools::parseFaceElement("6/4/1", v, vt, vn, errorMsg));
    QCOMPARE(v, 6);
    QCOMPARE(vt, 4);
    QCOMPARE(vn, 1);

    QVERIFY(ObjReadingTools::parseFaceElement("6/", v, vt, vn, errorMsg));

    QVERIFY(!ObjReadingTools::parseFaceElement("", v, vt, vn, errorMsg));

    QVERIFY(!ObjReadingTools::parseFaceElement("///////", v, vt, vn, errorMsg));

    QVERIFY(ObjReadingTools::parseFaceElement("6//1", v, vt, vn, errorMsg));
    QCOMPARE(v, 6);
    QCOMPARE(vt, 0);
    QCOMPARE(vn, 1);

    QVERIFY(ObjReadingTools::parseFaceElement("6123/123123/113", v, vt, vn, errorMsg));
    QCOMPARE(v, 6123);
    QCOMPARE(vt, 123123);
    QCOMPARE(vn, 113);
}


QTEST_APPLESS_MAIN(TestReader)

#include "tst_testreader.moc"
