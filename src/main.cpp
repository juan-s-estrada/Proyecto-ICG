#include <GL/glew.h>
#include "offReader.h"
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/type_ptr.hpp> 
#include <string>
#include <iostream>
#include <AntTweakBar.h>
#include "GLSLProgram.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtx/transform2.hpp"
#include "btBulletDynamicsCommon.h"

//------SDL------/
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

 using namespace std;

 //The music that will be played
 Mix_Music *gMusic = NULL;

 //The sound effects that will be used
 Mix_Chunk *gScratch = NULL;
 Mix_Chunk *gHigh = NULL;
 Mix_Chunk *gMedium = NULL;
 Mix_Chunk *gLow = NULL;
//---Textures, thanks SDL!---//
SDL_Surface *TextureImage[1];

glm::vec3 Ambient = glm::vec3(0.1f, 0.1f, 0.8f);
glm::mat4 projection, view;
int Selection = -1;
float NormalColor[3] = { 0.5f, 0.5f, 0.5f };
float ModelColor[3] = { 0.5f, 0.5f, 0.5f };
float BBColor[3] = { 0.5f, 0.5f, 0.5f };
float gTranslation[] = { 0.0f, 0.0f, 0.0f };
float g_Rotation[] = { 0.0f, 0.0f, 0.0f, 1.0f };
float ScaleFactor[] = { 1.f, 1.f,1.f };
int DummySelection = 0;
bool ZBuffer = true;
bool CullFace = true;
TwBar *bar, *barLights;
GLuint indexes[32];
GLuint textureID[32];
GLuint VBOid[32][6];

GLushort BBoxIndices[] = { 0, 1, 1, 5, 5, 4, 4, 0
, 3, 2, 2, 6, 6, 7, 7, 3
, 1, 2, 5, 6, 4, 7, 0, 3

};

float ModelShininess;
float SpecularColor[3];
typedef enum 
{
	Immediate, VertexArray, DisplayList, VBO

}RenderingMode;

glm::vec3 LightPos1 = glm::vec3(0.5f, 0.5f, 0.5f);
glm::vec3 LightPos2 = glm::vec3(1.0f, 1.0f, 1.0f);
glm::vec3 Col1 = glm::vec3(.2f, .2f, .2f);
glm::vec3 Col2 = glm::vec3(.2f, .2f, .2f);
glm::vec3 Col1S = glm::vec3(.2f, .2f, .2f);
glm::vec3 Col2S = glm::vec3(.2f, .2f, .2f);
typedef enum {Point,Wireframe,Filled }RenderingStyle;
bool success;
CGLSLProgram * glslProgram[3];
std::vector<offReader> offObjects;
bool IOtest = false;
bool Orthogonal = false;
bool DrawBBox = true;
bool DrawNormals =true;
bool DrawBBoxDummy = true;
bool DrawNormalsDummy = true;
int NumModels = 0;

RenderingStyle Style = Filled;
bool initGlew()
{
	if (glewInit()){ std::cout << "Error"; return false; }
		
	else
	{
		printf("Vendor: %s \n", glGetString(GL_VENDOR));
		printf("Renderer: %s \n", glGetString(GL_RENDERER));
		printf("OpenGL Version: %s \n", glGetString(GL_VERSION));
		printf("GLSL Version: %s \n", glGetString(GL_SHADING_LANGUAGE_VERSION));

		printf("\n\nShader: Color \n");
		for (int i = 0; i <= 2; i++)
		glslProgram[i] = new CGLSLProgram();
		//-----Per Pixel shader that-s ought to be upgraded with bumpmapping----//
			
			glslProgram[0]->loadFromFile("../shaders/perPixel.vert");
			glslProgram[0]->loadFromFile("../shaders/perPixel.frag");
			glslProgram[0]->create();
			glslProgram[0]->loadUniformVariables();
			glslProgram[0]->loadAttributeVariables();

		//--------Not at all important, previusly used for normals and bounding boxes-----///
			glslProgram[1]->loadFromFile("../shaders/Color.vert");
			glslProgram[1]->loadFromFile("../shaders/Color.frag");
			glslProgram[1]->create();
			glslProgram[1]->loadUniformVariables();
			glslProgram[1]->loadAttributeVariables();
		//--------Sky Box-----///
			glslProgram[2]->loadFromFile("../shaders/SkyBox.vert");
			glslProgram[2]->loadFromFile("../shaders/SkyBox.frag");
			glslProgram[2]->create();
			glslProgram[2]->loadUniformVariables();
			glslProgram[2]->loadAttributeVariables();

			return true;}
}

void loadTextures(){
	if ((TextureImage[0] = SDL_LoadBMP("../SkyBox.bmp")))
	{

		/* Set the status to true */
		bool Status = true;
		//OutputDebugStringW(L"My output string.");
		/* Create The Texture */
		glGenTextures(1, &textureID[0]);

		/* Typical Texture Generation Using Data From The Bitmap */
		glBindTexture(GL_TEXTURE_2D, textureID[0]);

		/* Generate The Texture */
		glTexImage2D(GL_TEXTURE_CUBE_MAP, 0, 3, TextureImage[0]->w,
			TextureImage[0]->h, 0, GL_RGB,
			GL_UNSIGNED_BYTE, TextureImage[0]->pixels);
		glGetError();

		/* Linear Filtering */
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}


}
void cosa(){

	if (Selection >0 && Selection <= offObjects.size()){
		offObjects[Selection - 1].mesh.RotationMatrix[0] = g_Rotation[0];
		offObjects[Selection - 1].mesh.RotationMatrix[1] = g_Rotation[1];
		offObjects[Selection - 1].mesh.RotationMatrix[2] = g_Rotation[2];
		offObjects[Selection - 1].mesh.RotationMatrix[3] = g_Rotation[3];

		offObjects[Selection - 1].Scaling[0] = ScaleFactor[0];
		offObjects[Selection - 1].Scaling[1] = ScaleFactor[1];
		offObjects[Selection - 1].Scaling[2] = ScaleFactor[2];

		offObjects[Selection - 1].mesh.DisplacementVector[0] = gTranslation[0];

		offObjects[Selection - 1].mesh.DisplacementVector[1] = gTranslation[1];

		offObjects[Selection - 1].mesh.DisplacementVector[2] = gTranslation[2];

		offObjects[Selection - 1].bColor[0] = BBColor[0];
		offObjects[Selection - 1].bColor[1] = BBColor[1];
		offObjects[Selection - 1].bColor[2] = BBColor[2];

		offObjects[Selection - 1].nColor[0] = NormalColor[0];
		offObjects[Selection - 1].nColor[1] = NormalColor[1];
		offObjects[Selection - 1].nColor[2] = NormalColor[2];



		offObjects[Selection - 1].color[0] = ModelColor[0];
		offObjects[Selection - 1].color[1] = ModelColor[1];
		offObjects[Selection - 1].color[2] = ModelColor[2];
		offObjects[Selection - 1].Shininess = ModelShininess;
		offObjects[Selection - 1].SpecularRef[0] = SpecularColor[0];
		offObjects[Selection - 1].SpecularRef[1] = SpecularColor[1];
		offObjects[Selection - 1].SpecularRef[2] = SpecularColor[2];

	}

}
static void initVBO( int i){
glewInit();
glGenBuffers(1, &VBOid[i][0]);
glGenBuffers(1, &VBOid[i][1]);
glGenBuffers(1, &VBOid[i][2]);
glGenBuffers(1, &VBOid[i][3]);
glGenBuffers(1, &VBOid[i][4]);

//---Carga la geometria basica ---// 
glBindBuffer(GL_ARRAY_BUFFER, VBOid[i][0]);
glBufferData(GL_ARRAY_BUFFER, offObjects[i].fVertexData.size()  * sizeof(glm::vec3), &offObjects[i].fVertexData[0], GL_STATIC_DRAW_ARB);
glEnableVertexAttribArray(0);
glEnableVertexAttribArray(1);
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*) (offObjects[i].fVertexData.size()/2 * sizeof(glm::vec3)) );
glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBOid[i][1]);
glBufferData(GL_ELEMENT_ARRAY_BUFFER, offObjects[i].indexData.size() * sizeof(GLshort), &offObjects[i].indexData[0], GL_STATIC_DRAW_ARB);

//--Carga el BBox y sus indices--//
glBindBuffer(GL_ARRAY_BUFFER_ARB, VBOid[i][2]);
glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBOid[i][3]);
glBufferDataARB(GL_ARRAY_BUFFER_ARB, 8 * sizeof(Vertices), &offObjects[i].BBox[0], GL_STATIC_DRAW_ARB);
glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER, 24 * sizeof(GLshort), &BBoxIndices[0], GL_STATIC_DRAW_ARB);

//--Carga las normales--//
glBindBuffer(GL_ARRAY_BUFFER_ARB, VBOid[i][4]);
glBufferDataARB(GL_ARRAY_BUFFER_ARB, offObjects[i].Normals.size()  * sizeof(GL_FLOAT), &offObjects[i].Normals[0], GL_STATIC_DRAW_ARB);
glBindBufferARB(GL_ARRAY_BUFFER_ARB, NULL);
glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, NULL);

//--Normales per vertex--//


}
void LoadModel(std::string path){
	if (offObjects.size() <= 32) {
		offObjects.push_back(offReader(path));
		initVBO(offObjects.size() - 1);
		//(offObjects.size() - 1);
		NumModels++;
		if (Selection == -1) { Selection = 1; DummySelection = 1; cosa(); }
	}
	else{
		std::cout << "Ya existen 32 objetos" << std::endl;
	
	}};
static	void  OnMousePos(GLFWwindow* window, double xpos, double ypos)
{
	double rxpos, rypos;
	glfwGetCursorPos(window, &rxpos, &rypos);

	TwMouseMotion(int(rxpos), int(rypos));
	cosa();

}
static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{

	if (!TwEventMouseButtonGLFW(button, action)){
		TwMouseButton(TW_MOUSE_PRESSED, TW_MOUSE_LEFT);

	}
	cosa();



}
void addObject(GLFWwindow* window, int count, const char** paths)
{
	int i;
	for (i = 0; i < count; i++){
		std::string path(paths[i]);
		LoadModel(path);
	}
	
}
void TW_CALL Scale(void *clientData)
{

}
void TW_CALL Select(void *clientData)
{
	Selection = DummySelection;
	if (Selection <= offObjects.size() && Selection > 0){
		g_Rotation[0] = offObjects[Selection-1].mesh.RotationMatrix[0];
		g_Rotation[1] = offObjects[Selection-1].mesh.RotationMatrix[1];
		g_Rotation[2] = offObjects[Selection-1].mesh.RotationMatrix[2];
		g_Rotation[3] = offObjects[Selection-1].mesh.RotationMatrix[3];
		gTranslation[0] = offObjects[Selection - 1].mesh.DisplacementVector[0];
		gTranslation[1] = offObjects[Selection - 1].mesh.DisplacementVector[1];
		gTranslation[2] = offObjects[Selection - 1].mesh.DisplacementVector[2];
		ScaleFactor[0] = offObjects[Selection - 1].Scaling[0];
		ScaleFactor[1] = offObjects[Selection - 1].Scaling[1];
		ScaleFactor[2] = offObjects[Selection - 1].Scaling[2];
		BBColor[0] = offObjects[Selection - 1].bColor[0];
		BBColor[0] = offObjects[Selection - 1].bColor[0];
		BBColor[0] = offObjects[Selection - 1].bColor[0];
		NormalColor[0] = offObjects[Selection - 1].nColor[0];
		NormalColor[1] = offObjects[Selection - 1].nColor[1];
		NormalColor[2] = offObjects[Selection - 1].nColor[2];
		ModelColor[0] = offObjects[Selection - 1].color[0];
		ModelColor[1] = offObjects[Selection - 1].color[1];
		ModelColor[2] = offObjects[Selection - 1].color[2];
		ModelShininess = offObjects[Selection - 1].Shininess;
		SpecularColor[0] = offObjects[Selection - 1].SpecularRef[0];
		SpecularColor[1] = offObjects[Selection - 1].SpecularRef[1];
		SpecularColor[2] = offObjects[Selection - 1].SpecularRef[2];
	}



}
void TW_CALL ApplyChanges(void *clientData)
{
	DrawBBox = DrawBBoxDummy;
	DrawNormals = DrawNormalsDummy;
	
}
inline void setAntTweak(){
	TwInit(TW_OPENGL, NULL);
	bar = TwNewBar("Objects");
	barLights = TwNewBar("Lights");
	TwAddVarRW(bar, "ObjRotation", TW_TYPE_QUAT4F, &g_Rotation,
		" label='Object rotation' opened=true help='Change the object orientation.' ");
	TwAddVarRW(bar, "Draw Bounding Box", TW_TYPE_BOOL8, &DrawBBoxDummy,
		"group='Rendering Options'  label='Draw Bounding Box'  help='Toggles between drawing/not drawing the Bounding Box' ");
	TwAddVarRW(bar, "Z-Buffer", TW_TYPE_BOOL8, &ZBuffer,
		"group='Rendering Options'  label='Z-Buffer'  help='Toggles between using/not using the Z-Buffer ' ");
	TwAddVarRW(bar, "Back face culling", TW_TYPE_BOOL8, &CullFace,
		"group='Rendering Options'  label='BackFaceCulling'  help='Toggles between using/not using the Back face culling ' ");


	TwAddVarRW(bar, "Draw per face normals", TW_TYPE_BOOL8, &DrawNormalsDummy, " group='Rendering Options' label='Draw per face normals'  help='Toggles between drawing/not drawing the face normals' ");
	TwAddButton(bar, "Apply Options", ApplyChanges, NULL, "group='Rendering Options' label='Apply Options' ");
	TwAddVarRW(bar, "X", TW_TYPE_FLOAT, &ScaleFactor[0], " group='Scale' label='Scale X' step=0.01");
	TwAddVarRW(bar, "Y", TW_TYPE_FLOAT, &ScaleFactor[1], "group='Scale' label='Scale Y' step=0.01");
	TwAddVarRW(bar, "Z", TW_TYPE_FLOAT, &ScaleFactor[2], "group='Scale' label='Scale Z' step=0.01");
	TwAddButton(bar, "Apply", Scale, NULL, "group='Scale' label='Apply' ");
	TwAddVarRW(bar, "Perspective/Projection", TW_TYPE_BOOL8, &Orthogonal, "  label='Perspective/Projection'  help='Toggles between perspective/projection matrices' ");


	TwAddVarRO(bar, "Models:", TW_TYPE_INT32, &NumModels, "group='Selection' label='Number of Models'");
	TwAddVarRW(bar, "Selected model", TW_TYPE_INT32, &DummySelection, "group='Selection' label='Selected model' ");
	TwAddButton(bar, "Select", Select, NULL, " group='Selection' label='Select' ");

	TwAddVarRW(bar, "BB Color", TW_TYPE_COLOR3F, &BBColor, "group='Material' label='BB Color' ");
	TwAddVarRW(bar, "Diffuse", TW_TYPE_COLOR3F, &ModelColor, "group='Material' label='Model Color' ");
	TwAddVarRW(bar, "Normal Color", TW_TYPE_COLOR3F, &NormalColor, "group='Material' label='Normal Color' ");
	TwAddVarRW(bar, "Shininess", TW_TYPE_FLOAT, &ModelShininess, "group='Material' label='Shininess'  ");
	TwDefine("Objects/Shininess min = 0");
	TwAddVarRW(bar, "Specular Color", TW_TYPE_COLOR3F, &SpecularColor, "group='Material' label='SpecularColor' ");


	TwAddVarRW(bar, "X Displacement", TW_TYPE_FLOAT, &gTranslation[0], "group='Displacement' label='X Displacement' step=0.01 ");
	TwAddVarRW(bar, "Y Displacement", TW_TYPE_FLOAT, &gTranslation[1], "group='Displacement' label='Y Displacement' step=0.01 ");
	TwAddVarRW(bar, "Z Displacement", TW_TYPE_FLOAT, &gTranslation[2], "group='Displacement' label='Z Displacement' step=0.01 ");

	TwAddVarRW(barLights, "Light 1 X ", TW_TYPE_FLOAT, &LightPos1[0], "group='Lights' label='L1 X' step=0.01 ");
	TwAddVarRW(barLights, "Light 1 Y ", TW_TYPE_FLOAT, &LightPos1[1], "group='Lights' label='L1 Y' step=0.01 ");
	TwAddVarRW(barLights, "Light 1 Z ", TW_TYPE_FLOAT, &LightPos1[2], "group='Lights' label='L1 Z' step=0.01 ");
	TwAddVarRW(barLights, "Light 1 Diffuse", TW_TYPE_COLOR3F, &Col1, "group='Lights' label='L1 Dif' ");
	TwAddVarRW(barLights, "Light 1 Specular", TW_TYPE_COLOR3F, &Col1S, "group='Lights' label='L1 Spc' ");

	TwAddVarRW(barLights, "Light 2 X ", TW_TYPE_FLOAT, &LightPos2[0], "group='Lights' label='L2 X' step=0.01 ");
	TwAddVarRW(barLights, "Light 2 Y ", TW_TYPE_FLOAT, &LightPos2[1], "group='Lights' label='L2 Y' step=0.01 ");
	TwAddVarRW(barLights, "Light 2 Z ", TW_TYPE_FLOAT, &LightPos2[2], "group='Lights' label='L2 Z' step=0.01 ");
	TwAddVarRW(barLights, "Light 2 Difusse", TW_TYPE_COLOR3F, &Col2, "group='Lights' label='L2 Dif' ");
	TwAddVarRW(barLights, "Light 2 Spc", TW_TYPE_COLOR3F, &Col2S, "group='Lights' label='L2 Spc' ");
	TwAddVarRW(barLights, "Ambient", TW_TYPE_COLOR3F, &Ambient, "group='Lights' label='Ambient' ");



	TwEnumVal Styles[] = { { Point, "Point" }, { Wireframe, "Wireframe" }, { Filled, "Filled" } };
	TwType StyleOpt;
	StyleOpt = TwDefineEnum("Style", Styles, 3);
	TwAddVarRW(bar, "Style", StyleOpt, &Style, NULL);





}

#ifdef _WIN32
#undef main
#endif

void loadMusic(){

	gMusic = Mix_LoadMUS("../Soundtrack/Saturnsky.mp3");
	if (gMusic == NULL)
	{
		printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
		success = false;
	}
	else{ printf("So loaded"); 
	//Mix_PlayMusic(gMusic, -1);
	
	}

	
}









int main(){
	std::vector<std::string> Paths;
	Paths.push_back("../files/Stars_mz.bmp");
	Paths.push_back("../files/Stars_pz.bmp");
	Paths.push_back("../files/Stars_mx.bmp");
	Paths.push_back("../files/Stars_px.bmp");
	Paths.push_back("../files/Stars_my.bmp");
	Paths.push_back("../files/Stars_py.bmp");

	if (SDL_Init( SDL_INIT_AUDIO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}


	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		success = false;
	}
	loadMusic();

	const float NCP = 1.f;
	const float FCP = 50.0f;
	const float fAngle = 45.f;
	
	int it=0;
	GLFWwindow* window;
	
	if (!glfwInit())
		std::exit(EXIT_FAILURE);
	window = glfwCreateWindow(1024, 768, "Simple example", NULL, NULL);
	 //window = glfwCreateWindow(1366, 768, "Kessel Run", glfwGetPrimaryMonitor(), nullptr);

	if (!window)
	{
		glfwTerminate();
		std::exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	
	
	       
	TwWindowSize(1024, 768);
	glfwSetCursorPosCallback(window, (GLFWcursorposfun)OnMousePos);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetDropCallback(window, addObject);
	const char title[] = "Tarea IV";
	glfwSetWindowTitle(window,title);
	setAntTweak();
	double time = glfwGetTime(), dt = 0;            // Current time and elapsed time
	double frameDTime = 0, frameCount = 0, fps = 0; // Framerate

	initGlew();


//	loadTextures();
	while (!glfwWindowShouldClose(window))
	{

		dt = glfwGetTime() - time;
		if (dt < 0) dt = 0;
		time += dt;

		float ratio;
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		if (Style == Filled)glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		if (Style == Wireframe)glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		if (Style == Point)glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);

		glEnable(GL_LINE_SMOOTH);
		if (CullFace) glEnable(GL_CULL_FACE);
		if (ZBuffer) glEnable(GL_DEPTH_TEST);
		
		

		if (Orthogonal) { projection = glm::ortho(-(float)width / (float)height, (float)width / (float)height, -1.f, 1.f, 1.0f, 1000.0f); }
		else{ projection = glm::perspective(45.0f, (float)width / (float)height, 1.0f, 1000.0f); }
		view = glm::lookAt(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	//	drawSkyBox();

	for (int i = 0; i < offObjects.size(); i++){
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::scale(glm::vec3(offObjects[i].Scaling[0], offObjects[i].Scaling[1], offObjects[i].Scaling[2])) * glm::translate(model, offObjects[i].mesh.DisplacementVector)*glm::scale(model, glm::vec3(offObjects[i].Largest, offObjects[i].Largest,offObjects[i].Largest))*glm::translate(model, glm::vec3(-offObjects[i].MidPoint.Vertex[0], -offObjects[i].MidPoint.Vertex[1], -offObjects[i].MidPoint.Vertex[2]))* glm::translate(model, glm::vec3(offObjects[i].MidPoint.Vertex[0], offObjects[i].MidPoint.Vertex[1], offObjects[i].MidPoint.Vertex[2]))* glm::mat4_cast(offObjects[i].mesh.RotationMatrix)* glm::translate(model, glm::vec3(-offObjects[i].MidPoint.Vertex[0], -offObjects[i].MidPoint.Vertex[1], -offObjects[i].MidPoint.Vertex[2]));
		//model = glm::translate(model, offObjects[i].mesh.DisplacementVector);
		glm::mat4 mv = view*model;
		glm::mat4 normalMatrix = glm::transpose(glm::inverse(mv));
		
		glm::mat4 mvp = projection * view * model;
		
				glBindBuffer(GL_ARRAY_BUFFER_ARB, VBOid[i][0]);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBOid[i][1]);
			//	glEnableClientState(GL_VERTEX_ARRAY);             
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
				glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)((offObjects[i].fVertexData.size() / 2) * sizeof(glm::vec3)));

				glslProgram[0]->enable();
				//-------------------------Matrices------------------------------//
				glslProgram[0]->loadUniformMatrix("MVP", GShader::UNIFORM_SIZE_4D, glm::value_ptr(mvp));
				glslProgram[0]->loadUniformMatrix("MV", GShader::UNIFORM_SIZE_4D, glm::value_ptr(mv));
				glslProgram[0]->loadUniformMatrix("NM", GShader::UNIFORM_SIZE_4D, glm::value_ptr(normalMatrix));
				//----------------------------Material------------------------//
				glslProgram[0]->loadUniformf("ambientColour", GShader::UNIFORM_SIZE_3D, glm::value_ptr(Ambient));
				glslProgram[0]->loadUniformf("diffColour", GShader::UNIFORM_SIZE_3D, &offObjects[i].color[0]);

				//----------------------------Lights--------------------------//
				glslProgram[0]->loadUniformf("LightPos1", GShader::UNIFORM_SIZE_3D, glm::value_ptr(LightPos1));
				glslProgram[0]->loadUniformf("LightPos2", GShader::UNIFORM_SIZE_3D, glm::value_ptr(LightPos2));
				glslProgram[0]->loadUniformf("LightCol1", GShader::UNIFORM_SIZE_3D, glm::value_ptr(Col1));
				glslProgram[0]->loadUniformf("LightCol2", GShader::UNIFORM_SIZE_3D, glm::value_ptr(Col2));

				glslProgram[0]->loadUniformf("LightColS1", GShader::UNIFORM_SIZE_3D, glm::value_ptr(Col1S));
				glslProgram[0]->loadUniformf("LightColS2", GShader::UNIFORM_SIZE_3D, glm::value_ptr(Col2S));
				glslProgram[0]->loadUniformf("ObjShininess", GShader::UNIFORM_SIZE_1D, &offObjects[i].Shininess);

				glslProgram[0]->loadUniformf("ObjSpecular", GShader::UNIFORM_SIZE_3D, &offObjects[i].SpecularRef[0]);

				glDrawElements(GL_TRIANGLES, offObjects[i].indexData.size(), GL_UNSIGNED_SHORT, 0);
				glslProgram[0]->disable();

				
				if (DrawBBox){
					glBindBuffer(GL_ARRAY_BUFFER_ARB, VBOid[i][2]);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBOid[i][3]);
					glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
					glslProgram[1]->enable();
					glslProgram[1]->loadUniformMatrix("MVP", GShader::UNIFORM_SIZE_4D, glm::value_ptr(mvp));
					glslProgram[1]->loadUniformf("color", GShader::UNIFORM_SIZE_3D, &offObjects[i].bColor[0]);

					glDrawElements(GL_LINES, 24, GL_UNSIGNED_SHORT, 0);
					glslProgram[1]->disable();

				}
				if (DrawNormals){
					glBindBuffer(GL_ARRAY_BUFFER_ARB, VBOid[i][4]);
					glEnableClientState(GL_VERTEX_ARRAY);             // activate vertex coords array
					glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);

					glslProgram[1]->enable();
					glslProgram[1]->loadUniformMatrix("MVP", GShader::UNIFORM_SIZE_4D, glm::value_ptr(mvp));
					glslProgram[1]->loadUniformf("color", GShader::UNIFORM_SIZE_3D,&offObjects[i].nColor[0]);

					//glColor3f(offObjects[i].nColor[0], offObjects[i].nColor[1], offObjects[i].nColor[2]);
					glDrawArrays(GL_LINES, 0, offObjects[i].Normals.size() / 3);
					glslProgram[1]->disable();


				}

				glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
				glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
		
			}
		

		

		frameCount++;
		frameDTime += dt;
		if (frameDTime>1.0)
		{
			fps = frameCount / frameDTime;
			char newTitle[128];
			_snprintf_s(newTitle, sizeof(newTitle), "%s (%.1f fps)", title, fps);
			glfwSetWindowTitle(window,newTitle); // uncomment to display framerate
			frameCount = frameDTime = 0;
		}



		TwDraw();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
//std::exit(EXIT_SUCCESS);
	
	
	std::exit(0);



}