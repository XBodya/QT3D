#include "camera.h"

ViewportTypes::Camera::Camera() {
    m_zNear = m_zFar = 0.5;
};

ViewportTypes::Camera::Camera(float _zNear, float _zFar): m_zNear(_zNear), m_zFar(_zFar)
{

};

void ViewportTypes::Camera::resetCamera()
{
    m_currentCameraPosition = m_defaultCameraPosition;
    m_scale = 1;
}

void ViewportTypes::Camera::move(QVector3D direction)
{
    m_currentCameraPosition += direction;
}

void ViewportTypes::Camera::zoom(float dScale)
{
    m_scale = fmax(m_scale + dScale, 0);
}
