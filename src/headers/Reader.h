#pragma once
#ifndef __READER_H_
#define __READER_H_

// -- Global Includes
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// -- Class Includes
#include <Model.h>

// -- Using Spaces
using std::cout;
using std::endl;



class Reader : public Model{
private:
	//-- Variables


	void processNode(aiNode* node, const aiScene* scene);

	//-- Process Functions
	void processMaterial(aiMaterial *mater);
	void processMesh(aiMesh *mesh);
	void processNode(const aiScene *scene, aiNode *node);
	
	//-- Read Functions
	void readMaterials(const aiScene *scene);
	void readMeshes(const aiScene *scene);
	void readNodes(const aiScene *scene);
public:
	Reader(Routes* routes);
	~Reader();
};

#endif