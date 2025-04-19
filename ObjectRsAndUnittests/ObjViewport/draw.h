#ifndef DRAW_H
#define DRAW_H

#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QVector3D>
#include <QColor>

namespace GlDrawTools{
    QOpenGLShaderProgram* createShaderProgram(QString vertexShaderFilename, QString fragmentShaderFilename);
    QOpenGLBuffer* createVertexBuffer();
    QVector3D toRGBf(QColor color);
}

#endif // DRAW_H
