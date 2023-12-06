#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include "GameObject.h"
#include "Paddle.h"
#include "Wall.h"
#include "AudioManager.h"

class Ball : public GameObject
{
public:
	Ball(const glm::vec2& position, const glm::vec4& color, const glm::vec2& velocity, const float speed, const float radius, const int vertexCount, const std::vector<VertexAttribute>& vertexAttributes, const unsigned int& shader, GLFWwindow* window, AudioManager audioManager);
	~Ball();

	void draw(float deltaTime) override;
	bool checkCollision(Paddle& other , float deltaTime, int type);
	void setSpeed(float newSpeed);

private:
	float radius, speed;
	std::vector<glm::vec2> trailPoaitions;
	float* generateCircleVertices();
	void collisionResolution(glm::vec2 target, Paddle& other, int type);
	void collisionResolution(glm::vec2 target, Wall& other);
	void updateTrails();
	AudioManager audioManager;
};
