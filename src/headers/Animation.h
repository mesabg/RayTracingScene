#pragma once
#ifndef __ANIMATION_H_
#define __ANIMATION_H_

//-- Global includes
#include <vector>

//-- Class Includes
#include <Transformation.h>

//-- Using Spaces
using std::vector;
using glm::vec3;
using glm::vec4;
using glm::mat4;

//-- Externs
extern float getFrameTimeSeconds();

class State {
public:
	vec3 nextPosition;
	vec3 direction;
	bool done;
	int iterations;

	State();
	State(vec3 nextPosition, vec3 direction, bool done, int iterations);
	~State();
};



class Animation
{
private:
	Transformation* transformation;
	const float RUN_SPEED = 20.0f;
	bool isOn, reverse;
	int index, iterations;
	vector<State*> *state;

public:
	Animation(Transformation* transformation);
	Animation(Transformation* transformation, vector<State*>* state);
	~Animation();

	//-- Setters
	void setOn(bool isOn);
	void setOnRet();

	//-- Some Functions
	void animate();
	bool allDone();
	bool allNoDone();
};

#endif
