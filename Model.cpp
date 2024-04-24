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

glm::mat4 Model::setSize(double sx, double sy, double sz) const
{
	double length = fabs(maxPoint.x - minPoint.x);
	double scaleFactor = 1.0f / length;
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
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> texcoords;

	while (!in.eof()) {
		std::getline(in, line);
		std::istringstream iss(line.c_str());
		char trash;
		if (!line.compare(0, 2, "v ")) {
			iss >> trash;
			glm::vec3 v;
			iss >> v.x;
			iss >> v.y;
			iss >> v.z;
			vertices.push_back(v);
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
			iss >> uv.x;
			iss >> uv.y;
			texcoords.push_back(uv);
		}
		else if (!line.compare(0, 3, "vn ")) {
			iss >> trash >> trash;
			glm::vec3 normal;
			iss >> normal.x;
			iss >> normal.y;
			iss >> normal.z;
			normal = glm::normalize(normal);
			normals.push_back(normal);
		}
		else if (!line.compare(0, 2, "f ")) {
			iss >> trash;
			int index[3];
			while (iss >> index[0] >> trash >> index[1] >> trash >> index[2])
			{
				RawVertex data;
				data.position = glm::vec4(vertices[index[0] - 1],1.0f);
				data.texCoord = texcoords[index[1] - 1];
				data.normal = normals[index[2] - 1];
				data.color = glm::vec4(255.0f, 255.0f, 255.0f, 255.0f);
				EBO.push_back(VBO.size());
				VBO.push_back(data);
			}
		}
	}
    in.close();
}