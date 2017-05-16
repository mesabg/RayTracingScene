#pragma once
#ifndef __RENDER_FACADE_H__
#define __RENDER_FACADE_H__

//-- Global Includes
#include <cassert>
#include <string>
#include <vector>
#include <map>

//-- Class Includes
#include <Reader.h>
#include <FrameBuffer.h>

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

class RenderFacade {
private:
	map<string, CGLSLProgram*> *shaders;
	FrameBuffer* frameBufferObject;

	vector<Model*> model_entities;
	vector<Model*> light_entities;

	Projection* projection;
	Camera* camera;

	//-- Private functions
	void checkAssertions();
public:
	RenderFacade();
	~RenderFacade();

	//-- Initialization Methods
	void initModelEntities(
		const vector<Routes*> routes,
		const vector<Transformation*> transformations,
		const vector<Light*> lights,
		const vector<MaterialProperties*> materialProperties);

	void initLightEntities(
		const vector<Routes*> routes,
		const vector<Transformation*> transformations,
		const vector<Light*> lights,
		const vector<MaterialProperties*> materialProperties);

	void bindProjection(Projection* projection);

	void bindCamera(Camera* camera);

	void bindShaderPrograms(map<string, CGLSLProgram*> *shaders);

	void bindFBO(FrameBuffer* FBO);

	//-- Render Methods
	void render(string shaderName);
	void render(Projection* projection, Camera* camera, string shaderName);
	void low_quality_render(Model* model);
	void low_quality_render(Projection* projection, Camera* camera, const int index);
	void render_shadow_map();
	
	//-- Calculations Methods
	void FBOrender_shadow_map();
	void FBOrender_color_cube_map(Model* model, const int index);
	void buildCubeMap();
	int isClicked(vec3 ray);

	//-- Getters
	Model* getModel(const int index);
	Model* getLightModel(const int index);

	//-- Init Vertex Buffers
	void initVBOs();
};

#endif
