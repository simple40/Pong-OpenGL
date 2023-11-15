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


using namespace std;

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

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); //for fullsize of window 

    if (glewInit() != GLEW_OK)
        cout << "error";
    cout << glGetString(GL_VERSION); 

    float positions[6] = {
        0.0f, 0.5f,
       -0.5f, -0.5f,
       0.5f, -0.5f
    };

    const int vertexNo = 20;
    float circleVertices[vertexNo * 2];
    const float radius = 0.03f;

    for (int i = 0; i < vertexNo; i++) {
        float angle = 2.0f * M_PI * i / vertexNo;
        circleVertices[i * 2] = radius * cosf(angle);
        circleVertices[i * 2 + 1] = radius * sinf(angle);
    }

    float paddleVertices[] = {
        -0.9f, 0.2f,  //left paddle
        -0.9f,-0.2f,
        -0.85f,-0.2f,
        -0.85f, 0.2f,

         0.9f, 0.2f,  //right paddle
         0.9f,-0.2f,
         0.85f,-0.2f,
         0.85f, 0.2f,
    };

    float walls[] = {
        -1.0f, 1.0f,    //upper wall
        -1.0f, 0.98f,
         1.0f, 0.98f,
         1.0f, 1.0f,

        -1.0f, -1.0f,   //lower wall
        -1.0f, -0.98f,
         1.0f, -0.98f,
         1.0f, -1.0f
    };

    unsigned int ballVAO, paddleVAO;
    glGenVertexArrays(1, &ballVAO);
    glBindVertexArray(ballVAO);

    unsigned int ballVBO;
    glGenBuffers(1, &ballVBO);
    glBindBuffer(GL_ARRAY_BUFFER, ballVBO);
    glBufferData(GL_ARRAY_BUFFER, vertexNo * 2 * sizeof(float), circleVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, false, sizeof(float) * 2, 0);


    glGenVertexArrays(1, &paddleVAO);
    glBindVertexArray(paddleVAO);

    unsigned int paddleVBO;
    glGenBuffers(1, &paddleVBO);
    glBindBuffer(GL_ARRAY_BUFFER, paddleVBO);
    glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(float), paddleVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, false, 2 * sizeof(float), 0);

    
    unsigned int wallsVAO;
    glGenVertexArrays(1, &wallsVAO);
    glBindVertexArray(wallsVAO);

    unsigned int wallsVBO;
    glGenBuffers(1, &wallsVBO);
    glBindBuffer(GL_ARRAY_BUFFER, wallsVBO);
    glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(float), walls, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, false, 2 * sizeof(float), 0);

    ShaderProgramSource source = parseShader("res/shaders/Basic.shader");
    unsigned int shader = createShader(source.VertexSource, source.FragmentSource);
    glUseProgram(shader);
    int location = glGetUniformLocation(shader, "uniformColor");
    int modelLocation = glGetUniformLocation(shader, "modelMatrix");

    float paddle1y = 0.0f;
    float paddle2y = 0.0f;
    float paddleSpeed = 0.001f;

    glm::vec2 ballPosition(0.0f);
    glm::vec2 ballVelocity(0.1f, 0.0f);

    float deltaTime = 0.0f;
    float lastFrameTime = 0.0f;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {

        float currentFrameTime = glfwGetTime();
        deltaTime = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        ballPosition += ballVelocity * deltaTime;
        glBindVertexArray(ballVAO);
        glUniform4f(location, 1.0f, 1.0f, 0.0f, 1.0f);
        glm::mat4 modelB = glm::translate(glm::mat4(1.0f), glm::vec3(ballPosition.x, ballPosition.y, 1.0f));
        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(modelB));
        glDrawArrays(GL_TRIANGLE_FAN, 0, vertexNo);
        

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) 
            paddle1y += paddleSpeed;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            paddle1y -= paddleSpeed;

        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
            paddle2y += paddleSpeed;
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
            paddle2y -= paddleSpeed;

        paddle1y = glm::clamp(paddle1y, -1.0f + 0.22f , 1.0f - 0.22f );
        paddle2y = glm::clamp(paddle2y, -1.0f + 0.22f , 1.0f - 0.22f );

        glBindVertexArray(paddleVAO);

        //paddle 1
        glm::mat4 model1 = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f,paddle1y,1.0f));
        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model1));
        glUniform4f(location, 1.0f, 0.0f, 0.0f, 1.0f);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

        //paddle 2
        glm::mat4 model2 = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, paddle2y, 1.0f));
        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model2));
        //glUniform4f(location, 1.0f, 1.0f, 1.0f, 1.0f);
        glDrawArrays(GL_TRIANGLE_FAN, 4, 4);

        //upper wall
        glBindVertexArray(wallsVAO);
        glUniform4f(location, 0.0f, 1.0f, 1.0f, 1.0f);
        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        //lower wall
        glBindVertexArray(wallsVAO);
        glUniform4f(location, 0.0f, 1.0f, 1.0f, 1.0f);
        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));
        glDrawArrays(GL_TRIANGLE_FAN, 4, 4);

        glm::mat4 invModelMatrixPaddle1 = glm::inverse(model1);
        glm::mat4 invModelMatrixPaddle2 = glm::inverse(model2);

        glm::vec4 ballPosPaddle1Space = invModelMatrixPaddle1 * glm::vec4(ballPosition, 1.0f, 1.0f);
        glm::vec4 ballPosPaddle2Space = invModelMatrixPaddle2 * glm::vec4(ballPosition, 1.0f, 1.0f);
        //cout << ballPosPaddle1Space.x << endl;
        //cout << ballPosPaddle2Space.x << endl;
        
        if (ballPosPaddle1Space.x > -1.0f && ballPosPaddle1Space.x < 1.0f && ballPosPaddle1Space.y > -1.0f && ballPosPaddle1Space.y < 1.0f) {
            cout << "padde 1" << endl;
        }
        if (ballPosPaddle2Space.x > -1.0f && ballPosPaddle2Space.x < 1.0f && ballPosPaddle2Space.y > -1.0f && ballPosPaddle2Space.y < 1.0f) {
            cout << "padde 2" << endl;
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