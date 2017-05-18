#pragma once
#ifndef __STAGE_H__
#define __STAGE_H__

#include <Camera.h>
#include <Light.h>
#include <Routes.h>
#include <SkyBox.h>
#include <Projection.h>
#include <ShaderProgram.h>
#include <Model.h>
#include <Properties.h>
#include <CGLSLProgram.h>
#include <glm/glm/gtc/type_ptr.hpp>

extern unsigned int amountOfEntities;
extern unsigned int amountOfLights;
extern vec2 getDisplaySize();
extern float getFrameTimeSeconds();
extern float getCurrentTime();

class Stage : public RenderColleague
{
private:
	//-- Singleton
	static Stage* uniqueStage;

	//-- Local Variables
	Skybox skybox;
	Light dlSun;
	Camera* camera;
	Projection* projection;
	vector<Model*> entities;
	vector<Transformation*> transformations;
	vector<Properties*> properties;
	map <string, CGLSLProgram*> *illuminationPrograms;

	//-- Button Click variables
	bool clicked;
	int xPos, yPos;
	int xPosFirst, yPosFirst;
	bool ImALight;

	void buildModels();
	Stage(RenderController* renderController, const float width, const float height);
	~Stage();	
public:

	//-- Instance
	static Stage* Instance(RenderController* renderController, float width = 1440.0f, float height = 900.0f);
	static void Destroy();

	//-- Getters
	Projection* getProjection();

	//-- Init Shaders
	void initGLSLPrograms();

	//-- Get message
	void Notify(string message, void* data);

	//-- Functions
	void render();
};

#endif