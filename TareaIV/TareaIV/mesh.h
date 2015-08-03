#include "Triangle.h"
#include <vector>

class Face{
public:
	std::vector<Triangle> Faces;
	float x, y, z;
};



class Mesh
{
public:
	glm::quat RotationMatrix; //Quaternion
	glm::vec3 DisplacementVector;
	std::vector<Face> Faces;
	Mesh();
	~Mesh();
};
