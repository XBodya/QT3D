#ifndef DRAWABLEOBJECT_H
#define DRAWABLEOBJECT_H


#include <QMatrix4x4>

class DrawableObject
{
public:
    DrawableObject();
    //virtual ~DrawableObject() = 0;
    virtual void draw(QMatrix4x4 viewMatrix, QMatrix4x4 projectionMatrix) = 0;

};

#endif // DRAWABLEOBJECT_H
