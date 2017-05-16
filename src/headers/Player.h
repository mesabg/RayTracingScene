#pragma once
#ifndef __PLAYER_H__
#define __PLAYER_H__

//-- Global Includes
#include <glew\glew.h>
#include <glfw3.h>
#include <glm\glm\glm.hpp>
#include <glm\glm\gtc\matrix_transform.hpp>
#include <glm\glm\gtc\type_ptr.hpp>

//-- Class Includes
#include <Model.h>

//-- Using Spaces
using std::string;
using std::vector;
using glm::vec3;

//-- Externs
extern float getFrameTimeSeconds();

class Player {
private:
	const float RUN_SPEED = 20.0f;
	const float TURN_SPEED = 160.0f;
	float currentSpeed;
	float currentTurnSpeed;
	Model* model;
	Transformation* transformation;

public:
	Player();
	Player(Model* model);
	~Player();

	void keyboardInputs(int key, int action);
	void move();
	void setModel(Model* model);

	float getRotationY();
	glm::vec3 getPosition();
};


#endif