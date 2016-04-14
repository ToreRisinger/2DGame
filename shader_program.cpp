#include "shader_program.h"
#include "debug_output.h"
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

ShaderProgram::ShaderProgram()
{
	_programID = 0;
	_vertexShaderID = 0;
	_fragmentShaderID = 0;
}

ShaderProgram::~ShaderProgram()
{

}

void ShaderProgram::compileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath)
{
	_programID = glCreateProgram();

	_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	if (_vertexShaderID == 0)
	{
		error_output("Vertex shader failed to be created!");
	}

	_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	if (_fragmentShaderID == 0)
	{
		error_output("Fragment shader failed to be created!");
	}

	compileShader(vertexShaderFilePath, _vertexShaderID);
	compileShader(fragmentShaderFilePath, _fragmentShaderID);
}

void ShaderProgram::linkShaders()
{
	glAttachShader(_programID, _vertexShaderID);
	glAttachShader(_programID, _fragmentShaderID);

	glBindAttribLocation(_programID, 0, "inPos");

	glLinkProgram(_programID);

	GLint isLinked = 0;
	glGetProgramiv(_programID, GL_LINK_STATUS, (int *)&isLinked);
	if (isLinked == 0)
	{
		GLint maxLength = 0;
		glGetProgramiv(_programID, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<char> errorLog(maxLength);
		glGetProgramInfoLog(_programID, maxLength, &maxLength, &errorLog[0]);

		glDeleteProgram(_programID);
		glDeleteShader(_vertexShaderID);
		glDeleteShader(_fragmentShaderID);

		error_output("Shaders failed to link!" + errorLog[0]);
	}

	glDetachShader(_programID, _vertexShaderID);
	glDetachShader(_programID, _fragmentShaderID);
	glDeleteShader(_vertexShaderID);
	glDeleteShader(_fragmentShaderID);
}

GLuint ShaderProgram::getUniformLocation(const std::string uniformName)
{
	GLuint location = glGetUniformLocation(_programID, uniformName.c_str());
	if (location == GL_INVALID_INDEX)
	{
		error_output("Uniform " + uniformName + " not found in shader!");
	}

	return location;
}

void ShaderProgram::use()
{
	glUseProgram(_programID);
	
}

void ShaderProgram::unuse()
{
	glUseProgram(0);
}

void ShaderProgram::compileShader(const std::string& filePath, GLuint id)
{
	std::ifstream vertexFile(filePath);
	if (vertexFile.fail())
	{
		perror(filePath.c_str());
		error_output("Failed to open " + filePath + "!");
	}

	std::string fileContents{};
	std::string line{};

	while (std::getline(vertexFile, line))
	{
		fileContents += line + "\n";
	}

	vertexFile.close();

	const char* contentsPtr = fileContents.c_str();
	glShaderSource(id, 1, &contentsPtr, nullptr);

	glCompileShader(id);

	GLint success = 0;
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);

	if (success == GL_FALSE)
	{
		GLint maxLength = 0;

		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<char> errorLog(maxLength);
		glGetShaderInfoLog(id, maxLength, &maxLength, &errorLog[0]);

		glDeleteShader(id);

		error_output("Shader " + filePath + " failed to compile: " + errorLog[0]);
	}
}

GLuint ShaderProgram::getProgramID()
{
	return _programID;
}
