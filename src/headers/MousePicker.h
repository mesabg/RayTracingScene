#pragma once
#ifndef __MOUSE_PICKER_H_
#define __MOUSE_PICKER_H_

//-- Global Includes
#include <glm\glm\glm.hpp>
#include <glm\glm\gtc\matrix_transform.hpp>
#include <glm\glm\gtc\type_ptr.hpp>

//-- Class Includes
#include <Projection.h>
#include <Camera.h>

//-- Usings
using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat4;
using glm::inverse;
using glm::normalize;

//-- Externs
extern vec2 getMousePosition();
extern vec2 getDisplaySize();

class MousePicker
{
private:
	glm::vec3 currentRay;
	Projection* projection;
	Camera* camera;
public:
	MousePicker(Camera* camera, Projection* projection);
	~MousePicker();

	//-- Getters
	vec3 getCurrentMouseRay();

	//-- Calculating Ray
	void calculateRay();
};

#endif
#pragma once
