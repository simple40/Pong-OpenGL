#include "Wall.h"

Wall::Wall(const glm::vec2& position, const glm::vec4& color, const glm::vec2& velocity, const float* Pvertices, const int vertexCount, const std::vector<VertexAttribute>& vertexAttributes, const unsigned int& shader, GLFWwindow* window, const float& length, const float& width)
    :GameObject(position, color, velocity, nullptr, vertexCount, vertexAttributes, shader, window), length(length), width(width)
{
    vertices = genetrateVertices();
    setupBuffers();
    std::cout << "Wall" << std::endl;
}

Wall::~Wall()
{
    delete[] vertices;
}

void Wall::draw(float deltaTime)
{
    glBindVertexArray(VAO);
    modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, localPosition.y, 1.0f));
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glUniform4f(colorLocation, color.x, color.y, color.z, color.a);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

float* Wall::genetrateVertices()
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



