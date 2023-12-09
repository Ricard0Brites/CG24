#pragma once
#include<glew.h>
#include <ext.hpp>
#include <string>

class OBJ;

typedef glm::mat4 mat4;
typedef glm::vec3 vec3;

class Shader
{
public:
	Shader();
	~Shader();

	void InitShader();
	void UseShader();
	void SetProjectionMatrix(mat4 Matrix);
	void SetViewMatrix(mat4 Matrix);
	void SetModelMatrix(mat4 Matrix);

private:
	unsigned int _Shader;
	unsigned int _VertexShader, _FragmentShader;
	void LoadShader(const char* File, std::string &ReturnVal);
};