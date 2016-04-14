#pragma once

#include <GL/glew.h>
#include <string>

class ShaderProgram
{
public:
	ShaderProgram();
	~ShaderProgram();

	void compileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath);
	void linkShaders();

	GLuint getUniformLocation(const std::string uniformName);

	void use();
	void unuse();
	GLuint getProgramID();

private:
	void compileShader(const std::string& filePath, GLuint id);

	GLuint _programID;

	GLuint _vertexShaderID;

	GLuint _fragmentShaderID;

	GLint m_vertex_attrib_location_position = -1;
	GLint m_vertex_attrib_location_tex_coord = -1;
};