#include "TextRenderer.h"

TextRenderer::TextRenderer(const std::string& fontPath, int fontSize, unsigned int screenWidth, unsigned int screenHeight, GLuint shaderProgram)
	:fontPath(fontPath), fontSize(fontSize), screenWidth(screenWidth), screenHeight(screenHeight), shader(shaderProgram)
{
	initialize();
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

TextRenderer::~TextRenderer()
{

}



void TextRenderer::initialize()
{
	if (FT_Init_FreeType(&ft)) {
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
		return;
	}
	//std::string font_name = std::filesystem::  //update c++
	/*if (font_name.empty())
	{
		std::cout << "ERROR::FREETYPE: Failed to load font_name" << std::endl;
		return;
	}*/
	if (FT_New_Face(ft, fontPath.c_str(), 0, &face)) {
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
		FT_Done_FreeType(ft);
		return;
	}
	FT_Set_Pixel_Sizes(face, 0, fontSize);
	loadCharacters();
	std::cout << "initialize" << std::endl;
}

void TextRenderer::loadCharacters()
{
	std::cout << "load" << std::endl;
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	for (unsigned char c = 0; c < 128; c++) {
		if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
			std::cout << "ERROR: Failed to load glyph " << c << " " << std::endl;
			continue;
		}

		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
		//std::cout << face->glyph->bitmap.width << std::endl;
		//terxture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
		static_cast<GLuint>(face->glyph->advance.x)
		};
		characters.insert(std::pair<GLchar, Character>(c, character));
	}
	/*for (auto it = characters.begin(); it != characters.end(); ++it) {
		std::cout << it->first << std::endl;
	}*/
	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}

void TextRenderer::rendertext(const std::string& text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color)
{
	//std::cout << "render";
	glUseProgram(shader);
	glUniform3f(glGetUniformLocation(shader, "textColor"), color.x, color.y, color.z);
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(shader, "text"), 0); // 0 corresponds to texture unit 0
	glBindVertexArray(VAO);
	glEnableVertexAttribArray(0);

	for (const char& c : text) {
		//std::cout << c;
		Character ch = characters[c];
		GLfloat xpos = x + ch.bearing.x * scale;
		GLfloat ypos = y - (ch.size.y - ch.bearing.y) * scale; //check this
		//std::cout << xpos << " " << ypos;
		
		GLfloat w = ch.size.x * scale;
		GLfloat h = ch.size.y * scale;

		GLfloat vertices[6][4] = {
			{xpos,   ypos + h, 0.0f, 0.0f},
			{xpos,   ypos,   0.0f, 1.0f},
			{xpos + w, ypos,   1.0f, 1.0f},
			{xpos,   ypos + h, 0.0f, 0.0f},
			{xpos + w, ypos,   1.0f, 1.0f},
			{xpos + w, ypos + h, 1.0f, 0.0f}

		};
		//std::cout << "texture " << ch.TextureID;
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		//std::cout << "vbo " << VBO << std::endl;
		//std::cout << "vao " << VAO<<sizeof(vertices) << std::endl;
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		x += (ch.advance >> 6) * scale;
		return;
	}
	//std::cout << std::endl;
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}
