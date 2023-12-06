#include "Paddle.h"

Paddle::Paddle(const glm::vec2& position, const glm::vec4& color, const glm::vec2& velocity, const float* Pvertices, const int vertexCount, const std::vector<VertexAttribute>& vertexAttributes, const unsigned int& shader, GLFWwindow* window, const float& length, const float& width)
	:GameObject(position, color, velocity, nullptr, vertexCount, vertexAttributes, shader, window), length(length), width(width)
{
    paddleSpeed = 0.001f;
    vertices = genetrateVertices();
    setupBuffers();
    std::cout << "Paddle" << std::endl;
}

Paddle::~Paddle()
{
    delete[] vertices;
}

void Paddle::draw(float deltaTime)
{
    GameObject::draw(deltaTime);
    glBindVertexArray(VAO);
    modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, localPosition.y, 1.0f));
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glUniform4f(colorLocation, color.x,color.y,color.z,color.a);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void Paddle::handleInput(int upKey, int downKey)
{
    if (inputAllowed) {
        if (glfwGetKey(window, upKey) == GLFW_PRESS) {
            localPosition.y += paddleSpeed;
            position.y += paddleSpeed;
        }

        if (glfwGetKey(window, downKey) == GLFW_PRESS) {
            localPosition.y -= paddleSpeed;
            position.y -= paddleSpeed;
        }
    }

    position.y = glm::clamp(position.y, -1.0f + 0.22f, 1.0f - 0.22f);
    localPosition.y = glm::clamp(localPosition.y, -1.0f + 0.22f, 1.0f - 0.22f);
}

float* Paddle::genetrateVertices()
{
    float* vertices = new float[8];
    vertices[0] = position.x - (length / 2.0f); vertices[1] = position.y + (width / 2.0f);
    vertices[2] = position.x - (length / 2.0f); vertices[3] = position.y - (width / 2.0f);
    vertices[4] = position.x + (length / 2.0f); vertices[5] = position.y - (width / 2.0f);
    vertices[6] = position.x + (length / 2.0f); vertices[7] = position.y + (width / 2.0f);
    /*std::cout << "here";
    for (int i = 0; i < 8; i++) {
        std::cout << vertices[i] << std::endl;
    }*/
    return vertices;
}

   

