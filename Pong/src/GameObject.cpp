#include "GameObject.h"


GameObject::GameObject(const glm::vec2& position, const glm::vec4& color, const glm::vec2& velocity, const float* vertices, const int vertexCount, const std::vector<VertexAttribute>& vertexAttributes, const unsigned int& shader, GLFWwindow* window )
	:position(position), color(color),velocity(velocity), vertices(vertices), vertexCount(vertexCount), vertexAttributes(vertexAttributes),shader(shader), window(window)
{
	glGenVertexArrays(1, &VAO);
	colorLocation = glGetUniformLocation(shader, "uniformColor");
	modelLocation = glGetUniformLocation(shader, "modelMatrix");
	localPosition = glm::vec2(0.0f, 0.0f);
	//setupBuffers();
}

GameObject::~GameObject()
{
	glDeleteVertexArrays(1, &VAO);
}

void GameObject::draw(float deltaTime)
{
	glBindVertexArray(VAO);
	glUseProgram(shader);
}

void GameObject::move(float deltaTime)
{
	position += velocity * deltaTime;
	localPosition += velocity * deltaTime;
}

glm::vec2 GameObject::getPosition()
{
	return position;
}

void GameObject::setPosition(const glm::vec2& newPosition)
{
	glm::vec2 disp =   newPosition - position;
	position = newPosition;
	localPosition += disp;
}

glm::vec2 GameObject::getVelocity()
{
	return velocity;
}

void GameObject::setVelocity(const glm::vec2 newVelocity)
{
	velocity = newVelocity;
}



void GameObject::setupBuffers()
{
	//std::cout << "setup";
	glBindVertexArray(VAO);

	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(float) * vertexAttributes[0].size, vertices, GL_STATIC_DRAW);

	for (const auto& attribute : vertexAttributes) {
		glEnableVertexAttribArray(attribute.index);
		glVertexAttribPointer(attribute.index, attribute.size, attribute.type, attribute.normalized, attribute.stride, attribute.offset);
	}
	glBindVertexArray(0); // Unbind VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind VBO
}
