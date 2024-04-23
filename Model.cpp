#include "Model.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include "Math.h"
Model::Model(const std::string& path)
{
	loadObjFile(path);
}

Model::~Model()
{

}

glm::mat4 Model::setSize(float sx, float sy, float sz) const
{
	float length = fabs(maxPoint.x - minPoint.x);
	float scaleFactor = 1.0f / length;
	glm::mat4 result = glm::mat4(1.0f);
	result[0][0] = sx * scaleFactor;
	result[1][1] = sy * scaleFactor;
	result[2][2] = sz * scaleFactor;
	return result;
}


void Model::loadObjFile(const std::string& path)
{
	// obj loader.
	std::ifstream in;
	in.open(path, std::ifstream::in);
	if (in.fail())
	{
		std::cout << "Fail to load obj->" << path << std::endl;
		return;
	}

	VBO.clear();
	EBO.clear();

	std::string line;
	minPoint = glm::vec3(+10000000000, +10000000000, +10000000000);
	maxPoint = glm::vec3(-10000000000, -10000000000, -10000000000);
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> texcoords;

	while (!in.eof()) {
		std::getline(in, line);
		std::istringstream iss(line.c_str());
		char trash;
		if (!line.compare(0, 2, "v ")) {
			iss >> trash;
			glm::vec3 v;
			for (int i = 0; i < 3; i++) iss >> v[i];
			RawVertex vertex(v);
			VBO.push_back(v);
			if (minPoint.x > v.x)minPoint.x = v.x;
			if (minPoint.y > v.y)minPoint.y = v.y;
			if (minPoint.z > v.z)minPoint.z = v.z;
			if (maxPoint.x < v.x)maxPoint.x = v.x;
			if (maxPoint.y < v.y)maxPoint.y = v.y;
			if (maxPoint.z < v.z)maxPoint.z = v.z;

		}
		else if (!line.compare(0, 3, "vt ")) {
			iss >> trash >> trash;
			glm::vec2 uv;
			iss >> uv.x >> uv.y;
			uv.y = 1 - uv.y;
			texcoords.push_back(uv);
		}
		else if (!line.compare(0, 3, "vn ")) {
			iss >> trash >> trash;
			glm::vec3 vn;
			for (int i = 0; i < 3; i++) iss >> vn[i];
			normals.push_back(vn);
		}
		else if (!line.compare(0, 2, "f ")) {
			int vIndex, vtIndex, vnIndex;
			char bar;
			iss >> trash;
			for (int i = 0; i < 3; i++) {
				iss >> vIndex >> bar >> vtIndex >> bar >> vnIndex;
				if (vIndex > VBO.size() || vtIndex > texcoords.size() || vnIndex > normals.size()) {
					std::cout << path << "超过范围!" << std::endl;
					return;
				}
				VBO[vIndex - 1].texCoord = texcoords[vtIndex - 1];
				VBO[vIndex - 1].normal = normals[vIndex - 1];
				EBO.push_back(vIndex - 1);
			}
		}
	}
    in.close();
}