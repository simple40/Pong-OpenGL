#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>\
#include <ft2build.h>
#include <freetype/freetype.h>
#include FT_FREETYPE_H  

#include "Ball.h"
#include "Paddle.h"
#include "Wall.h"
#include "TextRenderer.h"

enum class GameState {
	START,
	LOSE,
	PLAY
};

class Game {
public:
	Game();
	~Game();

	void run();

private:
	void initialize();
	void handleInput();
	void update();
	void render();
	void restart();

	GLFWwindow* window;
	GameState gameState;
};