#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ft2build.h>
#include <freetype/freetype.h>
#include FT_FREETYPE_H  
#include <irrKlang.h>

#include "Ball.h"
#include "Paddle.h"
#include "Wall.h"
#include "TextRenderer.h"
#include "AudioManager.h"



using namespace std;

enum class GameState {
    START,
    LOSE,
    PLAY
};


struct ShaderProgramSource
{
    string VertexSource;
    string FragmentSource;
};

static ShaderProgramSource parseShader(const string& filePath) {

    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    ifstream stream(filePath);
    string line;
    stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line)) {
        if (line.find("#shader") != string::npos) {
            if (line.find("vertex") != string::npos) {
                type = ShaderType::VERTEX;
            }
            else if(line.find("fragment") != string::npos) {
                type = ShaderType::FRAGMENT;
            }
        }
        else {
            ss[(int)type] << line << "\n";
        }
        
    }
    return { ss[0].str(),ss[1].str() };
}

static int compileShader(unsigned int  type, const string& source) {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    bool r = glIsShader(id);
    if (r == GL_FALSE)
        cout << "shader = " << r << endl;
    glShaderSource(id, 1, &src, NULL);
    glCompileShader(id);
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE){
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        cout << "failed to compile " << (type == GL_VERTEX_SHADER ? "vertex " : "fragment") << "shader!" << endl;
        cout << message << endl;
        glDeleteShader(id);
        return 0;
    }
    return id;
}

static unsigned int createShader(const string& vertexShader, const string& fragmentShader) {
    unsigned int program = glCreateProgram();
    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    char msg[512];
    glGetProgramInfoLog(program, 512, NULL, msg);
    cout << msg << endl;
    int res;
    glGetProgramiv(program, GL_LINK_STATUS, &res);
    if (res == GL_FALSE)
        cout << "res = " << res << endl;
    glDeleteShader(vs);
    glDeleteShader(fs);
    return program;
}

// Global variables
float aspectRatio = 1.0f;
float zoom = 1.0f;

// Update Viewport and Projection Matrix
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    aspectRatio = static_cast<float>(width) / static_cast<float>(height);

    // Update projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-zoom * aspectRatio, zoom * aspectRatio, -zoom, zoom, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Pong", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    //glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); //for fullsize of window 

    if (glewInit() != GLEW_OK)
        cout << "error";
    cout << glGetString(GL_VERSION); 

    irrklang::ISoundEngine* SoundEngine = irrklang::createIrrKlangDevice();
    //SoundEngine->play2D("res/sounds/getout.ogg", true);

    AudioManager audioManager;

    

    float paddle1Vertices[] = {
        -0.9f, 0.2f,  //left paddle
        -0.9f,-0.2f,
        -0.85f,-0.2f,
        -0.85f, 0.2f,
    };
    float paddle2Vertices[] = {
         0.9f, 0.2f,  //right paddle
         0.9f,-0.2f,
         0.85f,-0.2f,
         0.85f, 0.2f,
    };

    float wall1Vertices[] = {
        -1.0f, 1.0f,    //upper wall
        -1.0f, 0.98f,
         1.0f, 0.98f,
         1.0f, 1.0f,
    };
    float wall2Vertices[] = {
         -1.0f, -1.0f,   //lower wall
        -1.0f, -0.98f,
         1.0f, -0.98f,
         1.0f, -1.0f
    };

    ShaderProgramSource source = parseShader("res/shaders/Basic.shader");
    unsigned int shader = createShader(source.VertexSource, source.FragmentSource);
    glUseProgram(shader);
    ShaderProgramSource source2 = parseShader("res/shaders/TextRenderer.shader");
    unsigned int shader2 = createShader(source2.VertexSource, source2.FragmentSource);
    glUseProgram(shader2);

    int location = glGetUniformLocation(shader, "uniformColor");
    int modelLocation = glGetUniformLocation(shader, "modelMatrix");

    float paddle1y = 0.0f;
    float paddle2y = 0.0f;
    float paddleSpeed = 0.001f;

    std::vector<VertexAttribute> ballVertexAttributes;
    ballVertexAttributes.push_back({ 0, 2, GL_FLOAT, false, sizeof(float) * 2, 0 });

    glm::vec2 ballPosition(0.0f, 0.0f);
    glm::vec2 ballVelocity(0.0f, 0.0f);
    glm::vec4 color(0.0f, 0.0f, 1.0f, 1.0f);
    glm::mat4 ballModelMatrix;
    float ballSpeed = 1.5f;
    const int vertexNo = 20;
    float circleVertices[vertexNo * 2];
    const float radius = 0.03f;

    Ball ball(ballPosition,color,ballVelocity,ballSpeed,radius,vertexNo,ballVertexAttributes,shader,window, audioManager);

    std::vector<VertexAttribute> paddleVertexAttributes;
    paddleVertexAttributes.push_back({ 0, 2, GL_FLOAT, false, sizeof(float) * 2, 0 });

    glm::vec2 paddlePosition1(-0.875f, 0.0f);
    glm::vec2 paddlePosition2( 0.875f, 0.0f);
    glm::vec2 paddleVelocity(0.0f);
    glm::vec4 paddleColor(0.0f, 1.0f, 1.0f, 1.0f);
    float paddlesLength = 0.05f;
    float paddlesWidth = 0.4f;


    Paddle paddle1(paddlePosition1, paddleColor, paddleVelocity, paddle1Vertices, 4, paddleVertexAttributes, shader, window, paddlesLength, paddlesWidth);
    Paddle paddle2(paddlePosition2, paddleColor, paddleVelocity, paddle2Vertices, 4, paddleVertexAttributes, shader, window, paddlesLength, paddlesWidth);

    glm::vec2 wallPosition1(0.0f,  0.99f);
    glm::vec2 wallPosition2(0.0f, -0.99f);
    glm::vec2 wallVelocity(0.0f);
    glm::vec4 wallColor(0.0f, 1.0f, 1.0f, 1.0f);
    float wallLength = 2.0f;
    float wallWidth = 0.1f;

    Paddle wall1(wallPosition1, wallColor, wallVelocity, wall1Vertices, 4, paddleVertexAttributes, shader, window, wallLength, wallWidth);
    Paddle wall2(wallPosition2, wallColor, wallVelocity, wall1Vertices, 4, paddleVertexAttributes, shader, window, wallLength, wallWidth);

    TextRenderer textRenderer("D:/OpenGL/Pong/Pong/res/Fonts/arial.ttf",48,640, 480, shader2);

    float deltaTime = 0.0f;
    float lastFrameTime = 0.0f;
    glUseProgram(shader);
    /* Loop until the user closes the window */

    GameState gameState(GameState::START);

    while (!glfwWindowShouldClose(window))
    {

        float currentFrameTime = glfwGetTime();
        deltaTime = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        //textRenderer.rendertext("hello", 0.0f, 0.0f, 1, glm::vec3(1.0f, 1.0f, 1.0f));

        ball.draw(deltaTime);

        paddle1.draw(deltaTime);;
        paddle2.draw(deltaTime);

        wall1.draw(deltaTime);
        wall2.draw(deltaTime);

        if (gameState == GameState::START) {
            // Render start screen elements
            // For example, display "Press Spacebar to Start" text
            if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
                ball.setVelocity(glm::vec2(1.0f, 0.0f));
                gameState = GameState::PLAY;
                ball.setSpeed(ballSpeed);
            }
        }
        else if (gameState == GameState::PLAY) {
            // Render gameplay elements
            // This includes rendering players, obstacles, etc.
            paddle1.handleInput(GLFW_KEY_W, GLFW_KEY_S);
            paddle2.handleInput(GLFW_KEY_UP, GLFW_KEY_DOWN);

            ball.checkCollision(paddle1, deltaTime, 1);
            ball.checkCollision(paddle2, deltaTime, 1);
            ball.checkCollision(wall1, deltaTime, 2);
            ball.checkCollision(wall2, deltaTime, 2);

        }
        else if (gameState == GameState::LOSE) {
            // Render lose screen elements
            // For example, display "Game Over. Press Spacebar to Restart" text
            ball.setPosition(glm::vec2(0.0f, 0.0f));
            ball.setVelocity(glm::vec2(0.0f, 0.0f));
            paddle1.setPosition(glm::vec2(-0.875f, 0.0f));
            paddle2.setPosition(glm::vec2(0.875f, 0.0f));
            gameState = GameState::START;
        }

        if (ball.getPosition().x > 1.0f || ball.getPosition().x < -1.0f) {
            gameState = GameState::LOSE;
        }
        

        glBindVertexArray(0);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
//do the shader2 and I think that the uniforms in the text renderer are not been set in the draw so check the text reder fuction which is rendering the text