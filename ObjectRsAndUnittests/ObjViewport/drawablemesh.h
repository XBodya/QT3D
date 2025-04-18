#ifndef DRAWABLEMESH_H
#define DRAWABLEMESH_H

#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

#include <QMatrix4x4>

#include "drawableobject.h"

class DrawableMesh: public DrawableObject
{
public:
    DrawableMesh(QString fragmentShaderFilename, QString vertexShaderFilename);
    virtual ~DrawableMesh();
    void draw(QMatrix4x4 viewMatrix, QMatrix4x4 projectionMatrix) override;
protected:
    int m_nVertices = -1;

    QOpenGLBuffer* m_vertices = nullptr;
    QOpenGLBuffer* m_normals = nullptr;
    QOpenGLShaderProgram* m_shader = nullptr;
};

#endif // DRAWABLEMESH_H
