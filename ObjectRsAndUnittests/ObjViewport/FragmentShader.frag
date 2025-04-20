uniform vec3 fragmentColor;
//uniform vec3 lightDir;

uniform bool drawPixelByNormalCoords;

varying vec3 m_vertex;
varying vec3 m_normal;

void main(void)
{
    vec3 lightDir = vec3(0.0, 0.0, -1.0);
    float diffuse = dot(m_normal, -lightDir);
    vec3 viewDir = normalize(m_vertex);
    vec3 reflectedLightDir = reflect(lightDir, m_normal);
    float specular = max(dot(reflectedLightDir, -viewDir), 0.0);
    float specularAmount = 0.05;
    float diffuseAmount = 0.75;
    float ambientAmount = 1.0 - diffuseAmount;
    if(drawPixelByNormalCoords)
    {
        vec3 normalColor = vec3(abs(m_normal[0]), abs(m_normal[1]), abs(m_normal[2]));
        //gl_FragColor = vec4(normalColor, 1);
        gl_FragColor = vec4(normalColor * ambientAmount + normalColor * diffuse * diffuseAmount + specular * specularAmount, 1.0);
    }
    else
    {
        gl_FragColor = vec4(fragmentColor * ambientAmount + fragmentColor * diffuse * diffuseAmount + specular * specularAmount, 1.0);
    }
}
