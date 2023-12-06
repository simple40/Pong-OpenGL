
#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include "GameObject.h"

class Wall :public GameObject
{
public:
	Wall(const glm::vec2& position, const glm::vec4& color, const glm::vec2& velocity, const float* vertices, const int vertexCount, const std::vector<VertexAttribute>& vertexAttributes, const unsigned int& shader, GLFWwindow* window, const float& length, const float& width);
	~Wall();

	float length, width;

	void draw(float deltaTime) override;


private:

	float* genetrateVertices();

};