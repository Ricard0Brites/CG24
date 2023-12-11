#pragma once
#include "glm.hpp"

using namespace glm;


class Camera
{

public:
	Camera();
	~Camera();

	vec3 GetLocation();
	void SetLocation(vec3 NewLocation);
	vec3 GetForwardVector();
	vec3 GetRightVector();
	vec3 GetUpVector();

	mat4 GetViewMatrix();
	mat4 GetRotatioMatrix();

	void AddOffset(vec3 DirectionVector, float Magnitude);
	void AddOffset(vec3 Offset);
	void AddPitch(float Offset);
	void AddYaw(float Offset);

private:
	vec3 _Location, _Rotation;
};

