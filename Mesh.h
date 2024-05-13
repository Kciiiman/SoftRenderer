#ifndef __MESH_H__
#define __MESH_H__

#include <vector>
#include "glm.hpp"

class RawVertex {
public:
	glm::vec4 position;
	glm::vec4 color;
	glm::vec2 texCoord;
	glm::vec3 normal;

	RawVertex() = default;
	~RawVertex() = default;

	RawVertex(
		const glm::vec4& _pos,
		const glm::vec4& _color = glm::vec4(255, 255, 255, 255),
		const glm::vec2& _tex = glm::vec2(0, 0),
		const glm::vec3& _normal = glm::vec3(0, 0, 1)
	) : position(_pos), color(_color), texCoord(_tex), normal(_normal) { }
	RawVertex(
		const glm::vec3& _pos,
		const glm::vec2& _tex = glm::vec2(0, 0),
		const glm::vec3& _normal = glm::vec3(0, 0, 1),
		const glm::vec4& _color = glm::vec4(255, 255, 255, 255)
	) : position(glm::vec4(_pos, 1.0f)), texCoord(_tex), normal(_normal), color(_color) { }
};

class Vertex {
public:
	glm::vec4 worldPos;		//世界变换后坐标
	glm::vec4 windowPos;	//投影变换后坐标
	glm::vec4 color;
	glm::vec2 texCoord;
	glm::vec3 normal;
	double oneDivZ;     //1/z用于深度测试

	Vertex() = default;
	~Vertex() = default;

	Vertex(
		const glm::vec4& _worldPos,
		const glm::vec4& _windowPos,
		const glm::vec4& _color,
		const glm::vec2& _texCoord,
		const glm::vec3& _normal,
		const double& _oneDivZ
	) : worldPos(_worldPos), windowPos(_windowPos), color(_color), texCoord(_texCoord), normal(_normal), oneDivZ(_oneDivZ) { }

	Vertex(const Vertex& v) : worldPos(v.worldPos), windowPos(v.windowPos), color(v.color), texCoord(v.texCoord), normal(v.normal), oneDivZ(v.oneDivZ) { }

};

class Mesh {
public:
	std::vector<RawVertex> VBO; //顶点缓冲
	std::vector<unsigned int> EBO;	//顶点索引

	Mesh() = default;
	Mesh(const int& vNum, const int iNum);
	Mesh(const Mesh& mesh) : VBO(mesh.VBO), EBO(mesh.EBO) { }
	~Mesh() = default;

	Mesh& operator=(const Mesh& mesh);
	Mesh& operator+=(const Mesh& mesh);
	void addMesh(const Mesh& mesh);
	void asBox(double width, double height, double depth);
	void asFloor(double length, double height);
	void asTriangle(RawVertex p1, RawVertex p2, RawVertex p3);

};
Mesh createPlane(const glm::vec3& leftTop, const glm::vec3& leftBottom, const glm::vec3& rightBottom, const glm::vec3& rightTop, const glm::vec3& normal);
Mesh createBox(const glm::vec3& center, double radius);


#endif // !__MESH_H__

