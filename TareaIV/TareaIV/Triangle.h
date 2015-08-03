#pragma once
#include "Vertices.h"


class Triangle
{
public:
	Vertices tVertices[3]; //x,y,z
	float tNormal[3];
	float tNormalVector[3];


	void CrossProduct(Vertices v0, Vertices v1,Vertices v2);
	void NormalVector();
	Triangle(Vertices v0, Vertices  v1, Vertices v2);
	~Triangle();
};

