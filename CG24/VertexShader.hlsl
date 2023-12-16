#version 330 core

in vec3 Position;
in vec3 Color;
in vec3 Normal;

out vec3 VertexColor;
out vec3 VertexNormal;
out vec3 LightDirection;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

void main()
{
	vec3 LightLocation = vec3(10, 25, 7);
	vec4 VertexLocation = ModelMatrix * vec4(Position, 1);

	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(Position, 1.f);
	VertexColor = Color;

	VertexNormal = (ModelMatrix * vec4(Normal, 0)).xyz;
    LightDirection = LightLocation - VertexLocation.xyz;
}