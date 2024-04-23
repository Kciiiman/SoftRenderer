#include "Light.h"

void DirectionalLight::lighting(const Material& material,
                                const glm::vec3& position,
                                const glm::vec3& normal,
                                const glm::vec3& eyeDir,
                                glm::vec3& ambient,
                                glm::vec3& diffuse,
                                glm::vec3& specular) const
{

    float diff = glm::max(glm::dot(normal, -this->m_direction), 0.0f);
    glm::vec3 halfwayDir = eyeDir + this->m_direction;
    halfwayDir = glm::normalize(halfwayDir);
    float spec = pow(glm::max(glm::dot(eyeDir, halfwayDir), 0.0f), material.m_shininess);
    ambient = m_ambient;
    diffuse = m_diffuse * diff;
    specular = m_specular * spec;
}

void PointLight::lighting(const Material& material,
    const glm::vec3& position,
    const glm::vec3& normal,
    const glm::vec3& eyeDir,
    glm::vec3& ambient,
    glm::vec3& diffuse,
    glm::vec3& specular) const
{
    // ambient
    ambient = this->m_ambient;

    // diffuse
    glm::vec3 lightDir = (this->m_position - position);
    lightDir = glm::normalize(lightDir);
    
    float diff = glm::max(glm::dot(normal, lightDir), 0.0f);
    diffuse = this->m_diffuse * diff;

    // specular
    glm::vec3 halfwayDir = eyeDir + lightDir;
    halfwayDir = glm::normalize(halfwayDir);
    float spec = pow(glm::max(glm::dot(eyeDir, halfwayDir), 0.0f), material.m_shininess);
    specular = this->m_specular * spec;

    // attenuation
    float distance = glm::length(this->m_position - position);
    float attenuation = 1.0 / (m_attenuation.x +
        m_attenuation.y * distance +
        m_attenuation.z * (distance * distance));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
}

void SpotLight::lighting(const Material& material,
    const glm::vec3& position,
    const glm::vec3& normal,
    const glm::vec3& eyeDir,
    glm::vec3& ambient,
    glm::vec3& diffuse,
    glm::vec3& specular) const
{
    // ambient
    ambient = this->m_ambient;

    // diffuse
    glm::vec3 lightDir = this->m_position - position;
    lightDir = glm::normalize(lightDir);
    float diff = glm::max(glm::dot(normal, lightDir), 0.0f);
    diffuse = this->m_diffuse * diff;

    // specular
    glm::vec3 halfwayDir = eyeDir + lightDir;
    halfwayDir = glm::normalize(halfwayDir);
    float spec = pow(glm::max(glm::dot(eyeDir, halfwayDir), 0.0f), material.m_shininess);
    specular = this->m_specular * spec;

    // spotlight (soft edges)
    float theta = glm::dot(lightDir, -this->m_direction);
    float epsilon = (this->m_cutoff - this->m_outcutoff);
    float intensity = (theta - this->m_outcutoff) / epsilon;
    if (intensity < 0.0f)intensity = 0.0f;
    if (intensity > 1.0f)intensity = 1.0f;
    diffuse *= intensity;
    specular *= intensity;

    // attenuation
    float distance = glm::length(this->m_position - position);
    float attenuation = 1.0 / (m_attenuation.x +
        m_attenuation.y * distance +
        m_attenuation.z * (distance * distance));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
}