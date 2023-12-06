#pragma once
#include <ft2build.h>
#include <freetype/freetype.h>
#include FT_FREETYPE_H  
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <map>
#include <filesystem>
#include <fstream>


struct Character 
{
	unsigned int TextureID;
	glm::ivec2 size;
	glm::ivec2 bearing;
	GLuint advance;

};

class TextRenderer {

public:
	TextRenderer(const std::string& fontPath, int fontSize, unsigned int screenWidth, unsigned int screenHeight, GLuint shaderProgram);
	~TextRenderer();
	void rendertext(const std::string& text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);
private:
	FT_Library ft;
	FT_Face face;
	const std::string& fontPath; int fontSize; unsigned int screenWidth; unsigned int screenHeight;
	void initialize();
	std::map<char, Character> characters;
	void loadCharacters();
	unsigned int VAO, VBO;
	GLuint shader;
};
