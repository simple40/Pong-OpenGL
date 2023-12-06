#include "Ball.h"

Ball::Ball(const glm::vec2& position, const glm::vec4& color, const glm::vec2& velocity, const float speed, const float radius, const int vertexCount, const std::vector<VertexAttribute>& vertexAttributes, const unsigned int& shader, GLFWwindow* window, AudioManager audioManager)
    :GameObject(position, color, velocity, nullptr, vertexCount, vertexAttributes, shader, window), radius(radius), speed(speed), audioManager(audioManager)
{
    vertices = generateCircleVertices();
    std::cout<< "b cons"<<colorLocation << std::endl<<modelLocation;
    trailPoaitions.push_back(localPosition);
    setupBuffers();
}

Ball::~Ball()
{
    delete[] vertices;
}

void Ball::draw(float deltaTime)
{
    GameObject::draw(deltaTime);
    //std::cout << position.x << " " << velocity.x << std::endl;
    move(deltaTime);
    //updateTrails();
    glBindVertexArray(VAO);
    //glUniform4f(colorLocation, color.x, color.y, color.z, color.a);

    glUniform4f(colorLocation, color.x, color.y, color.z, color.a);
    modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(localPosition.x, localPosition.y, 1.0f));
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glDrawArrays(GL_TRIANGLE_FAN, 0, vertexCount);
    //std::cout << "b d" << std::endl;
}

bool Ball::checkCollision(Paddle& other, float deltaTime, int type)
{
    glm::vec2 closestPointPosition;
    
    closestPointPosition.x = glm::clamp(position.x, other.getPosition().x - other.length / 2.0f, other.getPosition().x + other.length / 2.0f);
    closestPointPosition.y = glm::clamp(position.y, other.getPosition().y - other.width / 2.0f, other.getPosition().y + other.width / 2.0f);

    float deltaX = position.x - closestPointPosition.x;
    float deltaY = position.y - closestPointPosition.y;
    
    if ((deltaX * deltaX + deltaY * deltaY) < (radius * radius))
    {
        other.inputAllowed = false;
        //std::cout << velocity.x << " " << velocity.y << std::endl;
        setPosition(position - (velocity * deltaTime));
        collisionResolution(velocity, other, type);
        std::cout << "collision";
        audioManager.playSound("res/sounds/solid.wav");
        return true;
    }
    other.inputAllowed = true;
    return false;
}

float* Ball::generateCircleVertices()
{
    float* circleVertices = new float[vertexCount * 2];
    for (int i = 0; i < vertexCount; i++) {
        float angle = 2.0f * M_PI * i / vertexCount;
        circleVertices[i * 2] = radius * cosf(angle) + position.x;
        circleVertices[i * 2 + 1] = radius * sinf(angle) + position.y;
    }
    std::cout << "b gen" << std::endl;
    /*for (int i = 0; i < vertexCount; i++) {
        std::cout << circleVertices[i * 2] << " " << circleVertices[i * 2 + 1] << std::endl;
    }*/
    return circleVertices;
}

void Ball::collisionResolution(glm::vec2 target, Paddle& other, int type)
{
    glm::vec2 compass[] = {
        glm::vec2(0.0f, 1.0f),	// up
        glm::vec2(1.0f, 0.0f),	// right
        glm::vec2(0.0f, -1.0f),	// down
        glm::vec2(-1.0f, 0.0f)	// left
    };

    float max = 0.0f;
    unsigned int bestMatch = -1;
    for (unsigned int i = 0; i < 4; i++) {
        float dotProduct = glm::dot(glm::normalize(target), compass[i]);
        if (dotProduct > max) {
            max = dotProduct;
            bestMatch = i;
        }
    }

    //if (bestMatch == 1 || bestMatch == 3) {
    //    float diffX = position.x - other.getPosition().x;
    //    float diffY = position.y - other.getPosition().y;
    //    velocity.x = -1.0f * (velocity.x);
    //    //std::cout << velocity.x;
    //}
    //else {
    //    velocity.y = -1.0f * (velocity.y);
    //}
    //glm::vec2 collisionNormal = glm::normalize(other.getPosition() - position);
    std::cout << velocity.x << " " << velocity.y << std::endl;
    glm::vec2 reflection = glm::reflect(velocity, other.getPosition());
    double dot_product = velocity.x * other.getPosition().x + velocity.y * other.getPosition().y;
    double magnitude1 = sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
    double magnitude2 = sqrt(other.getPosition().x * other.getPosition().x + other.getPosition().y * other.getPosition().y);
    double angle = acos(dot_product / (magnitude1 * magnitude2));
    std::cout <<"angle "<< angle * 180 / M_PI << std::endl;

    //glm::vec2 reflection = glm::reflect(velocity, collisionNormal);
    std::cout << "after reflection" << std::endl;
    if (reflection == velocity) 
    {
        velocity = glm::normalize(velocity) * -1.0f * speed;
        return;
    }
    speed += 0.1f;
    std::cout << reflection.x << " " << reflection.y << std::endl;
    velocity = glm::normalize(reflection) * speed;
    std::cout << velocity.x << " " << velocity.y << std::endl;
    //velocity = glm::vec2(0.0f, 0.0f);
}

void Ball::collisionResolution(glm::vec2 target, Wall& other)
{
}

void Ball::updateTrails()
{
    trailPoaitions.push_back(localPosition);

    const int maxTrailSize = 50;
    while (trailPoaitions.size() > maxTrailSize) {
        trailPoaitions.erase(trailPoaitions.begin());
    }
}

void Ball::setSpeed(float newSpeed)
{
    speed = newSpeed;
}
