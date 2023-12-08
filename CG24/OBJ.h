#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <string>

class OBJ
{
public:
		OBJ();
		~OBJ();
	bool LoadOBJ(const char *OBJFile);

	std::vector<float> GetVertices();

	const int GetNumOfVertices();

private:
	struct Location { float X, Y, Z; };
	struct Color { float R = 1.f, G = 1.f, B = 1.f; };
	struct Normal { float X, Y, Z; };

	void LoadMtl(const char* MatFile);

	std::vector<float> _VertexData;
	std::map<std::string, Color> _ColorMap;

	void AddObjectData(int *VertexLocations, int VertexLocationSize, int *VertexNormals, int VertexNormalSize, const char* MtlFile, std::vector<Location> Vertices, std::vector<Normal> Normals);
};