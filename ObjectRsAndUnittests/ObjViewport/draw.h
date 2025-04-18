#ifndef DRAW_H
#define DRAW_H

#include <QOpenGLShaderProgram>

namespace GlDrawTools{
    QOpenGLShaderProgram* createShaderProgram(QString vertexShaderFilename, QString fragmentShaderFilename);
}

#endif // DRAW_H
