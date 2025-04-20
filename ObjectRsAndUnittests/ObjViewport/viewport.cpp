#include "viewport.h"
#include "mesh.h"
#include "draw.h"
#include "drawablegrid.h"

#include <QMatrix4x4>
#include <QString>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QCursor>
#include <QVector>

Viewport::Viewport(QWidget *parent)
    : QOpenGLWidget(parent)
{

}

Viewport::~Viewport()
{
    delete m_vertexBuffer;
    delete m_normalsBuffer;
    delete m_shader;
    delete m_grid;
    m_vertexBuffer = nullptr;
    m_normalsBuffer = nullptr;
    m_shader = nullptr;
    m_grid = nullptr;
}

void Viewport::initializeGL()
{
    // Я не знаю почему, но относительный путь для файлов не работает(((
    QString vertexShaderFilename = "C:\\Users\\games\\gits\\r3ds\\ObjectRsAndUnittests\\ObjViewport\\VertexShader.vert";
    QString fragmentShaderFilename = "C:\\Users\\games\\gits\\r3ds\\ObjectRsAndUnittests\\ObjViewport\\FragmentShader.frag";
    m_shader = createShaderProgram(vertexShaderFilename, fragmentShaderFilename);
    Q_ASSERT(m_shader != nullptr);

    m_grid = new DrawableGrid();
    m_grid->setShader("C:\\Users\\games\\gits\\QT3D\\ObjectRsAndUnittests\\ObjViewport\\GridVertexShader.vert",
                      "C:\\Users\\games\\gits\\QT3D\\ObjectRsAndUnittests\\ObjViewport\\GridFragmentShader.frag");
}

void Viewport::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
}

void Viewport::paintGL(){
    QVector4D backColor = GlDrawTools::toRGBAf(m_backgroundColor);
    glClearColor(backColor[0], backColor[1], backColor[2], backColor[3]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if(!m_haveObjData)
        return;

    m_modelViewMatrix = QMatrix4x4 ();
    m_modelViewMatrix.scale(m_camera.m_scale);
    m_modelViewMatrix.translate(m_camera.m_currentCameraPosition[0], m_camera.m_currentCameraPosition[1], m_camera.m_currentCameraPosition[2]);

    m_projectionMatrix = QMatrix4x4 ();
    Q_ASSERT((width() != 0) && (height() != 0));

    QMatrix4x4 rotation = QMatrix4x4();

    rotation.rotate(m_rotateAngles[0], 1.0f, 0.0f, 0.0f);
    rotation.rotate(m_rotateAngles[1], 0.0f, 1.0f, 0.0f);
    rotation.rotate(m_rotateAngles[2], 0.0f, 0.0f, 1.0f);

    m_modelViewMatrix = m_modelViewMatrix * rotation;

    float screenAspectRatio = width() / float(height());
    //m_projectionMatrix.ortho(-1, 1, -1 / screenAspectRatio, 1 / screenAspectRatio, m_camera.m_zNear, m_camera.m_zFar);
    m_projectionMatrix.ortho(-10, 10, -10 / screenAspectRatio, 10 / screenAspectRatio, -10, 10);
    // projectionMatrix.ortho(-1, 1, -1 / screenAspectRatio, 1 / screenAspectRatio, 0.5, 0.5);

    //QTextStream out(stdout);
    //out << "Curpos and scale:\n";
    //out << m_camera.m_currentCameraPosition.x() << ' ' << m_camera.m_currentCameraPosition.y() << ' ' << m_camera.m_currentCameraPosition.z() << '\n';
    //out << " " << ' ' << m_camera.m_scale << '\n';

    m_grid->draw(m_modelViewMatrix, m_projectionMatrix);
    for(auto it: m_drawableObjects){
        it->draw(m_modelViewMatrix, m_projectionMatrix);
    }


    // if(!m_shader->bind())
    //     qFatal("Cannot bind shader");

    // m_shader->setUniformValue("modelViewMatrix", m_modelViewMatrix);
    // m_shader->setUniformValue("projectionMatrix", m_projectionMatrix);
    // m_shader->setUniformValue("fragmentColor", m_fragmentColor);
    // //m_shader->setUniformValue("lightDir", m_camera.m_currentCameraPosition);
    // m_shader->setUniformValue("drawPixelByNormalCoords", m_drawPixelByNormalCoords);

    // //QTextStream out(stdout);
    // //out << haveObjData << '\n';

    // if(!m_vertexBuffer->bind())
    //     qFatal("Cannot bind vertex buffer");

    // m_shader->setAttributeBuffer("vertex", GL_FLOAT, 0, 3, 0);
    // m_shader->enableAttributeArray("vertex");
    // m_vertexBuffer->release();

    // if(!m_normalsBuffer->bind())
    //     qFatal("Cannot bind normals buffer");

    // m_shader->setAttributeBuffer("normal", GL_FLOAT, 0, 3, 0);
    // m_shader->enableAttributeArray("normal");
    // m_normalsBuffer->release();

    // glDrawArrays(GL_TRIANGLES, 0, m_nVertices);

    // m_shader->disableAttributeArray("vertex");
    // m_shader->disableAttributeArray("normal");
    // m_shader->release();
}

bool Viewport::addObject(QVector<QVector3D> vertices, QVector<int> polygonVertexIndices, QVector<int> startPolygon)
{
    QVector<int> triangleVertexIndices = MeshTools::buildTriangleVertexIndices(polygonVertexIndices, startPolygon);
    QVector<float> triangleVertexCoords = MeshTools::packTriangleVertexCoords(vertices, triangleVertexIndices);
    QVector<float> triangleNormalsCoords = MeshTools::buildAndPackTriangleNormalsCoords(vertices, triangleVertexIndices);

    m_vertexBuffer = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    if(!m_vertexBuffer->create())
        return false;
    if(!m_vertexBuffer->bind())
        return false;

    m_nVertices = triangleVertexCoords.size() / 3;
    int dataSize = triangleVertexCoords.size() * sizeof(float);

    m_vertexBuffer->allocate(triangleVertexCoords.constData(), dataSize);
    m_vertexBuffer->release();

    m_haveObjData = true;

    m_normalsBuffer = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    if(!m_normalsBuffer->create())
        return false;
    if(!m_normalsBuffer->bind())
        return false;
    m_normalsBuffer->allocate(triangleNormalsCoords.constData(), dataSize);
    m_normalsBuffer->release();


    m_camera.resetCamera();
    setFitToView(vertices);
    return true;
}

bool Viewport::addObject(DrawableObject* obj)
{
    if(obj == nullptr)
        return false;
    m_drawableObjects.append(obj);
    m_haveObjData = true;
    m_camera.resetCamera();
    return true;
}

QOpenGLShaderProgram *Viewport::createShaderProgram(QString vertexShaderFilename, QString fragmentShaderFilename)
{
    return GlDrawTools::createShaderProgram(vertexShaderFilename, fragmentShaderFilename);
}

bool Viewport::changeFragmentColor(QColor color)
{
    if(!color.isValid())
        return false;
    m_fragmentColor = QVector3D(color.redF(), color.greenF(), color.blueF());
    return true;
}

void Viewport::rotateModelView(float degrees, QMatrix4x4 &modelViewMatrix)
{
    QMatrix4x4 rotation = QMatrix4x4();
    rotation.rotate(degrees, 0.0f, 0.0f, 1.0f);
    modelViewMatrix = modelViewMatrix * rotation;
}

void Viewport::rotateModelViewX(float degrees, QMatrix4x4 &modelViewMatrix)
{
    QMatrix4x4 rotation = QMatrix4x4();
    rotation.rotate(degrees, 1.0f, 0.0f, 0.0f);
    modelViewMatrix = modelViewMatrix * rotation;
}

void Viewport::rotateModelViewY(float degrees, QMatrix4x4 &modelViewMatrix)
{
    QMatrix4x4 rotation = QMatrix4x4();
    rotation.rotate(degrees, 0.0f, 1.0f, 0.0f);
    modelViewMatrix = modelViewMatrix * rotation;
}

void Viewport::mousePressEvent(QMouseEvent* mouseEvent)
{
    if(mouseEvent->button() == Qt::LeftButton)
    {
        m_currentCursorPosition = QCursor::pos();
    }
}

void Viewport::mouseReleaseEvent(QMouseEvent* mouseEvent)
{
    QPoint currentCursorPosition = QCursor::pos();
    QTextStream out(stdout);
    QVector2D normalizedDeltaCursorPos = QVector2D((m_currentCursorPosition - currentCursorPosition));
    normalizedDeltaCursorPos[0] = (normalizedDeltaCursorPos[0] / this->size().width());
    normalizedDeltaCursorPos[1] = normalizedDeltaCursorPos[1] / this->size().height();
    if(mouseEvent->modifiers() == Qt::ControlModifier)
    {
        if(mouseEvent->button() == Qt::LeftButton){
            out << QVector3D(normalizedDeltaCursorPos, 0)[0]<< ' ' << QVector3D(normalizedDeltaCursorPos, 0)[1] << ' ' << QVector3D(normalizedDeltaCursorPos, 0)[2] << '\n';
            this->m_camera.move(QVector3D(-normalizedDeltaCursorPos[0], normalizedDeltaCursorPos[1], 0));
        }
    }
    if(mouseEvent->modifiers() == Qt::AltModifier)
    {
        if(mouseEvent->button() == Qt::LeftButton){
            out << "2\n";
            this->changeRotateAngles(QVector3D(-180.0 * normalizedDeltaCursorPos.y(), -180.0 * normalizedDeltaCursorPos.x(), 0));
        }
    }
    this->makeCurrent();
    this->update();
}

void Viewport::wheelEvent(QWheelEvent* event)
{
    QPoint delta = event->angleDelta();
    if(delta.y() > 0)
    {
        m_camera.zoom(0.1);
    }
    else if(delta.y() < 0)
    {
        m_camera.zoom(-0.1);
    }
    this->makeCurrent();
    this->update();
}

QVector<DrawableObject *> Viewport::drawableObjects() const
{
    return m_drawableObjects;
}

DrawableGrid *Viewport::grid() const
{
    return m_grid;
}

QColor Viewport::backgroundColor() const
{
    return m_backgroundColor;
}

void Viewport::setBackgroundColor(const QColor &newBackgroundColor)
{
    m_backgroundColor = newBackgroundColor;
}

void Viewport::setFitToView(QVector<QVector3D> &vertices)
{
    if(!m_haveObjData)
        return;

    MeshTools::getBoundingBox(vertices, m_modelMaxXYZ, m_modelMinXYZ);
    m_modelCenter = MeshTools::getCentreBoundingBox(m_modelMaxXYZ, m_modelMinXYZ);
    m_radiusOfModel = MeshTools::getRadiusBoundingBox(m_modelMaxXYZ, m_modelCenter);

    //qDebug() << m_modelMaxXYZ.x() << ' ' << m_modelMaxXYZ.y() << ' ' << m_modelMaxXYZ.z() << '\n';
    //qDebug() << m_modelMinXYZ.x() << ' ' << m_modelMinXYZ.y() << ' ' << m_modelMinXYZ.z() << '\n';
    //qDebug() << m_modelCenter.x() << ' ' << m_modelCenter.y() << ' ' << m_modelCenter.z() << '\n';
    //qDebug() << m_radiusOfModel << '\n';

    //updateFitToView();
}

void Viewport::updateFitToView()
{
    if(!m_haveObjData)
        return;


    float widthScale = width() / (m_modelMaxXYZ.x() - m_modelMinXYZ.x()),
        heightScale = height() / (m_modelMaxXYZ.y() - m_modelMinXYZ.y());

    //m_camera.m_scale = fmin(widthScale, heightScale);
    QVector3D cameraNewPosition = m_modelCenter;
    //cameraNewPosition[2] -= m_radiusOfModel;
    cameraNewPosition[1] *=- 1;
    cameraNewPosition[2] -= m_radiusOfModel;
    m_camera.m_currentCameraPosition = cameraNewPosition;

    this->makeCurrent();
    this->update();



    QTextStream out(stdout);
    out << cameraNewPosition.x() << ' ' << cameraNewPosition.y() << ' ' << cameraNewPosition.z() << '\n';
    out << fmin(widthScale, heightScale) << ' ' << m_camera.m_scale << ' ' << m_radiusOfModel << '\n';

}

void Viewport::changeRotateAngles(QVector3D dAngles)
{

    m_rotateAngles += dAngles;
    for(int i = 0; i < 3; ++i)
    {
        if (fabs(m_rotateAngles[i]) > 360.0f){
            m_rotateAngles[i] = m_rotateAngles[i] - float((((int)(m_rotateAngles[i] / 360.0f)) * 360));
        }
    }

    for(int i = 0; i < 3; ++i)
    {
        if (m_rotateAngles[i] < 0.0f){
            m_rotateAngles[i] = 360.0f - m_rotateAngles[i];
        }
    }
}

