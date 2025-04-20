#ifndef VIEWPORT_H
#define VIEWPORT_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QVector>
#include <QVector3D>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QMap>

#include "drawablegrid.h"
#include "drawableobject.h"

#include "camera.h"

class Viewport : public QOpenGLWidget
{
    Q_OBJECT;
public:
    Viewport(QWidget *parent = nullptr);
    ~Viewport();

    bool addObject(QVector<QVector3D> vertices, QVector<int> polygonVertexIndices, QVector<int> startPolygon);
    bool addObject(DrawableObject* obj);
    QOpenGLShaderProgram* createShaderProgram(QString vertexShaderFilename, QString fragmentShaderFilename);
    bool changeFragmentColor(QColor color);
    void changeRotateAngles(QVector3D dAngles);
    void setFitToView(QVector<QVector3D> &vertices);
    void updateFitToView();
    ViewportTypes::Camera m_camera;
    QMatrix4x4 m_modelViewMatrix, m_projectionMatrix;

    bool m_haveObjData = false;
    bool m_drawPixelByNormalCoords = false;
    QOpenGLBuffer* m_vertexBuffer = nullptr;
    QOpenGLBuffer* m_normalsBuffer = nullptr;
    QOpenGLShaderProgram* m_shader = nullptr;
    int m_nVertices = 0;
    QVector3D m_fragmentColor = {1.0f, 1.0f, 0.0f};
    QVector3D m_rotateAngles = {0.0f, 0.0f, 0.0f};
    QVector3D m_modelMaxXYZ, m_modelMinXYZ, m_modelCenter;


    float m_radiusOfModel = 0;

    QVector<DrawableObject *> drawableObjects() const;

    DrawableGrid *grid() const;

    QColor backgroundColor() const;
    void setBackgroundColor(const QColor &newBackgroundColor);

protected:
    QColor m_backgroundColor = QColor(128, 128, 128);
    QPoint m_currentCursorPosition;
    DrawableGrid* m_grid = nullptr;
    //QMap<QString, int> m_meshIndices;

    void initializeGL() override;
    void resizeGL(int width, int height) override;
    void paintGL() override;

    void rotateModelView(float degrees, QMatrix4x4 &modelViewMatrix);
    void rotateModelViewX(float degrees, QMatrix4x4 &modelViewMatrix);
    void rotateModelViewY(float degrees, QMatrix4x4 &modelViewMatrix);

    void mousePressEvent(QMouseEvent* mouseEvent) override;
    void mouseReleaseEvent(QMouseEvent* mouseEvent) override;
    void wheelEvent(QWheelEvent* event) override;

    QVector<DrawableObject*> m_drawableObjects;

};
#endif // VIEWPORT_H
