#include "_skyBox.h"

_skyBox::_skyBox(std::vector<std::string> paths)
{
	
	if (TextureImage[0] = SDL_LoadBMP(paths[0].c_str())){}
	else{ printf("LOL NO!"); }
	if(TextureImage[1] = SDL_LoadBMP(paths[1].c_str())) {}
	else{ printf("LOL NO!"); };
	if(TextureImage[2] = SDL_LoadBMP(paths[2].c_str())){}
	else{ printf("LOL NO!"); };
	if(TextureImage[3] = SDL_LoadBMP(paths[3].c_str())){}
	else{ printf("LOL NO!"); };
	if(TextureImage[4] = SDL_LoadBMP(paths[4].c_str())){}
	else{ printf("LOL NO!"); };
	if(TextureImage[5] = SDL_LoadBMP(paths[5].c_str())){}
	else{ printf("LOL NO!"); };
	

	std::vector<glm::vec3>  vSkyBoxVertices =
	{
		glm::vec3(50.0f, 50.0f, 50.0f), glm::vec3(50.0f, -50.0f, 50.0f), glm::vec3(-50.0f, 50.0f, 50.0f), glm::vec3(-50.0f, -50.0f, 50.0f),
		glm::vec3(-50.0f, 50.0f, -50.0f), glm::vec3(-50.0f, -50.0f, -50.0f), glm::vec3(50.0f, 50.0f, -50.0f), glm::vec3(50.0f, -50.0f, -50.0f),
		glm::vec3(-50.0f, 50.0f, 50.0f), glm::vec3(-50.0f, -50.0f, 50.0f), glm::vec3(-50.0f, 50.0f, -50.0f), glm::vec3(-50.0f, -50.0f, -50.0f),
		glm::vec3(50.0f, 50.0f, -50.0f), glm::vec3(50.0f, -50.0f, -50.0f), glm::vec3(50.0f, 50.0f, 50.0f), glm::vec3(50.0f, -50.0f, 50.0f),
		glm::vec3(-50.0f, 50.0f, -50.0f), glm::vec3(50.0f, 50.0f, -50.0f), glm::vec3(-50.0f, 50.0f, 50.0f), glm::vec3(50.0f, 50.0f, 50.0f),
		glm::vec3(50.0f, -50.0f, -50.0f), glm::vec3(-50.0f, -50.0f, -50.0f), glm::vec3(50.0f, -50.0f, 50.0f), glm::vec3(-50.0f, -50.0f, 50.0f),
	};
	std::vector<glm::vec2>  vSkyBoxTexCoords =
	{
		glm::vec2(0.0f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec2(1.0f, 0.0f)
	};

	std::vector<glm::vec3> vSkyBoxNormals =
	{
		glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)
	};

	std::vector<GLfloat> VBO;

	for (int i = 0; i < vSkyBoxVertices.size(); i++){
		VBO.push_back((GLfloat)vSkyBoxVertices[i].x);
		VBO.push_back((GLfloat)vSkyBoxVertices[i].y);
		VBO.push_back((GLfloat)vSkyBoxVertices[i].z);

		VBO.push_back((GLfloat)vSkyBoxTexCoords[i % 4].x);
		VBO.push_back((GLfloat)vSkyBoxTexCoords[i % 4].y);

		VBO.push_back((GLfloat)vSkyBoxNormals[i / 4].x);
		VBO.push_back((GLfloat)vSkyBoxNormals[i / 4].y);
		VBO.push_back((GLfloat)vSkyBoxNormals[i / 4].z);
}

	glGenSamplers(1, &uiSampler);
	for (int i = 0; i < 6; i++){
		glGenTextures(1, &textureID[i]);
		glBindTexture(GL_TEXTURE_2D, textureID[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, TextureImage[i]->w,TextureImage[i]->h, 0, GL_RGB,GL_UNSIGNED_BYTE, TextureImage[i]->pixels);
		glGetError();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	//glewInit();
	glGenBuffers(1, &vboInd);
	glBindBuffer(GL_ARRAY_BUFFER, vboInd);
	glBufferData(GL_ARRAY_BUFFER, VBO.size()*sizeof(GLfloat), &VBO[0], GL_STATIC_DRAW_ARB);
	
	glEnableVertexAttribArray(0);
	
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	/*
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3) + sizeof(glm::vec2), 0);
	
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3) + sizeof(glm::vec2), (void*)sizeof(glm::vec3));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3) + sizeof(glm::vec2), (void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));
	*/
}


	
void _skyBox::renderSkyBox(){
	glBindVertexArray(vboInd);
	for (int i = 0; i < 6;i++)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID[i]);
	glBindSampler(i, uiSampler);
glDrawArrays(GL_TRIANGLE_STRIP, i * 4, 4);
}
}

_skyBox::~_skyBox()
{
}
