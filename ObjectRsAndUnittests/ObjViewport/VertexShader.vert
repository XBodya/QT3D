uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;

attribute vec3 vertex;
attribute vec3 normal;

varying vec3 m_vertex;
varying vec3 m_normal;

void main(void)
{
    m_vertex = vertex;
    m_normal = normal;
    gl_Position = projectionMatrix * modelViewMatrix * vec4(vertex, 1.0);
}
