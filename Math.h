﻿#pragma once
#include "Mesh.h"
#include "glm.hpp"

const double tolerance = 1e-5f;

glm::vec4 Lerp(const glm::vec4& v1, const glm::vec4& v2, const double& factor);
glm::vec4 Lerp(const glm::vec4& v1, const glm::vec4& v2, const glm::vec4& v3, const glm::vec3& factor);
glm::vec3 Lerp(const glm::vec3& v1, const glm::vec3& v2, const double& factor);
glm::vec3 Lerp(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3, const glm::vec3& factor);
glm::vec2 Lerp(const glm::vec2& v1, const glm::vec2& v2, const double& factor);
glm::vec2 Lerp(const glm::vec2& v1, const glm::vec2& v2, const glm::vec2& v3, const glm::vec3& factor);

double Lerp(const double& f1, const double& f2, const double& factor);
double Lerp(const double& f1, const double& f2, const double& f3, const glm::vec3& factor);

Vertex Lerp(const Vertex& v1, const Vertex& v2, const double& factor);
Vertex Lerp(const Vertex& v1, const Vertex& v2, const Vertex& v3, const glm::vec3& factor);

//除透视法
void perspectiveDivision(Vertex& v);

bool viewFrustumCutting(const Vertex& A, const Vertex& B, const Vertex& C, std::vector<glm::vec4>& planes);

//面剔除
bool backFaceCutting(const Vertex& A, const Vertex& B, const Vertex& C);

glm::vec3 getBarycentric(const Vertex& A, const Vertex& B, const Vertex& C, const glm::vec2& P);
glm::mat4 getViewMatrix(const glm::vec3& pos, const glm::vec3 front, const glm::vec3 right, const glm::vec3 up);
glm::mat4 getPerspectiveMatrix(const double& fovy, const double& aspect, const double& n, const double& f);
glm::mat4 getViewPortMatrix(int x, int y, int width, int height);

template<class T>
T triMin(T a, T b, T c);

template<class T>
T triMax(T a, T b, T c);

double distance(const glm::vec3& point, const glm::vec4& plane);		//PointToFace distance
bool equal(double a, double b);

