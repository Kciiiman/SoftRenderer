#pragma once
#include <string>
#include "glm.hpp"

class Camera
{
public:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;
	double fov;
	double aspect;

	double pitch; 
	double yaw; 

	double n; 
	double f;

	Camera(
		const glm::vec3& pos = glm::vec3(0.0f, 0.0f, -1.0f),
		const glm::vec3& worldup = glm::vec3(0.0f, 1.0f, 0.0f),
		const glm::vec3& lookAt = glm::vec3(0.0f, 0.0f, 0.0f),
		const double& _fov = 60.0,
		const double& _n = 0.3f,
		const double& _f = 100.0f
	);

	void updateAspact(const int& width, const int& height);
	void updateFov(const double& f);
	glm::mat4 viewMatrix();
	glm::mat4 perspectiveMatrix();

	void moveForward(const double& distance);
	void moveBackward(const double& distance);
	void moveRight(const double& distance);
	void moveLeft(const double& distance);
	void moveUp(const double& distance);
	void moveDown(const double& distance);

	void rotatePitch(const double& angle); 
	void rotateYaw(const double& angle); 

	void updateVectors();
};

