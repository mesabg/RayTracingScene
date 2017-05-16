#pragma once
#ifndef __CONCRETE_RENDER_CONTROLLER_H_
#define __CONCRETE_RENDER_CONTROLLER_H_

//-- Class Includes
#include <RenderInterface.h>
#include <GUI.h>
#include <Stage.h>

class ConcreteRenderController : public RenderController
{
private:
	//-- All Instances of Colleagues
	Stage* stage;
	GUI* gui;

	//-- Functions
	void render();
public:
	static double lastFrameTime;
	static double delta;
	static double getCurrentTime();
	static double getFrameTimeSeconds();

	ConcreteRenderController(float percent = 0.7f);
	~ConcreteRenderController();

	//-- Override
	void Send(string message, void* data, RenderColleague* colleague);

	//-- Play and Pause
	int finish();
	int infinity();
};

#endif