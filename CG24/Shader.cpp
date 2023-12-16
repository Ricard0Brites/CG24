#include "Shader.h"
#include <gl\GL.h>
#include <Windows.h>
#include <iostream>
#include <fstream>
#include "glm.hpp"

#include "OBJ.h"

#define FRAGMENTSHADER "FragmentShader.hlsl"
#define VERTEXSHADER "VertexShader.hlsl"

using namespace std;
using namespace glm;

Shader::Shader()
{
}

Shader::~Shader()
{
	glDeleteShader(_Shader);
}

void Shader::InitShader()
{
	_Shader = glCreateProgram();
	std::string _VertexShaderCode = "";
	LoadShader(VERTEXSHADER, _VertexShaderCode);
	const char* vsc = _VertexShaderCode.c_str();
	
	std::string _FragmentShaderCode = "";
	LoadShader(FRAGMENTSHADER, _FragmentShaderCode);
	const char* fsc = _FragmentShaderCode.c_str();

	#pragma region Vertex Shader
			//Vertex Shader
	_VertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(_VertexShader, 1, &vsc, 0);
	glCompileShader(_VertexShader);

#if _DEBUG
{
	GLint isCompiled = 0;
	glGetShaderiv(_VertexShader, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(_VertexShader, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(_VertexShader, maxLength, &maxLength, &infoLog[0]);

		// We don't need the shader anymore.
		glDeleteShader(_VertexShader);
		
		string debugMsg = "Vertex Shader Log:\n";
		for(GLchar Character : infoLog)
		{
			debugMsg += Character;
		}
		cout << debugMsg << endl;
		return;
	}
}
#endif
	#pragma endregion

	#pragma region Fragment Shader
	//Fragment Shader
	_FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(_FragmentShader, 1, &fsc, 0);
	glCompileShader(_FragmentShader);

#if _DEBUG
{
	GLint isCompiled = 0;
	glGetShaderiv(_FragmentShader, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(_FragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(_FragmentShader, maxLength, &maxLength, &infoLog[0]);

		// We don't need the shader anymore.
		glDeleteShader(_FragmentShader);

		string debugMsg = "Fragment Shader Log:\n";
		for (GLchar Character : infoLog)
		{
			debugMsg += Character;
		}
		cout << debugMsg << endl;
		return;
	}
}
#endif
	#pragma endregion

	//bind VAO to the shader program
	glAttachShader(_Shader, _VertexShader);
	glAttachShader(_Shader, _FragmentShader);
	glLinkProgram(_Shader);

	glCompileShader(_Shader);
}

void Shader::UseShader()
{
	glUseProgram(_Shader);
}

void Shader::SetProjectionMatrix(mat4 Matrix)
{
	unsigned int ProjectionMatrix = glGetUniformLocation(_Shader, "ProjectionMatrix");
	glUniformMatrix4fv(ProjectionMatrix, 1, GL_FALSE, value_ptr(Matrix));
}

void Shader::SetViewMatrix(mat4 Matrix)
{
	unsigned int ViewMatrix = glGetUniformLocation(_Shader, "ViewMatrix");
	glUniformMatrix4fv(ViewMatrix, 1, GL_FALSE, value_ptr(Matrix));
}

void Shader::SetModelMatrix(mat4 Matrix)
{
	unsigned int ModelMatrix = glGetUniformLocation(_Shader, "ModelMatrix");
	glUniformMatrix4fv(ModelMatrix, 1, GL_FALSE, value_ptr(Matrix));
}

void Shader::LoadShader(const char* File, std::string &ReturnVal)
{
	ifstream file(File);
	if (!file)
		return;

	string shaderCode((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	ReturnVal = shaderCode;
}