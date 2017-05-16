#pragma once
#ifndef __CAMERA_H__
#define __CAMERA_H__

//-- Global includes
#include <windows.h>
#include <glm\glm\glm.hpp>
#include <glm\glm\gtc\matrix_transform.hpp>
#include <glm\glm\gtc\type_ptr.hpp>
#include <glm\glm\gtx\rotate_vector.hpp>

//-- Class Includes
#include <RenderInterface.h>

//-- Namespaces
using glm::mat4;
using glm::vec3;

//-- Externs
extern float getFrameInterval();
extern vec2 getScreenCenter();

class Camera{
public:
	glm::vec3 vEye, vView, vUp;
	float fSpeed;
	float fSensitivity; // How many degrees to rotate per pixel moved by mouse (nice value is 0.10)
						// Main functions
	void RotateWithMouse();
	void Update();
	glm::mat4 Look();

	void SetMovingKeys(int a_iForw, int a_iBack, int a_iLeft, int a_iRight);
	void keyMove(int key, int action);
	void ResetMouse();

	// Functions that get viewing angles
	float GetAngleX(), GetAngleY();

	// Constructors
	Camera();
	Camera(glm::vec3 a_vEye, glm::vec3 a_vView, glm::vec3 a_vUp, float a_fSpeed, float a_fSensitivity);

private:
	POINT pCur; // For mosue rotation
	int iForw, iBack, iLeft, iRight;
};

extern Camera cCamera;

#endif