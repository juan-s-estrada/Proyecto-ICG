#pragma once
#include "SDL/SDL.h"
#include <vector>
#include <String>
#include <glm.hpp>
#include <GL/glew.h>


class _skyBox
{
public:

	GLuint textureID[6];
	GLuint vboInd;
	GLuint uiSampler;
	SDL_Surface *TextureImage[6];
	
	_skyBox(std::vector<std::string> Paths );
	void renderSkyBox();
	
	~_skyBox();
	void _renderSkyBox(GLint );

};

