#version 330 core

out vec4 Color;

in vec3 VertexColor;
in vec3 VertexNormal;
in vec3 LightDirection;

void main()
{
	float LightIntensity = max(dot(normalize(VertexNormal), normalize(LightDirection)), 0);
	vec3 DiffuseMap = LightIntensity * vec3(0.5,0.5,0.5);

	Color = vec4(DiffuseMap, 1) * vec4(VertexColor, 1);
}