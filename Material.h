#include "glm/glm.hpp"
#pragma once

class Material
{
public:
    Material() = default;
    ~Material() = default;
    double m_shininess;
    glm::vec3 m_ambient;
    glm::vec3 m_diffuse;
    glm::vec3 m_specular;
    glm::vec3 m_reflect;
    void setMaterial(glm::vec3 _amb, glm::vec3 _diff, glm::vec3 _spec, float _shin)
    {
        m_shininess = _shin;
        m_ambient = _amb;
        m_diffuse = _diff;
        m_specular = _spec;
    }
};

