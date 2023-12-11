#include "Camera.h"
#include "ext.hpp"

Camera::Camera()
{
	_Location = vec3(0);
	_Rotation = vec3(0);
}

Camera::~Camera()
{

}

vec3 Camera::GetLocation()
{
	return _Location;
}

void Camera::SetLocation(vec3 NewLocation)
{
	_Location = NewLocation;
}

vec3 Camera::GetForwardVector()
{
	return vec3(inverse(GetRotatioMatrix()) * vec4(0,0,-1,1));
}

vec3 Camera::GetRightVector()
{
	return vec3(inverse(GetRotatioMatrix()) * vec4(1,0,0,1));
}

vec3 Camera::GetUpVector()
{
	return vec3(inverse(GetRotatioMatrix()) * vec4(0,1,0,1));
}

mat4 Camera::GetViewMatrix()
{
	return GetRotatioMatrix() * translate(mat4(1), -_Location);
}

mat4 Camera::GetRotatioMatrix()
{
	mat4 rotation(1);
	rotation = rotate(rotation, radians(_Rotation.y), vec3(1,0,0));
	rotation = rotate(rotation, radians(_Rotation.z), vec3(0,1,0));
	return rotation;
}

void Camera::AddOffset(vec3 DirectionVector, float Magnitude)
{
	_Location += DirectionVector * Magnitude;
}

void Camera::AddOffset(vec3 Offset)
{
	_Location += Offset;
}

void Camera::AddPitch(float Offset)
{
	if (_Rotation.y + Offset > 88 || _Rotation.y + Offset < -88)
		return;
	_Rotation.y += Offset;
}

void Camera::AddYaw(float Offset)
{
	_Rotation.z += Offset;
	
	if (_Rotation.z > 360)
		_Rotation.z -= 360;
}
