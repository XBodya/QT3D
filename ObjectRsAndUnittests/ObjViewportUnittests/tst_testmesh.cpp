#include <QTest>
#include <QWidget>
#include <QTextStream>
#include "../ObjViewport/mesh.h"
#include "../ObjViewport/drawablegrid.h"

// add necessary includes here

class TestMesh : public QObject
{
    Q_OBJECT

public:
    TestMesh();
    ~TestMesh();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void testBuildTriangleVertexIndices();
    void testPackTriangleVertexCoords();
    void testNormals();
    void testGridCalculate();
};

TestMesh::TestMesh() {}

TestMesh::~TestMesh() {}

void TestMesh::initTestCase() {}

void TestMesh::cleanupTestCase() {}

void TestMesh::testNormals()
{
    QVector<QVector3D> vertices = {{0, 0, 0}, {1, 0, 0}, {2, 0, 0}, {2, 1, 0}, {1, 1, 0}};
    QVector<int> indices = {1, 2, 5, 2, 3, 4, 5};
    QVector<int> starts = {0, 3, 7};
    QVector<int> triangleIndices = MeshTools::buildTriangleVertexIndices(indices, starts);
    QVector<float> data = MeshTools::packTriangleVertexCoords(vertices, triangleIndices);
    QVector<float> data2 = MeshTools::buildAndPackTriangleNormalsCoords(vertices, triangleIndices);
        QCOMPARE(data.size(), data2.size());
}

void TestMesh::testBuildTriangleVertexIndices()
{
    QVector<int> indices = {1, 2, 5, 2, 3, 4, 5};
    QVector<int> starts = {0, 3, 7};
    QVector<int> triangleIndices = MeshTools::buildTriangleVertexIndices(indices, starts);

    // QTextStream output(stdout);
    // for(auto it: triangleIndices)
    //     output << it << ' ';
    // output << '\n';
    QCOMPARE(triangleIndices, QVector<int>({0, 1, 4, 1, 2, 3, 1, 2, 4}));
}

void TestMesh::testPackTriangleVertexCoords()
{
    QVector<QVector3D> vertices = {{0, 0, 0}, {1, 0, 0}, {2, 0, 0}, {2, 1, 0}, {1, 1, 0}};
    QVector<int> indices = {1, 2, 5, 2, 3, 4, 5};
    QVector<int> starts = {0, 3, 7};
    QVector<int> triangleIndices = MeshTools::buildTriangleVertexIndices(indices, starts);
    QVector<float> data = MeshTools::packTriangleVertexCoords(vertices, triangleIndices);
    QVector<float> excepted = {0, 0, 0, 1, 0, 0, 1, 1, 0, 1, 0, 0, 2, 0, 0, 2, 1, 0, 1, 0, 0, 2, 0, 0, 1, 1, 0};
    QTextStream out(stdout);
    //for(auto it: MeshTools::buildTriangleVertexIndices(indices, starts)){
    //    out << it << ' ';
    //}
    //out << '\n';
    QCOMPARE(data, excepted);
}

void TestMesh::testGridCalculate(){
    QVector<QVector2D> lines = DrawableGrid(1, 3, QColor(1, 2, 3)).getGrid2D();
    QTextStream out(stdout);
    for(int i = 0; i < lines.size(); i += 1)
    {
        out << '(' << lines[i].x() << ", " << lines[i].y() << ')' << '\n';
    }
}

QTEST_APPLESS_MAIN(TestMesh)

#include "tst_testmesh.moc"
