#include "drawablegrid.h"
#include "draw.h"
#include "mesh.h"

#include <GL/gl.h>
#include <QVector3D>
#include <QVector4D>

DrawableGrid::DrawableGrid(): DrawableObject(){
    calculateGrid();
}

DrawableGrid::~DrawableGrid()
{
    delete m_vertices;
    m_vertices = nullptr;

    delete m_shader;
    m_shader = nullptr;
}

DrawableGrid::DrawableGrid(double size, int nSegments, QColor color): m_size(size), m_nSegments(nSegments), m_color(color)
{
    calculateGrid();
}

QVector<QVector2D> DrawableGrid::getGrid2D()
{
    return m_linesCoords;
}

void DrawableGrid::calculateGrid()
{
    m_linesCoords.clear();
    QVector2D centerOfGrid = QVector2D(m_nSegments * m_size / 2, m_nSegments * m_size / 2);
    for(int i = 0; i <= m_nSegments; ++i){
        QVector2D horizontalLineStartPoint(0, i * m_size),
            horizontalLineEndPoint(m_size * m_nSegments, i * m_size);
        QVector2D verticalLineStartPoint(m_size * i, 0),
            verticalLineEndPoint(i * m_size, m_size * m_nSegments);
        m_linesCoords.append(horizontalLineStartPoint - centerOfGrid);
        m_linesCoords.append(horizontalLineEndPoint - centerOfGrid);
        m_linesCoords.append(verticalLineStartPoint - centerOfGrid);
        m_linesCoords.append(verticalLineEndPoint - centerOfGrid);
    }
}

void DrawableGrid::setShader(QString vertexShaderFilename, QString fragmentShaderFilename)
{
    if(m_linesCoords.isEmpty())
    {
        return;
    }
    m_shader = GlDrawTools::createShaderProgram(vertexShaderFilename, fragmentShaderFilename);
    m_hasShaderToDraw = m_shader != nullptr;
    return;
}


void DrawableGrid::draw(QMatrix4x4 viewMatrix, QMatrix4x4 projectionMatrix)
{
    if(!m_hasShaderToDraw)
    {
        return;
    }
    m_vertices = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);

    if(!m_vertices->create())
        return;
    if(!m_vertices->bind())
        return;

    QVector<float> linesCoords = MeshTools::pack2DLinesTo3D(m_linesCoords, 1);

    int nVertices = linesCoords.size();
    int dataSize = nVertices * sizeof(float);

    m_vertices->allocate(linesCoords.constData(), dataSize);
    m_vertices->release();

    if(!m_shader->bind())
        qFatal("Cannot bind shader");

    m_shader->setUniformValue("modelViewMatrix", viewMatrix);
    m_shader->setUniformValue("projectionMatrix", projectionMatrix);
    m_shader->setUniformValue("gridColor", QVector4D(m_color.redF(), m_color.greenF(), m_color.blueF(), m_color.alphaF()));

    if(!m_vertices->bind())
        qFatal("Cannot bind vertex buffer");

    m_shader->setAttributeBuffer("vertex", GL_FLOAT, 0, 3, 0);
    m_shader->enableAttributeArray("vertex");
    m_vertices->release();

    glDrawArrays(GL_LINES, 0, nVertices);

    m_shader->disableAttributeArray("vertex");
    m_shader->release();
}

bool DrawableGrid::canToDraw(){
    return m_hasShaderToDraw;
}
