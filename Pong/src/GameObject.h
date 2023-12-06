#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <GLFW/glfw3.h>

struct VertexAttribute {
	GLuint index;
	GLint size;
	GLenum type;
	GLboolean normalized;
	GLsizei stride;
	const void* offset;
	

};

class GameObject
{
public: 
	GameObject(const glm::vec2& position, const glm::vec4& color,const glm::vec2& velocity, const float* vertices,const int vertexCount, const std::vector<VertexAttribute>& vertexAttributes, const unsigned int& shader, GLFWwindow* window);
	virtual ~GameObject();

	virtual void draw(float deltaTime);
	virtual void move(float deltaTime);

	virtual glm::vec2 getPosition();
	void setPosition(const glm::vec2& newPosition);
	glm::vec2 getVelocity();
	void setVelocity(const glm::vec2 newVelocity);
protected:
	glm::vec2 position;
	glm::vec4 color;
	GLuint VAO;
	const float* vertices;
	int vertexCount;
	std::vector<VertexAttribute> vertexAttributes;
	glm::mat4 modelMatrix;
	glm::vec2 velocity;
	int colorLocation;
	int modelLocation;
	GLFWwindow* window;
	unsigned int shader;
	glm::vec2 localPosition;

	virtual void setupBuffers();

};