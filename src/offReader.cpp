#include "offReader.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm> 


#define WHITESPACE " \t\n\r\s "


char contains(const char *haystack, const char *needle)
{
	if (strstr(haystack, needle) == NULL)
		return 0;
	return 1;
}


offReader::offReader(std::string fPath)
{

	std::string line;
	std::ifstream objF;
	std::vector<glm::vec3> AuxfVertexData;
	std::string extension = fPath.substr(fPath.length() - 3, fPath.length() - 1);

	
	float x, y,z,w;
	int ind1, ind2, ind3,cosainservible;
	char slash;


	
	int numVertices, numFaces, numEdges, facexVerts, aux;
	objF.open(fPath);
	if (!objF) std::cout << "Error al abrir el archivo, causa probable: nombre erroneo" << std::endl;

	if (extension=="off") {
		objF >> line;
		objF >> numVertices >> numFaces >> numEdges;
		

		//std::cout << numVertices << numFaces << numEdges;


		objF >> x >> y >> z;
		minX = x;
		maxX = x;
		minY = y;
		maxY = y;
		minZ = z;
		maxZ = z;
		vertexData.push_back(Vertices(x, y, z));
		fVertexData.push_back(glm::vec3(x, y, z));

		

		for (int i = 0; i < numVertices - 1; i++){
			objF >> x >> y >> z;
			if (x < minX) minX = x;
			if (x > maxX) maxX = x;
			if (y < minY) minY = y;
			if (y > maxY) maxY = y;
			if (z < minZ) minZ = z;
			if (z > maxZ) maxZ = z;
			vertexData.push_back(Vertices(x, y, z));
			fVertexData.push_back(glm::vec3(x, y, z));


		}
		for (int i = 0; i < numVertices; i++) fVertexData.push_back(glm::vec3(0));

		for (int z = 0; z < numFaces; z++){
			objF >> facexVerts;
			Face fs;
			std::vector <int> faceVerts;

			for (int inner = 0; inner < facexVerts; inner++) {
				objF >> aux;
				faceVerts.push_back(aux);
			}

			if (facexVerts > 3){
				//Triangularizar
				for (int indI = 1; indI < facexVerts - 1; indI++)
				{
					fs.Faces.push_back(Triangle(vertexData[faceVerts[0]], vertexData[faceVerts[indI]], vertexData[faceVerts[indI + 1]]));
					indexData.push_back((GLshort)faceVerts[0]);
					indexData.push_back((GLshort)faceVerts[indI]);
					indexData.push_back((GLshort)faceVerts[indI + 1]);
					glm::vec3 VertexNormal = glm::vec3(fs.Faces.at(fs.Faces.size() - 1).tNormal[0], fs.Faces.at(fs.Faces.size() - 1).tNormal[1], fs.Faces.at(fs.Faces.size() - 1).tNormal[2]);
					fVertexData.at(faceVerts[0]*2 + 1) = glm::normalize(VertexNormal + fVertexData.at(faceVerts[0]*2 + 1));
					fVertexData.at(faceVerts[1]*2 + 1) = glm::normalize(VertexNormal + fVertexData.at(faceVerts[1]*2 + 1));
					fVertexData.at(faceVerts[2]*2 + 1) = glm::normalize(VertexNormal + fVertexData.at(faceVerts[2]*2 + 1));

				}
			}
			else{
				fs.Faces.push_back(Triangle(vertexData[faceVerts[0]], vertexData[faceVerts[1]], vertexData[faceVerts[2]]));
				indexData.push_back((GLshort)faceVerts[0]);
				indexData.push_back((GLshort)faceVerts[1]);
				indexData.push_back((GLshort)faceVerts[2]);
				glm::vec3 VertexNormal = glm::vec3(fs.Faces.at(fs.Faces.size() - 1).tNormal[0], fs.Faces.at(fs.Faces.size() - 1).tNormal[1], fs.Faces.at(fs.Faces.size() - 1).tNormal[2]);
				fVertexData[numVertices  + faceVerts[0]] = glm::normalize(VertexNormal + fVertexData[numVertices  + faceVerts[0]]);
				fVertexData[numVertices  + faceVerts[1]] = glm::normalize(VertexNormal + fVertexData[numVertices  + faceVerts[1]]);
				fVertexData[numVertices  + faceVerts[2]] = glm::normalize(VertexNormal + fVertexData[numVertices  + faceVerts[2]]);
				
			}


			mesh.Faces.push_back(fs);

		}
		//--BBox Verticies--//
		
		/*
		BBox.push_back(Vertices(minX, maxY, minZ)); //v0
		BBox.push_back(Vertices(maxX, maxY, minZ)); //v1
		BBox.push_back(Vertices(maxX, minY, minZ)); //v2
		BBox.push_back(Vertices(minX, minY, minZ)); //v3
		BBox.push_back(Vertices(minX, maxY, maxZ)); //v4
		BBox.push_back(Vertices(maxX, maxY, maxZ)); //v5
		BBox.push_back(Vertices(maxX, minY, maxZ)); //v6
		BBox.push_back(Vertices(minX, minY, maxZ)); //v7

		for (int faces = 0; faces < mesh.Faces.size(); faces++)
		{
			for (int normal = 0; normal < mesh.Faces[faces].Faces.size(); normal++){
				Normals.push_back(mesh.Faces[faces].Faces[normal].tNormalVector[0]);
				Normals.push_back(mesh.Faces[faces].Faces[normal].tNormalVector[1]);
				Normals.push_back(mesh.Faces[faces].Faces[normal].tNormalVector[2]);

				float a = mesh.Faces[faces].Faces[normal].tNormalVector[0] + mesh.Faces[faces].Faces[normal].tNormal[0] * 0.05f;
				float b = mesh.Faces[faces].Faces[normal].tNormalVector[1] + mesh.Faces[faces].Faces[normal].tNormal[1] * 0.05f;
				float c = mesh.Faces[faces].Faces[normal].tNormalVector[2] + mesh.Faces[faces].Faces[normal].tNormal[2] * 0.05f;
				Normals.push_back(a);
				Normals.push_back(b);
				Normals.push_back(c);
				}
				*/
			
		}
	
	if(extension=="obj")//Entendiendo que es un .OBJ
	{
		Face fs;

		minX = 99999999999;
		maxX = -99999999999;
		minY = 99999999999;
		maxY = -99999999999;
		minZ = 99999999999;
		maxZ = -99999999999;

		/**/
		bool doneonce = false;

		int NumVertices = 0;
		while (!objF.eof()){ //should for simplicity's sake, do this twice.
			std::getline(objF, line);
			if (line.substr(0, 2) == "vt") continue;
			if (line.substr(0, 2) == "vn") continue;

			if (line[0] == 'v' && line[0] != 'n' && line[0] != 't'){
				std::stringstream stream(line.substr(1, line.length()));
				stream >> x >> y >> z ;

				if (x < minX) minX = x;
				if (x > maxX) maxX = x;
				if (y < minY) minY = y;
				if (y > maxY) maxY = y;
				if (z < minZ) minZ = z;
				if (z > maxZ) maxZ = z;

				vertexData.push_back(Vertices(x, y, z));
				fVertexData.push_back(glm::vec3(x, y, z));
				NumVertices++;


			}
		
		}
		objF.clear();
		objF.seekg(0, objF.beg);
		int it = 0;
		for (int i = 0; i < NumVertices; i++) fVertexData.push_back(glm::vec3(0));

		while (!objF.eof()){ //should for simplicity's sake, do this twice.
			std::getline(objF, line);
			if (line[0] == 'f'){
				std::vector <int> faceVerts;
				//std::cout << line[0] << " different line " << it++ << std::endl;
				it++;
			
				char *dup = _strdup(line.substr(1,line.length()-1).c_str());
				char* token;
				char *next_token1 = NULL;

				token = strtok_s(dup, WHITESPACE, &next_token1);
				while ((token   != NULL))
				{
					if (atoi(token)==0) std::cout<< "Surprise";

					faceVerts.push_back(atoi(token));
					token = strtok_s(NULL, WHITESPACE, &next_token1);
					 

				}
				//std::cout << faceVerts.size();
				/*
				for (int z = 0; z < faceVerts.size(); z++) std::cout << faceVerts[z] << " ";
				std::cout << " " << std::endl;*/
				if (faceVerts.size() > 3){//Triangularizar
					for (int indI = 1; indI < faceVerts.size() - 1; indI++)
					{
						fs.Faces.push_back(Triangle(vertexData[faceVerts[0]-1], vertexData[faceVerts[indI]-1], vertexData[faceVerts[indI + 1]-1]));
						indexData.push_back((GLshort)faceVerts[0]-1);
						indexData.push_back((GLshort)faceVerts[indI]-1);
						indexData.push_back((GLshort)faceVerts[indI + 1]-1);
						glm::vec3 VertexNormal = glm::vec3(fs.Faces.at(fs.Faces.size() - 1).tNormal[0], fs.Faces.at(fs.Faces.size() - 1).tNormal[1], fs.Faces.at(fs.Faces.size() - 1).tNormal[2]);
						fVertexData[NumVertices + faceVerts[0]-1] = glm::normalize(VertexNormal + fVertexData[NumVertices + faceVerts[0]-1]);
						fVertexData[NumVertices + faceVerts[indI]-1] = glm::normalize(VertexNormal + fVertexData[NumVertices + faceVerts[indI]-1]);
						fVertexData[NumVertices + faceVerts[indI+1]-1] = glm::normalize(VertexNormal + fVertexData[NumVertices + faceVerts[indI + 1]-1]);

					
					
					}

				
				}
				else{
					fs.Faces.push_back(Triangle(vertexData[faceVerts[0]-1], vertexData[faceVerts[1]-1], vertexData[faceVerts[2]-1]));
					indexData.push_back((GLshort)faceVerts[0]-1);
					indexData.push_back((GLshort)faceVerts[1]-1);
					indexData.push_back((GLshort)faceVerts[2]-1);
					glm::vec3 VertexNormal = glm::vec3(fs.Faces.at(fs.Faces.size() - 1).tNormal[0], fs.Faces.at(fs.Faces.size() - 1).tNormal[1], fs.Faces.at(fs.Faces.size() - 1).tNormal[2]);
					fVertexData[NumVertices + faceVerts[0]-1] = glm::normalize(VertexNormal + fVertexData[NumVertices + faceVerts[0]-1]);
					fVertexData[NumVertices + faceVerts[1]-1] = glm::normalize(VertexNormal + fVertexData[NumVertices + faceVerts[1]-1]);
					fVertexData[NumVertices + faceVerts[2]-1] = glm::normalize(VertexNormal + fVertexData[NumVertices + faceVerts[2]-1]);
					
				
				}
				
				
				
				free(dup);
				faceVerts.clear();
			}







			

			
		}
		std::cout << "Fin Archivo" <<  
			it<<" "<< std::endl;
		mesh.Faces.push_back(fs);
		}
		
//		mesh.Faces.push_back(fs);
		/*
		for (int i = 0; i < indexData.size(); i += 3){

			glm::vec3 VertexNormal = glm::vec3(fs.Faces.at(fs.Faces.size() - 1).tNormal[0], fs.Faces.at(fs.Faces.size() - 1).tNormal[1], fs.Faces.at(fs.Faces.size() - 1).tNormal[2]);
			fVertexData[NumVertices + ind1 - 1] = glm::normalize(VertexNormal + fVertexData[NumVertices + ind1 - 1]);
			fVertexData[NumVertices + ind2 - 1] = glm::normalize(VertexNormal + fVertexData[NumVertices + ind2 - 1]);
			fVertexData[NumVertices + ind3 - 1] = glm::normalize(VertexNormal + fVertexData[NumVertices + ind3 - 1]);
		}
	*/




	//--BBox Verticies--//
	BBox.push_back(Vertices(minX, maxY, minZ)); //v0
	BBox.push_back(Vertices(maxX, maxY, minZ)); //v1
	BBox.push_back(Vertices(maxX, minY, minZ)); //v2
	BBox.push_back(Vertices(minX, minY, minZ)); //v3
	BBox.push_back(Vertices(minX, maxY, maxZ)); //v4
	BBox.push_back(Vertices(maxX, maxY, maxZ)); //v5
	BBox.push_back(Vertices(maxX, minY, maxZ)); //v6
	BBox.push_back(Vertices(minX, minY, maxZ)); //v7

	for (int faces = 0; faces < mesh.Faces.size(); faces++)
	{
		for (int normal = 0; normal < mesh.Faces[faces].Faces.size(); normal++){
			Normals.push_back(mesh.Faces[faces].Faces[normal].tNormalVector[0]);
			Normals.push_back(mesh.Faces[faces].Faces[normal].tNormalVector[1]);
			Normals.push_back(mesh.Faces[faces].Faces[normal].tNormalVector[2]);

			float a = mesh.Faces[faces].Faces[normal].tNormalVector[0] + mesh.Faces[faces].Faces[normal].tNormal[0] * 0.05f;
			float b = mesh.Faces[faces].Faces[normal].tNormalVector[1] + mesh.Faces[faces].Faces[normal].tNormal[1] * 0.05f;
			float c = mesh.Faces[faces].Faces[normal].tNormalVector[2] + mesh.Faces[faces].Faces[normal].tNormal[2] * 0.05f;
			Normals.push_back(a);
			Normals.push_back(b);
			Normals.push_back(c);

		}
	}
	
	
	float GreaterX = maxX - minX;
	float GreaterY = maxY - minY;
	float GreaterZ = maxZ - minZ;
	
	Largest=1/(float)std::max(std::max(GreaterX, GreaterY), GreaterZ);
	
	std::cout << GreaterX << " " << GreaterY << " " << GreaterZ <<" " <<Largest <<std::endl;
	
	
	MidPoint = Vertices((minX +maxX  )*0.5f, (minY+ maxY )*0.5f, (minZ +maxZ )*0.5f);

	if (extension == "obj"){
		//MidPoint = Vertices(minX + (maxX - minX) *0.5f, minY + (maxY - minY)*0.5f, minZ + (maxZ - minZ)*0.5f);
		Format = "obj";
	}
	else{
		//MidPoint = Vertices((maxX - minX) *0.5f, (maxY - minY)*0.5f,  (maxZ - minZ)*0.5f);
		Format = "off";
	}
	std::cout << " midpoint :" << MidPoint.Vertex[0] << " "  <<MidPoint.Vertex[1]<<" " << MidPoint.Vertex[2] << std::endl;
	 color[0] =  0.5f;
	 color[1] = 0.5f;
	 color[2] =  0.5f;
	 nColor[0] =  0.5f;
	 nColor[1] =  0.5f;
	 nColor[2] =  0.5f;
	 bColor[0] = 0.5f ;
	 bColor[1] = 0.5f ;
	 bColor[2] = 0.5f ;
	 Scaling[0] = 1.f;
	 Scaling[1] = 1.f;
	 Scaling[2] = 1.f;
	
	 
	 




	
	objF.close();
}


offReader::~offReader()
{
}
