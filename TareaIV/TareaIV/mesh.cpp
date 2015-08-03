#include "mesh.h"


Mesh::Mesh()
{
	RotationMatrix = glm::fquat(0.0f,0.0f,0.0f,0.0f);
	DisplacementVector = glm::vec3(0.f, 0.f, 0.f);


}


Mesh::~Mesh()
{
}



