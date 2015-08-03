#pragma once
#include <gtc/quaternion.hpp>
#include <glm.hpp>
class Vertices
{
public:
	glm::vec3 Vertex; //i-j-k


public:
	Vertices();
	Vertices(float v0, float v1, float v2);
	~Vertices();
};

