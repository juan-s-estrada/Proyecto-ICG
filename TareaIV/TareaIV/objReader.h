#pragma once

#include <string>
#include "mesh.h"

class objReader
{
public:
	Mesh mesh;




	objReader(std::string fpath);
	~objReader();
};

