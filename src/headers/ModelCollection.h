#pragma once
#ifndef __MODEL_COLLECTION_H__
#define __MODEL_COLLECTION_H__

//-- Global Includes
#include <string>
#include <vector>
#include <map>

//-- Class Includes
#include <Reader.h>

//-- Using Spaces
using std::string;
using std::vector;
using std::map;
using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat4;

//-- Externs
extern unsigned int amountOfEntities;
extern unsigned int amountOfLights;

class ModelCollection{
protected:
	vector<Model*> entities;
	vector<Light*> lightSet;
	vector<bool> reflectionMap;
public:
	ModelCollection(
		vector<Routes*> routes, 
		vector<Transformation*> tranformations, 
		vector<Light*> lights, 
		vector<MaterialProperties*> materialProperties);

	~ModelCollection();

	//-- Methods
	void render(Projection* projection, Camera* camera, vector<Light*>* lights, CGLSLProgram* shader);
	void render(Projection* projection, Camera* camera, vector<Light*>* lights, map<string, CGLSLProgram*> *shaders, vector<string> position);
	void render(Projection* projection, Camera* camera, vector<Light*>* lights, map<string, CGLSLProgram*> *shaders, vector<string> position, GLuint cubeTex);
	void low_render(glm::mat4 depthMVP, CGLSLProgram* shader);
	void initVBOs();
	void setSkyBox(SkyBox* skybox);
	void setReflectionMap(vector<bool> reflectionMap);
	void setShadow(glm::mat4 depthBiasMVP, GLuint ShadowMapId);

	//-- Getter
	Model* getEntity(const unsigned int index);
	vector<Light*> *getLightSet();
};

#endif
