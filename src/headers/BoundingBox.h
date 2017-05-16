#pragma once
#ifndef __BOUNDING_BOX_H_
#define __BOUNDING_BOX_H_

//-- Global Includes
#include <glew\glew.h>
#include <vector>
#include <glm\glm\glm.hpp>
#include <glm\glm\gtc\matrix_transform.hpp>
#include <glm\glm\gtc\type_ptr.hpp>

//-- Using Spaces
using std::vector;
using glm::vec3;
using glm::vec4;
using glm::mat4;

class BoundingBox
{
private:
	vec3 center, min, max;
	vector<vec3> *vertices, *normals;
	vector<vec4> *faces;
public:
	BoundingBox();
	BoundingBox(vec3 center);
	BoundingBox(vec3 min, vec3 max);
	~BoundingBox();

	/*Getters*/
	vec3 getCenter();

	/*Setters*/
	void setBorder(vec3 min, vec3 max);
	
	/*Some Functions*/
	void move(mat4 transformation);
	bool checkIntersection(vec3 parametric);
	void recalculate();
	void draw();
};

#endif
