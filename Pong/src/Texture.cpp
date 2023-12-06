//#include "Texture.h"
//#include "vendor/std_image/std_image.h"
//#define _USE_MATH_DEFINES
//#include <cmath>
//#include <iostream>
//#include <GL/glew.h>
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
//#include <vector>
//#include <map>
//#include <filesystem>
//#include <fstream>
//Texture::Texture(const std::string& path)
//	:m_RendererID(0), m_FilePath(path), m_LocalBuffer(nullptr),
//	m_Width(0), m_Height(0), m_BPP(0)
//{
//	stbi_set_flip_vertically_on_load(1);
//	m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);
//	if (m_LocalBuffer == NULL)
//		std::cout << "Error in loading";
//	glGenTextures(1, &m_RendererID);
//	(glBindTexture(GL_TEXTURE_2D, m_RendererID));
//
//	(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
//	(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
//	(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
//	(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
//	std::cout << m_Width;
//	(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
//	(glBindTexture(GL_TEXTURE_2D, 0));
//	if (m_LocalBuffer) {
//		stbi_image_free(m_LocalBuffer);
//	}
//}
//
//Texture::~Texture()
//{
//	(glDeleteTextures(1, &m_RendererID));
//}
//
//void Texture::Bind(unsigned int slot) const
//{
//	(glActiveTexture(GL_TEXTURE0 + slot));
//	(glBindTexture(GL_TEXTURE_2D, m_RendererID));
//}
//
//void Texture::Unbind() const
//{
//	(glBindTexture(GL_TEXTURE_2D, 0));
//}