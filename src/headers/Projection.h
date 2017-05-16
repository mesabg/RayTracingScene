#pragma once
#ifndef __PROJECTION_H__
#define __PROJECTION_H__

//-- Global Includes
#include <glm\glm\glm.hpp>
#include <glm\glm\gtx\transform.hpp>
#include <cstdio>

//-- Class Includes
//#include <CGLSLProgram.h>

using glm::mat4;
using glm::vec3;

class Projection {
private:
	mat4 projection;
	float fieldOfView;	/*Amount of zoom*/
	float aspectRatio;	/*Screen relation (width/height)*/
	float near_;			/*near_ plane*/
	float far_;			/*far_ plane*/

						/*Private Functions*/
	void refresh();
public:
	Projection();
	Projection(float fieldOfView, float aspectRatio, float near_, float far_);
	~Projection();

	/*Getters*/
	mat4 getMatrix();
	mat4 *getProjection();
	float getFieldOfView();
	float getAspectRatio();
	float getNear();
	float getFar();

	/*Setters*/
	void setFieldOfView(float fieldOfView);
	void setAspectRatio(float aspectRatio);
	void setNearAndFar(float near_, float far_);
	void setNear(float near_);
	void setFar(float far_);

	/*Adders*/
	void addFieldOfView(float fieldOfView);
	void addAspectRatio(float aspectRatio);
	void addNearAndFar(float near_, float far_);
	void addNear(float near_);
	void addFar(float far_);
};

#endif