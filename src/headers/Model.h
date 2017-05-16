#pragma once
#ifndef __MODEL_H_
#define __MODEL_H_

//-- Defines
#define FOR(q,n) for(int q=0;q<n;q++)
#define UFOR(q,n) for(unsigned int q=0;q<n;q++)
#define SFOR(q,s,e) for(int q=s;q<=e;q++)
#define RFOR(q,n) for(int q=n;q>=0;q--)
#define RSFOR(q,s,e) for(int q=s;q>=e;q--)
#define ESZ(elem) (int)elem.size()

//-- Global Includes
#include <string>
#include <sstream>
#include <vector>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing fla

//-- Class Includes
#include <VertexBufferObject.h>
#include <Texture.h>
#include <Transformation.h>
#include <Properties.h>

//-- Namespaces
using std::vector;
using std::string;

class Model {
private:
	bool bLoaded;
	static VertexBufferObject vboModelData;
	static unsigned int uiVAO;
	static vector<Texture> tTextures;
	vector<int> iMeshStartIndices;
	vector<int> iMeshSizes;
	vector<int> iMaterialIndices;
	int iNumMaterials;
	vector<unsigned int> indexes;
	Properties* properties;

public:
	bool LoadModelFromFile(const char* sFilePath);

	static void FinalizeVBO();
	static void BindModelsVAO();

	void RenderModel();
	vector<unsigned int> getIndexes();

	Model(vector<unsigned int> indexes);
};

string GetDirectoryPath(string sFilePath);

#endif
