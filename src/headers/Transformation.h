#pragma once
#ifndef __TRANSFORMATION_H__
#define __TRANSFORMATION_H__

#include <glm\glm\glm.hpp>
#include <glm\glm\gtc\quaternion.hpp>
#include <glm\glm\gtx\quaternion.hpp>
#include <glm\glm\gtx\transform.hpp>

using glm::mat4;
using glm::vec3;
using glm::vec4;
using glm::quat;
using glm::mat4_cast;

class Transformation {
private:
	mat4 TInverse, T, S, R;
	vec3 t, r, center;
	quat quaternion;
	float scale, angle, rotY;

public:
	Transformation();
	Transformation(float tx, float ty, float tz, float scale, float angle, float rx, float ry, float rz);
	Transformation(float scale, vec3 position, float angle, vec3 rotationDir);
	~Transformation();

	/*getters*/
	vec3* getPosition();
	mat4 getTransformMatrix();
	mat4 getTraslationInverseMatrix();
	mat4 getTraslationMatrix();
	mat4 getScaleMatrix();
	quat* getQuaternion();
	float* getScaleValue();
	vec3* getTraslationValues();
	float* getTraslationValueX();
	float* getTraslationValueY();
	float* getTraslationValueZ();
	mat4 getRotationMatrix();
	float getRotationY();

	/*Setters*/
	void setTraslationMatrix(float tx, float ty, float tz);
	void setTraslationMatrix(vec3 tr);
	void setScaleMatrix(float scale);
	void setRotationMatrix(float angle, float rx, float ry, float rz);
	void setRotationMatrix(float angle, vec3 tr);


	/*Adders*/
	void increaseTraslationMatrix(float tx, float ty, float tz);
	void addScaleMatrix(float scale);
	void increaseRotationMatrix(float angle);

	//-- Update/Recalculate
	void updateTraslationMatrix();
	void updateRotationMatrix();
	void updateRotationMatrixQuaternion();
	void updateScaleMatrix();

};

#endif