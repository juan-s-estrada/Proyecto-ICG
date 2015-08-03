#include "Vertices.h"


Vertices::Vertices(){
	Vertex = glm::vec3(0.0f, 0.0f, 0.0f);

}



Vertices::Vertices(float v0, float v1, float v2)
{
	Vertex = glm::vec3(v0, v1, v2);
}


Vertices::~Vertices()
{
}
