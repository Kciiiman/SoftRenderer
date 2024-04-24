#include "Shader.h"
#include "Texture.h"
#include "Light.h"
#include <iostream>


BaseShader::BaseShader() {
	modelMatrix = glm::mat4(1.0f);
	viewMatrix = glm::mat4(1.0f);
	projectMatrix = glm::mat4(1.0f);
	texture = nullptr;
}

Vertex BaseShader::vertexShader(const RawVertex& v) {
	Vertex result;
	result.worldPos = modelMatrix * v.position;
	result.windowPos = projectMatrix * viewMatrix * result.worldPos;
	result.color = v.color;
	result.texCoord = v.texCoord;
	result.normal = v.normal;

	result.oneDivZ = 1.0 / result.windowPos.w;
	result.worldPos *= result.oneDivZ;
	result.color *= result.oneDivZ;
	result.texCoord *= result.oneDivZ;

	return result;
}

glm::vec4 BaseShader::fragmentShader(const Vertex& v2f) {
	if (texture == nullptr)
	{
		return v2f.color;
	}
	else
		return texture->sample2D(v2f.texCoord);
}

void BaseShader::bindTexture(Texture* texture)
{
	this->texture = texture;
}


void BaseShader::setModelMatrix(const glm::mat4& m) {
	modelMatrix = m;
}

void BaseShader::setViewMatrix(const glm::mat4& m) {
	viewMatrix = m;
}

void BaseShader::setProjectMatrix(const glm::mat4& m) {
	projectMatrix = m;
}



/********************************双线性模型***********************************/
GouraudShader::GouraudShader()
{
	m_texture = nullptr;
	m_light = nullptr;
	m_material = nullptr;
	modelMatrix = glm::mat4(1.0f);
	viewMatrix = glm::mat4(1.0f);
	projectMatrix = glm::mat4(1.0f);
}

Vertex GouraudShader::vertexShader(const RawVertex& v)
{
	Vertex result;
	result.worldPos = modelMatrix * v.position;
	result.windowPos = projectMatrix * viewMatrix * result.worldPos;
	result.color = v.color;
	result.texCoord = v.texCoord;
	result.normal = invModelMatrix * glm::vec4(v.normal,1.0f);

	// Gouraud shading.
	if (m_texture)
		result.color = m_texture->sample2D(result.texCoord);
	glm::vec3 _amb, _diff, _spec;
	if (m_light)
	{
		glm::vec3 eyeDir = m_eyePos - glm::vec3(result.worldPos);
		eyeDir = glm::normalize(eyeDir);
		m_light->lighting(*m_material,
			result.worldPos,
			result.normal,
			eyeDir,
			_amb,
			_diff,
			_spec);

		result.color.x *= (_amb.x + _diff.x + _spec.x);
		result.color.y *= (_amb.y + _diff.y + _spec.y);
		result.color.z *= (_amb.z + _diff.z + _spec.z);
		result.color.w = 1.0f;
	}

	result.oneDivZ = 1.0 / result.windowPos.w;
	result.worldPos *= result.oneDivZ;
	result.texCoord*= result.oneDivZ;
	result.color *= result.oneDivZ;
	return result;
}

glm::vec4 GouraudShader::fragmentShader(const Vertex& v2f)
{
	glm::vec4 litColor = v2f.color;
	return litColor;
}

void GouraudShader::bindTexture(Texture* texture)
{
	this->m_texture = texture;
}



/********************************phong模型***********************************/
PhongShader::PhongShader()
{
	m_texture = nullptr;
	m_light = nullptr;
	m_material = nullptr;
	modelMatrix = glm::mat4(1.0f);
	viewMatrix = glm::mat4(1.0f);
	projectMatrix = glm::mat4(1.0f);
	invModelMatrix = glm::mat4(1.0f);
}

Vertex PhongShader::vertexShader(const RawVertex& v)
{
	Vertex result;
	result.worldPos = modelMatrix * v.position;
	result.windowPos = projectMatrix * viewMatrix * result.worldPos;
	result.color = v.color;
	result.texCoord = v.texCoord;
	result.normal = invModelMatrix * glm::vec4(v.normal,1.0f);

	// oneDivZ to correct mapping.
	result.oneDivZ = 1.0 / result.windowPos.w;
	result.worldPos *= result.oneDivZ;
	result.texCoord *= result.oneDivZ;
	result.color *= result.oneDivZ;
	return result;
}

glm::vec4 PhongShader::fragmentShader(const Vertex& v2f)
{
	glm::vec4 litColor = v2f.color;

	// Gouraud shading.
	if (m_texture)
		
		litColor = m_texture->sample2D(v2f.texCoord);
	glm::vec3 _amb, _diff, _spec;
	if (m_light)
	{
		glm::vec3 eyeDir = m_eyePos - glm::vec3(v2f.worldPos);
		eyeDir = glm::normalize(eyeDir);
		m_light->lighting(*m_material,
			v2f.worldPos,
			v2f.normal,
			eyeDir,
			_amb,
			_diff,
			_spec);

		litColor.x *= (_amb.x + _diff.x + _spec.x);
		litColor.y *= (_amb.y + _diff.y + _spec.y);
		litColor.z *= (_amb.z + _diff.z + _spec.z);
		litColor.w = 255.0f;
	}
	return litColor;
}







