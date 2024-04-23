#pragma once
#include "Material.h"

#define M_PI 3.1415926

class Light
{
public:
    Light() = default;
    virtual ~Light() = default;

    virtual void lighting(const Material& material,
        const glm::vec3& position,
        const glm::vec3& normal,
        const glm::vec3& eyeDir,
        glm::vec3& ambient,
        glm::vec3& diffuse,
        glm::vec3& specular) const = 0;
};

//平行光
class DirectionalLight : public Light
{
public:
    glm::vec3 m_ambient;
    glm::vec3 m_diffuse;
    glm::vec3 m_specular;
    glm::vec3 m_direction;

    virtual void lighting(const Material& material,
        const glm::vec3& position,
        const glm::vec3& normal,
        const glm::vec3& eyeDir,
        glm::vec3& ambient,
        glm::vec3& diffuse,
        glm::vec3& specular) const;

    void setDirectionalLight(glm::vec3 _amb, glm::vec3 _diff, glm::vec3 _spec,
        glm::vec3 _dir)
    {
        m_ambient = _amb;
        m_diffuse = _diff;
        m_specular = _spec;
        m_direction = glm::normalize(_dir);
    }
};


//点光源
class PointLight : public Light
{
public:
    glm::vec3 m_ambient;
    glm::vec3 m_diffuse;
    glm::vec3 m_specular;
    glm::vec3 m_position;
    glm::vec3 m_attenuation;

    virtual void lighting(const Material& material,
        const glm::vec3& position,
        const glm::vec3& normal,
        const glm::vec3& eyeDir,
        glm::vec3& ambient,
        glm::vec3& diffuse,
        glm::vec3& specular) const;

    void setPointLight(glm::vec3 _amb, glm::vec3 _diff, glm::vec3 _spec,
        glm::vec3 _pos, glm::vec3 _atte)
    {
        m_ambient = _amb;
        m_diffuse = _diff;
        m_specular = _spec;
        m_position = _pos;
        m_attenuation = _atte;
    }
};

//聚束光
class SpotLight : public Light
{
public:
    double m_cutoff, m_outcutoff;
    glm::vec3 m_ambient;
    glm::vec3 m_diffuse;
    glm::vec3 m_specular;
    glm::vec3 m_position;
    glm::vec3 m_direction;
    glm::vec3 m_attenuation;

    virtual void lighting(const Material& material,
        const  glm::vec3& position,
        const  glm::vec3& normal,
        const  glm::vec3& eyeDir,
        glm::vec3& ambient,
        glm::vec3& diffuse,
        glm::vec3& specular) const;

    void setSpotLight(glm::vec3 _amb, glm::vec3 _diff, glm::vec3 _spec,
        double _cut, glm::vec3 _pos, glm::vec3 _dir, glm::vec3 _atte)
    {
        m_cutoff = cos(_cut * M_PI / 180.0);
        m_outcutoff = cos((_cut + 10.0) * M_PI / 180.0);
        m_ambient = _amb;
        m_diffuse = _diff;
        m_specular = _spec;
        m_position = _pos;
        m_direction = glm::normalize(_dir);
        m_attenuation = _atte;
    }
};
