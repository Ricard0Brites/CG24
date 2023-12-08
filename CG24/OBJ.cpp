#include "OBJ.h"
#include <iostream>
#include <fstream>

#ifndef STRINGSTARTSWITH

#define STRINGSTARTSWITH(String, StringToLookFor) (String.rfind(StringToLookFor, 0) == 0)

#endif
#define ZEROARRAY(Array) memset(Array, 0, sizeof(Array));

OBJ::OBJ()
{
}

OBJ::~OBJ()
{
}

bool OBJ::LoadOBJ(const char* OBJFile)
{
	std::ifstream file(OBJFile);

	if(!file)
	{
		std::cout << "OBJ File Not Available." << std::endl;
		return false;
	}

	std::vector<Location> locations;
	std::vector<Normal> normals;
	char mtlFileName[100] = {};

	std::string currentLine;

	int counter = 0;

	while(std::getline(file, currentLine))
	{
		if(STRINGSTARTSWITH(currentLine, "v "))
		{
			Location location;
			sscanf_s(currentLine.c_str(), "v %f %f %f", &location.X, &location.Y, &location.Z);
			locations.push_back(location);
			continue;
		}
		if (STRINGSTARTSWITH(currentLine, "vn "))
		{
			Normal normal;

			sscanf_s(currentLine.c_str(), "vn %f %f %f", &normal.X, &normal.Y, &normal.Z);
			normals.push_back(normal);
			continue;
		}
		if (STRINGSTARTSWITH(currentLine, "f"))
		{
			int localVertices[3], localNormals[3];
			ZEROARRAY(localVertices);
			ZEROARRAY(localNormals);

			int valuesFetched = sscanf_s(currentLine.c_str(), "f %d//%d %d//%d %d//%d", &localVertices[0], &localNormals[0], &localVertices[1], &localNormals[1], &localVertices[2], &localNormals[2]);
			if(valuesFetched == 6)
				AddObjectData(localVertices, sizeof(localVertices) / sizeof(int), localNormals, sizeof(localNormals) / sizeof(int), mtlFileName, locations, normals);
			continue;
		}
		if (STRINGSTARTSWITH(currentLine, "mtllib"))
		{
			char fileName[100];

			sscanf_s(currentLine.c_str(), "mtllib %s", fileName, (int)sizeof(fileName));
			LoadMtl(fileName);
			continue;
		}
		if(STRINGSTARTSWITH(currentLine, "usemtl"))
		{
			sscanf_s(currentLine.c_str(), "usemtl %s", mtlFileName, (int)sizeof(mtlFileName));
			continue;
		}
		
	}
	return true;
}

std::vector<float> OBJ::GetVertices()
{
	return _VertexData;
}

const int OBJ::GetNumOfVertices()
{
	return (int)_VertexData.size() / 9;
}

void OBJ::LoadMtl(const char* MatFile)
{
	std::ifstream file(MatFile);

	if(!file)
	{
		std::cout << "Material File Not Available." << std::endl;
		return;
	}

	std::string currentLine = "";
	while(std::getline(file, currentLine))
	{
		if(STRINGSTARTSWITH(currentLine, "Kd"))
		{
			Color &color = _ColorMap[MatFile];
			sscanf_s(currentLine.c_str(), "Kd %f %f %f", &color.R, &color.G, &color.B);
		}
		if(STRINGSTARTSWITH(currentLine, "newmtl"))
		{
			sscanf_s(currentLine.c_str(), "newmtl %s", MatFile, (int)sizeof(MatFile));
			_ColorMap[MatFile] = Color();
		}
	}
}

void OBJ::AddObjectData(int *VertexLocations, int VertexLocationSize, int *VertexNormals, int VertexNormalSize, const char* MtlFile, std::vector<Location> Vertices, std::vector<Normal> Normals)
{
	if(VertexLocationSize != VertexNormalSize)
	{
		std::cout << "Arrays Of Different Size are not allowed. OBJ::AddObjectData()" << std::endl;
	}

	for(int i = 0; i < VertexLocationSize; ++i)
	{

		Color color = _ColorMap[MtlFile];
		Location location = Vertices[VertexLocations[i] - 1];
		Normal normal = Normals[VertexNormals[i] - 1];

		_VertexData.push_back(location.X); 
		_VertexData.push_back(location.Y); 
		_VertexData.push_back(location.Z); 
		
		_VertexData.push_back(color.R); 
		_VertexData.push_back(color.G); 
		_VertexData.push_back(color.B);
		
		_VertexData.push_back(normal.X); 
		_VertexData.push_back(normal.Y); 
		_VertexData.push_back(normal.Z); 
	}
}
