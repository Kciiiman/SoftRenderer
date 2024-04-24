#include "Camera.h"
#include "Math.h"

Camera::Camera(
	const glm::vec3& pos,
	const glm::vec3& worldup,
	const glm::vec3& lookAt,
	const double& _fov,
	const double& _n,
	const double& _f
) : position(pos), worldUp(worldup), n(_n), f(_f) {
	front = glm::normalize(lookAt - pos);
	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
	fov = glm::radians(_fov);

	glm::vec3 worldFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 frontXZ = glm::normalize(glm::vec3(front.x, 0.0f, front.z));
	double cosPitch = glm::dot(front, frontXZ);
	double cosYaw = glm::dot(worldFront, frontXZ);
	cosPitch = cosPitch > 1 ? 1 : cosPitch;
	cosPitch = cosPitch < 0 ? 0 : cosPitch;
	cosYaw = cosYaw > 1 ? 1 : cosYaw;
	cosYaw = cosYaw < -1 ? -1 : cosYaw;
	pitch = glm::degrees(acos(cosPitch));
	pitch = front.y > 0 ? pitch : -pitch;
	yaw = glm::degrees(acos(cosYaw));
	yaw = front.x > 0 ? yaw : 360 - yaw;
}

void Camera::updateAspact(const int& width, const int& height) {
	double w = width;
	double h = height;
	aspect = w / h;
}

void Camera::updateFov(const double& f) {
	fov = glm::radians(f);
}

glm::mat4 Camera::viewMatrix() {
	return getViewMatrix(position, front, right, up);
}

glm::mat4 Camera::perspectiveMatrix() {
	return getPerspectiveMatrix(fov, aspect, n, f);
}

void Camera::moveForward(const double& distance) {
	position += front * static_cast<float>(distance);
}

void Camera::moveBackward(const double& distance) {
	position -= front * static_cast<float>(distance);
}

void Camera::moveRight(const double& distance) {
	position += right * static_cast<float>(distance);
}

void Camera::moveLeft(const double& distance) {
	position -= right * static_cast<float>(distance);
}

void Camera::moveUp(const double& distance) {
	position += up * static_cast<float>(distance);
}

void Camera::moveDown(const double& distance) {
	position -= up * static_cast<float>(distance);
}

void Camera::rotatePitch(const double& angle) {
	pitch += angle;
	pitch = pitch > 89.0 ? 89.0 : pitch;
	pitch = pitch < -89.0 ? -89.0 : pitch;
	updateVectors();
}

void Camera::rotateYaw(const double& angle) {
	yaw += angle;
	while (yaw >= 360) yaw -= 360;
	while (yaw < 0) yaw += 360;
	updateVectors();
}

void Camera::updateVectors() {
	glm::vec3 _front;

	front.x = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = -cos(glm::radians(yaw)) * cos(glm::radians(pitch));

	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}