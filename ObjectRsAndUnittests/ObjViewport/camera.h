#ifndef CAMERA_H
#define CAMERA_H

#include <QMatrix4x4>

namespace ViewportTypes
{
class Camera
{
public:
    Camera();
    Camera(float _zNear, float _zFar);

    float m_scale = 1, m_zNear, m_zFar;
    QVector3D m_defaultCameraPosition = {0, 0, -1};
    QVector3D m_currentCameraPosition = m_defaultCameraPosition;

    void resetCamera();
    void move(QVector3D direction);
    void zoom(float dScale);
};

}
#endif // CAMERA_H
