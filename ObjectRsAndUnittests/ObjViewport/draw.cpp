#include "draw.h"

QOpenGLShaderProgram* GlDrawTools::createShaderProgram(QString vertexShaderFilename, QString fragmentShaderFilename)
{
    QOpenGLShaderProgram *shader = new QOpenGLShaderProgram();
    if(!shader->addShaderFromSourceFile(QOpenGLShader::Vertex, vertexShaderFilename))
    {
        delete shader;
        return nullptr;
    }
    if(!shader->addShaderFromSourceFile(QOpenGLShader::Fragment, fragmentShaderFilename))
    {
        delete shader;
        return nullptr;
    }
    if(!shader->link())
    {
        delete shader;
        return nullptr;
    }
    return shader;
}


QVector3D GlDrawTools::toRGBf(QColor color)
{
    return QVector3D(color.redF(), color.greenF(), color.blueF());
}
