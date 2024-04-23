#pragma once
#include <glm/glm.hpp>
#include <string>
#include "Math.h"

class Material;
class Texture;
class Light;

class Shader
{
public:
	Shader() = default;
	~Shader() = default;

	virtual Vertex vertexShader(const RawVertex& v) = 0;
	virtual glm::vec4 fragmentShader(const Vertex& v2f) = 0;

	virtual void bindTexture(Texture* texture) = 0;

	virtual void setModelMatrix(const glm::mat4& m) = 0;
	virtual void setViewMatrix(const glm::mat4& m) = 0;
	virtual void setProjectMatrix(const glm::mat4& m) = 0;

	virtual void setMaterial(const Material* material) = 0;
	virtual void setLight(const Light* light) = 0;
	virtual void setEyePos(const glm::vec3 eye) = 0;
};

class BaseShader :public Shader{
private:
	glm::mat4 modelMatrix;
	glm::mat4 viewMatrix;
	glm::mat4 projectMatrix;
	Texture* texture;
public:
	BaseShader();
	~BaseShader() = default;

	virtual Vertex vertexShader(const RawVertex& v) override;
	virtual glm::vec4 fragmentShader(const Vertex& v2f) override;

	virtual void bindTexture(Texture* texture) override;

	virtual void setModelMatrix(const glm::mat4& m) override;
	virtual void setViewMatrix(const glm::mat4& m) override;
	virtual void setProjectMatrix(const glm::mat4& m) override;

	virtual void setMaterial(const Material* material) override {}
	virtual void setLight(const Light* light) override {}
	virtual void setEyePos(const glm::vec3 eye) override {}
};
//双线性
class GouraudShader :public Shader
{
private:
	const Light* m_light;
	const Material* m_material;
	Texture* m_texture;
	glm::vec3  m_eyePos;
	glm::mat4 modelMatrix;
	glm::mat4 invModelMatrix;
	glm::mat4 viewMatrix;
	glm::mat4 projectMatrix;
public:
	GouraudShader();
	~GouraudShader() = default;

	virtual Vertex vertexShader(const RawVertex& v) override;
	virtual glm::vec4 fragmentShader(const Vertex& v2f) override;

	virtual void bindTexture(Texture* texture) override;

	virtual void setModelMatrix(const glm::mat4& m) override { modelMatrix = m; }
	virtual void setViewMatrix(const glm::mat4& m) override { viewMatrix = m; }
	virtual void setProjectMatrix(const glm::mat4& m) override { projectMatrix = m; }

	virtual void setMaterial(const Material* material) override { m_material = material; }
	virtual void setLight(const Light* light) override { m_light = light; }
	virtual void setEyePos(const glm::vec3 eye) override { m_eyePos = eye; }
};
//phong模型
class PhongShader :public Shader
{
private:
	const Light* m_light;
	const Material* m_material;
	Texture* m_texture;
	glm::vec3  m_eyePos;
	glm::mat4 modelMatrix;
	glm::mat4 invModelMatrix;
	glm::mat4 viewMatrix;
	glm::mat4 projectMatrix;
public:
	PhongShader();
	~PhongShader() = default;

	virtual Vertex vertexShader(const RawVertex& v) override;
	virtual glm::vec4 fragmentShader(const Vertex& v2f) override;

	virtual void bindTexture(Texture* texture) override { m_texture = texture; }

	virtual void setModelMatrix(const glm::mat4& m) override { modelMatrix = m; }
	virtual void setViewMatrix(const glm::mat4& m) override { viewMatrix = m; }
	virtual void setProjectMatrix(const glm::mat4& m) override { projectMatrix = m; }

	virtual void setMaterial(const Material* material) override { m_material = material; }
	virtual void setLight(const Light* light) override { m_light = light; }
	virtual void setEyePos(const glm::vec3 eye) override { m_eyePos = eye; }
};
