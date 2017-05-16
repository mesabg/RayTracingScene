#pragma once
#ifndef __LIGHT_H__
#define __LIGHT_H__

//-- Global Includes
#include <glm\glm\glm.hpp>
#include <string>

using glm::vec3;
using glm::tvec3;
using std::string;

class Light {
public:
	glm::vec3 vColor; // Color of directional light
	glm::vec3 vDirection; // and its direction

	float fAmbient;

	Light();
	Light(glm::vec3 a_vColor, glm::vec3 a_vDirection, float a_fAmbient);
};

#endif