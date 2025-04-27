#include "Shader.h"

#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
	: mProgramID(0)
{
	std::string vertexCode = loadFile(vertexPath);
	std::string fragmentCode = loadFile(fragmentPath);

	GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexCode);
	GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentCode);

	mProgramID = glCreateProgram();
	glAttachShader(mProgramID, vertexShader);
	glAttachShader(mProgramID, fragmentShader);
	glLinkProgram(mProgramID);

	if (!checkLinkStatus(mProgramID))
	{
		std::cout << "[Shader.cpp(shader)]: Failed to link program" << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	std::cout << "[Shader.cpp(shader)]: Shader initialization completed successfully." << std::endl;
}

Shader::~Shader()
{
	glDeleteProgram(mProgramID);
}


void Shader::Use() const
{
	glUseProgram(mProgramID);
}

void Shader::setBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(mProgramID, name.c_str()), (int)value);
}
void Shader::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(mProgramID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(mProgramID, name.c_str()), value);
}

void Shader::setVec3(const std::string& name, const glm::vec3& vec) const
{
	glUniform3fv(glGetUniformLocation(mProgramID, name.c_str()), 1, &vec[0]);
}

void Shader::setVec4(const std::string& name, const glm::vec4& vec) const
{
	glUniform4fv(glGetUniformLocation(mProgramID, name.c_str()), 1, &vec[0]);
}

void Shader::setMat4(const std::string& name, const glm::mat4& mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(mProgramID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}



std::string Shader::loadFile(const std::string& path)
{
	std::ifstream file(path);
	std::stringstream buffer;
	buffer << file.rdbuf();
	return buffer.str();
}

GLuint Shader::compileShader(GLenum type, const std::string& code)
{
	GLuint shader = glCreateShader(type);
	const char* source = code.c_str();
	glShaderSource(shader, 1, &source, nullptr);
	glCompileShader(shader);

	if (!checkCompileShader(shader))
	{
		std::cout << "[Shader.cpp]: Compilation Failed." << std::endl;
	}

	return shader;
}

bool Shader::checkCompileShader(GLuint shader)
{
	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char infoLog[512];
		glGetShaderInfoLog(shader, 512, nullptr, infoLog);
		std::cout << "[Shader.cpp]: Compile Error: " << infoLog << std::endl;
		return false;
	}

	return true;
}

bool Shader::checkLinkStatus(GLuint program)
{
	GLint success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
		char infoLog[512];
		glGetProgramInfoLog(program, 512, nullptr, infoLog);
		std::cout << "[Shader.cpp]: Link error: " << infoLog << std::endl;
		return false;
	}

	return true;
}

