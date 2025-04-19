#include "drawablemesh.h"
#include "draw.h"


DrawableMesh::DrawableMesh(QString vertexShaderFilename, QString fragmentShaderFilename,
                           QVector<float> triangleVertexCoords, QVector<float> triangleNormalCoords)
{
    m_shader = GlDrawTools::createShaderProgram(vertexShaderFilename, fragmentShaderFilename);
    Q_ASSERT(m_shader != nullptr);

    m_vertices = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    if(!m_vertices->create())
        qFatal("Cannot create vertex buffer for drawable mesh");
    if(!m_vertices->bind())
        qFatal("Cannot bind vertex buffer for drawable mesh");

    m_nVertices = triangleVertexCoords.size() / 3;
    int dataSize = triangleVertexCoords.size() * sizeof(float);

    m_vertices->allocate(triangleVertexCoords.constData(), dataSize);
    m_vertices->release();

    m_normals = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    if(!m_normals->create())
        qFatal("Cannot create normal buffer for drawable mesh");
    if(!m_normals->bind())
        qFatal("Cannot bind normal buffer for drawable mesh");

    //m_nVertices = triangleNormalCoords.size() / 3;
    dataSize = triangleNormalCoords.size() * sizeof(float);

    m_normals->allocate(triangleNormalCoords.constData(), dataSize);
    m_normals->release();
}

void DrawableMesh::draw(QMatrix4x4 viewMatrix, QMatrix4x4 projectionMatrix){
    if(!m_shader->bind())
        qFatal("Cannot bind shader");

    m_shader->setUniformValue("modelViewMatrix", viewMatrix);
    m_shader->setUniformValue("projectionMatrix", projectionMatrix);
    m_shader->setUniformValue("fragmentColor", GlDrawTools::toRGBf(m_fragmentColor));
    m_shader->setUniformValue("drawPixelByNormalCoords", m_toDrawNormalMap);

    if(!m_vertices->bind())
        qFatal("Cannot bind vertex buffer");

    m_shader->setAttributeBuffer("vertex", GL_FLOAT, 0, 3, 0);
    m_shader->enableAttributeArray("vertex");
    m_vertices->release();

    if(!m_normals->bind())
        qFatal("Cannot bind normals buffer");

    m_shader->setAttributeBuffer("normal", GL_FLOAT, 0, 3, 0);
    m_shader->enableAttributeArray("normal");
    m_normals->release();

    glDrawArrays(GL_TRIANGLES, 0, m_nVertices);

    m_shader->disableAttributeArray("vertex");
    m_shader->disableAttributeArray("normal");
    m_shader->release();
}

QColor DrawableMesh::fragmentColor() const
{
    return m_fragmentColor;
}

void DrawableMesh::setFragmentColor(const QColor &newFragmentColor)
{
    m_fragmentColor = newFragmentColor;
}

bool DrawableMesh::toDrawNormalMap() const
{
    return m_toDrawNormalMap;
}

void DrawableMesh::setToDrawNormalMap(bool newToDrawNormalMap)
{
    m_toDrawNormalMap = newToDrawNormalMap;
}

DrawableMesh::~DrawableMesh(){
    delete m_shader;
    m_shader = nullptr;

    delete m_normals;
    m_normals = nullptr;

    delete m_vertices;
    m_vertices = nullptr;
}
