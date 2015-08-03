#pragma once

#include "mesh.h"
#include <string>
#include <GLFW/glfw3.h>

class offReader
{

public:
	Mesh mesh;
	std::vector<GLfloat> Normals;
	Vertices MidPoint;
	std::string Format;
	std::vector<Vertices> BBox;
	std::vector<GLshort> indexData;
	std::vector<Vertices> vertexData;
	std::vector<glm::vec3> fVertexData;
	float color[3]; 
	float nColor[3];
	float bColor[3];
	float Scaling[3];
	float Shininess;
	float SpecularRef[3];

	float minX , maxX , minY , maxY , minZ , maxZ ;
	float Largest;
	offReader(std::string fPath);
	~offReader();
};

