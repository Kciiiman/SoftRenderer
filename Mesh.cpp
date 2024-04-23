#include "Mesh.h"
Mesh::Mesh(const int& vNum, const int iNum) {
	VBO.resize(vNum);
	EBO.resize(iNum);
}

Mesh& Mesh::operator=(const Mesh& mesh) {
	if (&mesh != this) {
		return *this;
	}
	VBO = mesh.VBO;
	EBO = mesh.EBO;
	return *this;
}

Mesh& Mesh::operator+=(const Mesh& mesh) {
	addMesh(mesh);
	return *this;
}

void Mesh::addMesh(const Mesh& mesh) {
	int offset = VBO.size();
	VBO.insert(VBO.end(), mesh.VBO.begin(), mesh.VBO.end());
	EBO.reserve(EBO.size() + mesh.EBO.size());
	for (int i = 0; i < mesh.EBO.size(); i++) {
		EBO.push_back(offset + mesh.EBO[i]);
	}
}


Mesh createPlane(const glm::vec3& rightTop, const glm::vec3& leftTop, const glm::vec3& leftBottom, const glm::vec3& rightBottom, const glm::vec3& normal) {
	Mesh result(4, 6);
	result.VBO[0].position = glm::vec4(rightTop, 1.0f);
	result.VBO[0].normal = normal;
	result.VBO[0].color = glm::vec4(255, 0, 0, 255);
	result.VBO[0].texCoord = glm::vec2(1.0f, 1.0f);

	result.VBO[1].position = glm::vec4(leftTop, 1.0f);
	result.VBO[1].normal = normal;
	result.VBO[1].color = glm::vec4(0, 255, 0, 255);
	result.VBO[1].texCoord = glm::vec2(0.0f, 1.0f);

	result.VBO[2].position = glm::vec4(leftBottom, 1.0f);
	result.VBO[2].normal = normal;
	result.VBO[2].color = glm::vec4(0, 0, 255, 255);
	result.VBO[2].texCoord = glm::vec2(0.0f, 0.0f);

	result.VBO[3].position = glm::vec4(rightBottom, 1.0f);
	result.VBO[3].normal = normal;
	result.VBO[3].color = glm::vec4(255, 0, 255, 255);
	result.VBO[3].texCoord = glm::vec2(1.0f, 0.0f);

	result.EBO[0] = 0;
	result.EBO[1] = 1;
	result.EBO[2] = 2;
	result.EBO[3] = 0;
	result.EBO[4] = 2;
	result.EBO[5] = 3;

	return result;
}

Mesh createBox(const glm::vec3& center, float radius) {
	Mesh result;

	Mesh front = createPlane(
		center + glm::vec3(radius, radius, radius),
		center + glm::vec3(-radius, radius, radius),
		center + glm::vec3(-radius, -radius, radius),
		center + glm::vec3(radius, -radius, radius),
		glm::vec3(0, 0, 1)
	);
	result.addMesh(front);

	Mesh left = createPlane(
		center + glm::vec3(-radius, radius, radius),
		center + glm::vec3(-radius, radius, -radius),
		center + glm::vec3(-radius, -radius, -radius),
		center + glm::vec3(-radius, -radius, radius),
		glm::vec3(-1, 0, 0)
	);
	result.addMesh(left);

	Mesh right = createPlane(
		center + glm::vec3(radius, -radius, radius),
		center + glm::vec3(radius, -radius, -radius),
		center + glm::vec3(radius, radius, -radius),
		center + glm::vec3(radius, radius, radius),
		glm::vec3(1, 0, 0)
	);
	result.addMesh(right);

	Mesh back = createPlane(
		center + glm::vec3(radius, -radius, -radius),
		center + glm::vec3(-radius, -radius, -radius),
		center + glm::vec3(-radius, radius, -radius),
		center + glm::vec3(radius, radius, -radius),
		glm::vec3(0, 0, -1)
	);
	result.addMesh(back);

	Mesh up = createPlane(
		center + glm::vec3(-radius, radius, radius),
		center + glm::vec3(radius, radius, radius),
		center + glm::vec3(radius, radius, -radius),
		center + glm::vec3(-radius, radius, -radius),
		glm::vec3(0, 1, 0)
	);
	result.addMesh(up);

	Mesh down = createPlane(
		center + glm::vec3(-radius, -radius, radius),
		center + glm::vec3(-radius, -radius, -radius),
		center + glm::vec3(radius, -radius, -radius),
		center + glm::vec3(radius, -radius, radius),
		glm::vec3(0, -1, 0)
	);
	result.addMesh(down);

	return result;
}

void Mesh::asBox(double width, double height, double depth)
{
    VBO.resize(24);
    EBO.resize(36);

    float halfW = width * 0.5f;
    float halfH = height * 0.5f;
    float halfD = depth * 0.5f;
    //front
    VBO[0].position = glm::vec4(halfW, halfH, halfD,1.0f);
    VBO[0].normal = glm::vec3(0.f, 0.f, 1.f);
    VBO[0].color = glm::vec4(255.f, 0.f, 0.f, 255.f);
    VBO[0].texCoord = glm::vec2(1.f, 1.f);
    VBO[1].position = glm::vec4(-halfW, halfH, halfD,1.0f);
    VBO[1].normal = glm::vec3(0.f, 0.f, 1.f);
    VBO[1].color = glm::vec4(0.f, 255.f, 0.f, 255.f);
    VBO[1].texCoord = glm::vec2(0.f, 1.f);
    VBO[2].position = glm::vec4(-halfW, -halfH, halfD,1.0f);
    VBO[2].normal = glm::vec3(0.f, 0.f, 1.f);
    VBO[2].color = glm::vec4(0.f, 0.f, 255.f, 255.f);
    VBO[2].texCoord = glm::vec2(0.f, 0.f);
    VBO[3].position = glm::vec4(halfW, -halfH, halfD,1.0f);
    VBO[3].normal = glm::vec3(0.f, 0.f, 1.f);
    VBO[3].color = glm::vec4(0.f, 255.f, 255.f, 255.f);
    VBO[3].texCoord = glm::vec2(1.f, 0.f);
    //left
    VBO[4].position = glm::vec4(-halfW, +halfH, halfD,1.0f);
    VBO[4].normal = glm::vec3(-1.f, 0.f, 0.f);
    VBO[4].color = glm::vec4(0.f, 0.f, 255.f, 255.f);
    VBO[4].texCoord = glm::vec2(1.f, 1.f);
    VBO[5].position = glm::vec4(-halfW, +halfH, -halfD,1.0f);
    VBO[5].normal = glm::vec3(-1.f, 0.f, 0.f);
    VBO[5].color = glm::vec4(255.f, 255.f, 0.f, 255.f);
    VBO[5].texCoord = glm::vec2(0.f, 1.f);
    VBO[6].position = glm::vec4(-halfW, -halfH, -halfD,1.0f);
    VBO[6].normal = glm::vec3(-1.f, 0.f, 0.f);
    VBO[6].color = glm::vec4(0.f, 255.f, 0.f, 255.f);
    VBO[6].texCoord = glm::vec2(0.f, 0.f);
    VBO[7].position = glm::vec4(-halfW, -halfH, halfD,1.0f);
    VBO[7].normal = glm::vec3(-1.f, 0.f, 0.f);
    VBO[7].color = glm::vec4(255.f, 255.f, 255.f, 255.f);
    VBO[7].texCoord = glm::vec2(1.f, 0.f);
    //back
    VBO[8].position = glm::vec4(-halfW, +halfH, -halfD,1.0f);
    VBO[8].normal = glm::vec3(0.f, 0.f, -1.f);
    VBO[8].color = glm::vec4(255.f, 0.f, 255.f, 255.f);
    VBO[8].texCoord = glm::vec2(0.f, 0.f);
    VBO[9].position = glm::vec4(+halfW, +halfH, -halfD,1.0f);
    VBO[9].normal = glm::vec3(0.f, 0.f, -1.f);
    VBO[9].color = glm::vec4(0.f, 255.f, 255.f, 255.f);
    VBO[9].texCoord = glm::vec2(1.f, 0.f);
    VBO[10].position = glm::vec4(+halfW, -halfH, -halfD,1.0f);
    VBO[10].normal = glm::vec3(0.f, 0.f, -1.f);
    VBO[10].color = glm::vec4(255.f, 255.f, 0.f, 255.f);
    VBO[10].texCoord = glm::vec2(1.f, 1.f);
    VBO[11].position = glm::vec4(-halfW, -halfH, -halfD,1.0f);
    VBO[11].normal = glm::vec3(0.f, 0.f, -1.f);
    VBO[11].color = glm::vec4(0.f, 0.f, 255.f, 255.f);
    VBO[11].texCoord = glm::vec2(0.f, 1.f);
    //right
    VBO[12].position = glm::vec4(halfW, +halfH, -halfD,1.0f);
    VBO[12].normal = glm::vec3(1.f, 0.f, 0.f);
    VBO[12].color = glm::vec4(0.f, 255.f, 0.f, 255.f);
    VBO[12].texCoord = glm::vec2(0.f, 0.f);
    VBO[13].position = glm::vec4(halfW, +halfH, +halfD,1.0f);
    VBO[13].normal = glm::vec3(1.f, 0.f, 0.f);
    VBO[13].color = glm::vec4(255.f, 0.f, 0.f, 255.f);
    VBO[13].texCoord = glm::vec2(1.f, 0.f);
    VBO[14].position = glm::vec4(halfW, -halfH, +halfD,1.0f);
    VBO[14].normal = glm::vec3(1.f, 0.f, 0.f);
    VBO[14].color = glm::vec4(0.f, 255.f, 255.f, 255.f);
    VBO[14].texCoord = glm::vec2(1.f, 1.f);
    VBO[15].position = glm::vec4(halfW, -halfH, -halfD,1.0f);
    VBO[15].normal = glm::vec3(1.f, 0.f, 0.f);
    VBO[15].color = glm::vec4(255.f, 0.f, 255.f, 255.f);
    VBO[15].texCoord = glm::vec2(0.f, 1.f);
    //top
    VBO[16].position = glm::vec4(+halfW, halfH, -halfD, 1.0f);
    VBO[16].normal = glm::vec3(0.f, 1.f, 0.f);
    VBO[16].color = glm::vec4(0.f, 0.f, 0.f, 255.f);
    VBO[16].texCoord = glm::vec2(0.f, 0.f);
    VBO[17].position = glm::vec4(-halfW, halfH, -halfD,1.0f);
    VBO[17].normal = glm::vec3(0.f, 1.f, 0.f);
    VBO[17].color = glm::vec4(255.f, 255.f, 0.f, 255.f);
    VBO[17].texCoord = glm::vec2(1.f, 0.f);
    VBO[18].position = glm::vec4(-halfW, halfH, halfD,1.0f);
    VBO[18].normal = glm::vec3(0.f, 1.f, 0.f);
    VBO[18].color = glm::vec4(0.f, 255.f, 255.f, 255.f);
    VBO[18].texCoord = glm::vec2(1.f, 1.f);
    VBO[19].position = glm::vec4(+halfW, halfH, halfD,1.0f);
    VBO[19].normal = glm::vec3(0.f, 1.f, 0.f);
    VBO[19].color = glm::vec4(255.f, 0.f, 0.f, 255.f);
    VBO[19].texCoord = glm::vec2(0.f, 1.f);
    //down
    VBO[20].position = glm::vec4(+halfW, -halfH, -halfD,1.0f);
    VBO[20].normal = glm::vec3(0.f, -1.f, 0.f);
    VBO[20].color = glm::vec4(0.f, 0.f, 255.f, 255.f);
    VBO[20].texCoord = glm::vec2(0.f, 0.f);
    VBO[21].position = glm::vec4(+halfW, -halfH, +halfD,1.0f);
    VBO[21].normal = glm::vec3(0.f, -1.f, 0.f);
    VBO[21].color = glm::vec4(255.f, 255.f, 255.f, 255.f);
    VBO[21].texCoord = glm::vec2(1.f, 0.f);
    VBO[22].position = glm::vec4(-halfW, -halfH, +halfD,1.0f);
    VBO[22].normal = glm::vec3(0.f, -1.f, 0.f);
    VBO[22].color = glm::vec4(0.f, 255.f, 0.f, 255.f);
    VBO[22].texCoord = glm::vec2(1.f, 1.f);
    VBO[23].position = glm::vec4(-halfW, -halfH, -halfD,1.0f);
    VBO[23].normal = glm::vec3(0.f, -1.f, 0.f);
    VBO[23].color = glm::vec4(255.f, 0.f, 255.f, 255.f);
    VBO[23].texCoord = glm::vec2(0.f, 1.f);

    //front
    EBO[0] = 0;
    EBO[1] = 1;
    EBO[2] = 2;
    EBO[3] = 0;
    EBO[4] = 2;
    EBO[5] = 3;
    //left
    EBO[6] = 4;
    EBO[7] = 5;
    EBO[8] = 6;
    EBO[9] = 4;
    EBO[10] = 6;
    EBO[11] = 7;
    //back
    EBO[12] = 8;
    EBO[13] = 9;
    EBO[14] = 10;
    EBO[15] = 8;
    EBO[16] = 10;
    EBO[17] = 11;
    //right
    EBO[18] = 12;
    EBO[19] = 13;
    EBO[20] = 14;
    EBO[21] = 12;
    EBO[22] = 14;
    EBO[23] = 15;
    //top
    EBO[24] = 16;
    EBO[25] = 17;
    EBO[26] = 18;
    EBO[27] = 16;
    EBO[28] = 18;
    EBO[29] = 19;
    //down
    EBO[30] = 20;
    EBO[31] = 21;
    EBO[32] = 22;
    EBO[33] = 20;
    EBO[34] = 22;
    EBO[35] = 23;
}

void Mesh::asFloor(double length, double height)
{
    VBO.resize(4);
    EBO.resize(6);
    VBO[0].position = glm::vec4(+length * 0.5f, height, -length * 0.5f,1.0f);
    VBO[0].normal = glm::vec3(0.f, 1.f, 0.f);
    VBO[0].color = glm::vec4(1.f, 0.f, 0.f, 1.f);
    VBO[0].texCoord = glm::vec2(0.f, 0.f);
    VBO[1].position = glm::vec4(-length * 0.5f, height, -length * 0.5f,1.0f);
    VBO[1].normal = glm::vec3(0.f, 1.f, 0.f);
    VBO[1].color = glm::vec4(0.f, 0.f, 0.f, 1.f);
    VBO[1].texCoord = glm::vec2(1.f, 0.f);
    VBO[2].position = glm::vec4(-length * 0.5f, height, +length * 0.5f,1.0f);
    VBO[2].normal = glm::vec3(0.f, 1.f, 0.f);
    VBO[2].color = glm::vec4(0.f, 0.f, 1.f, 1.f);
    VBO[2].texCoord = glm::vec2(1.f, 1.f);
    VBO[3].position = glm::vec4(+length * 0.5f, height, +length * 0.5f,1.0f);
    VBO[3].normal = glm::vec3(0.f, 1.f, 0.f);
    VBO[3].color = glm::vec4(0.f, 0.f, 0.f, 1.f);
    VBO[3].texCoord = glm::vec2(0.f, 1.f);
    EBO[0] = 0;
    EBO[1] = 1;
    EBO[2] = 2;
    EBO[3] = 0;
    EBO[4] = 2;
    EBO[5] = 3;

}

void Mesh::asTriangle(RawVertex p1, RawVertex p2, RawVertex p3)
{
    VBO.resize(3);
    EBO.resize(3);
    VBO[0].position = p1.position;
    VBO[0].normal = glm::vec3(0.f, 0.f, 1.f);
    VBO[0].color = glm::vec4(1.f, 0.f, 0.f, 1.f);
    VBO[0].texCoord = glm::vec2(0.f, 0.f);
    VBO[1].position = p2.position;
    VBO[1].normal = glm::vec3(0.f, 0.f, 1.f);
    VBO[1].color = glm::vec4(0.f, 1.f, 0.f, 1.f);
    VBO[1].texCoord = glm::vec2(1.f, 0.f);
    VBO[2].position = p3.position;
    VBO[2].normal = glm::vec3(0.f, 0.f, 1.f);
    VBO[2].color = glm::vec4(0.f, 0.f, 1.f, 1.f);
    VBO[2].texCoord = glm::vec2(0.5f, 1.f);
    EBO[0] = 0;
    EBO[1] = 1;
    EBO[2] = 2;
}