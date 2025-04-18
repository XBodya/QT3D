#ifndef DRAWABLEGRID_H
#define DRAWABLEGRID_H

#include "drawableobject.h"
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QColor>
#include <QVector>
#include <QVector2D>

class DrawableGrid: public DrawableObject
{
public:
    DrawableGrid();
    virtual ~DrawableGrid();
    DrawableGrid(double size, int nSegments, QColor color);
    void calculateGrid();
    void draw(QMatrix4x4 viewMatrix, QMatrix4x4 projectionMatrix) override;
    void setShader(QString fragmentShaderFilename, QString vertexShaderFilename);
    QVector<QVector2D> getGrid2D();
    bool canToDraw();
protected:
    bool m_hasShaderToDraw = false;
    double m_size = 3.0;
    int m_nSegments = 10;
    QColor m_color = QColor(255, 255, 255);
    QVector<QVector2D> m_linesCoords;

    QOpenGLBuffer* m_vertices = nullptr;
    QOpenGLShaderProgram* m_shader = nullptr;


    //QOpenGLBuffer* m_vertices = nullptr;
    //QOpenGLShaderProgram* m_shader = nullptr;
};

#endif // DRAWABLEGRID_H
