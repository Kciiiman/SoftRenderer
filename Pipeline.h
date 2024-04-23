#pragma once
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <iostream>
#include <string>
#include "Texture.h"
#include "Shader.h"
#include "Model.h"
#include "FrameBuffer.h"
#include "Light.h"
#include "Camera.h"
#include "glfw3.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"


enum ShadingMode
{
	Simple = 0,
	Gouraud = 1,
	Phong = 2
};

enum class RenderMode {
	LINE, MESH
};

class Pipeline {
private:
	Camera* m_Camera;
	int width;
	int height;
	glm::vec3 m_eyePos;
	RenderMode mode;
	FrameBuffer* m_frontBuffer;
	FrameBuffer* m_backBuffer;
	Shader* shader;
	const Material* m_material;
	std::vector<Texture*> m_Textures;
	std::vector<Model*> m_Models;
	glm::mat4 viewPortMatrix;
	DirectionalLight m_dirLight;
	PointLight m_pointLight;
	SpotLight m_spotLight;


public:
	Pipeline(int width, int height, RenderMode m);
	~Pipeline();

	void init();
	void display();

	void resize(const int& w, const int& h);
	void fillColorBuffer(const glm::vec4& color);
	void swapBuffer();

	unsigned int loadTexture(const std::string& path);
	bool bindTexture(const unsigned int& unit);
	bool unbindTexture(const unsigned int& unit);

	void setMaterial(const Material* material);

	void setModelMatrix(const glm::mat4& m);
	void setViewMatrix(const glm::mat4& m);
	void setProjectMatrix(const glm::mat4& m);

	void setDirectionLight(glm::vec3 dir, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec);
	void setPointLight(glm::vec3 pos, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, glm::vec3 atte);
	void setSpotLight(glm::vec3 pos, glm::vec3 dir, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, glm::vec3 atte, double cut);

	void setCamera(Camera* camera);
	void updateCamera();

	void add(Model* object);
	void drawObject(const Model* obj);

	//now
	void drawIndex(RenderMode mode, const Mesh* mesh);
	void setShaderMode(ShadingMode ShaderMode);

private:
	void bresenhamLineRasterization(const Vertex& from, const Vertex& to);
	void scanLinePerRow(const Vertex& left, const Vertex& right);
	void rasterTopTriangle(Vertex& v1, Vertex& v2, Vertex& v3);
	void rasterBottomTriangle(Vertex& v1, Vertex& v2, Vertex& v3);
	void edgeWalkingFillRasterization(const Vertex& v1, const Vertex& v2, const Vertex& v3);

};



