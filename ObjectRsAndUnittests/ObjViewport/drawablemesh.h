#ifndef DRAWABLEMESH_H
#define DRAWABLEMESH_H

#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

#include <QMatrix4x4>

#include <QVector>
#include <QVector3D>
#include <QColor>

#include "drawableobject.h"

class DrawableMesh: public DrawableObject
{
public:
    DrawableMesh(QString vertexShaderFilename, QString fragmentShaderFilename,
                 QVector<float> triangleVertexCoords, QVector<float> triangleNormalCoords);
    virtual ~DrawableMesh();
    void draw(QMatrix4x4 viewMatrix, QMatrix4x4 projectionMatrix) override;

    QColor fragmentColor() const;
    void setFragmentColor(const QColor &newFragmentColor);

    bool toDrawNormalMap() const;
    void setToDrawNormalMap(bool newToDrawNormalMap);

protected:
    int m_nVertices = -1;

    QOpenGLBuffer* m_vertices = nullptr;
    QOpenGLBuffer* m_normals = nullptr;
    QOpenGLShaderProgram* m_shader = nullptr;

    QColor m_fragmentColor = QColor(0, 255, 0);
    bool m_toDrawNormalMap = false;
};

#endif // DRAWABLEMESH_H
