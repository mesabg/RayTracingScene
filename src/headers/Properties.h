#pragma once
#ifndef __LIGHT_PROPERTIES_H__
#define __LIGHT_PROPERTIES_H__

//-- Glogal includes
#include <glm\glm\glm.hpp>

//-- Namespaces
using glm::vec3;

class Properties {
public:
	vec3 diffuse;
	vec3 specular;
	vec3 ambient;
	float shininess;

public:
	Properties(vec3 diffuse, vec3 specular, vec3 ambient, float shininess = 0.0f);
	~Properties();
};

#endif