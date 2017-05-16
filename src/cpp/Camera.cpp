#include <Camera.h>

const float PI = float(atan(1.0)*4.0);

Camera::Camera(){
	vEye = glm::vec3(0.0f, 0.0f, 0.0f);
	vView = glm::vec3(0.0f, 0.0, -1.0f);
	vUp = glm::vec3(0.0f, 1.0f, 0.0f);
	fSpeed = 25.0f;
	fSensitivity = 0.1f;
}

Camera::Camera(glm::vec3 a_vEye, glm::vec3 a_vView, glm::vec3 a_vUp, float a_fSpeed, float a_fSensitivity){
	vEye = a_vEye; vView = a_vView; vUp = a_vUp;
	fSpeed = a_fSpeed;
	fSensitivity = a_fSensitivity;
}


void Camera::RotateWithMouse(){
	GetCursorPos(&pCur);
	int iCentX = (float)getScreenCenter().x,
		iCentY = (float)getScreenCenter().y;

	float deltaX = (float)(iCentX - pCur.x)*fSensitivity;
	float deltaY = (float)(iCentY - pCur.y)*fSensitivity;

	if (deltaX != 0.0f)
	{
		vView -= vEye;
		vView = glm::rotate(vView, deltaX, glm::vec3(0.0f, 1.0f, 0.0f));
		vView += vEye;
	}
	if (deltaY != 0.0f)
	{
		glm::vec3 vAxis = glm::cross(vView - vEye, vUp);
		vAxis = glm::normalize(vAxis);
		float fAngle = deltaY;
		float fNewAngle = fAngle + GetAngleX();
		if (fNewAngle > -89.80f && fNewAngle < 89.80f)
		{
			vView -= vEye;
			vView = glm::rotate(vView, deltaY, vAxis);
			vView += vEye;
		}
	}
	SetCursorPos(iCentX, iCentY);
}


float Camera::GetAngleY(){
	glm::vec3 vDir = vView - vEye; vDir.y = 0.0f;
	glm::normalize(vDir);
	float fAngle = acos(glm::dot(glm::vec3(0, 0, -1), vDir))*(180.0f / PI);
	if (vDir.x < 0)fAngle = 360.0f - fAngle;
	return fAngle;
}


float Camera::GetAngleX(){
	glm::vec3 vDir = vView - vEye;
	vDir = glm::normalize(vDir);
	glm::vec3 vDir2 = vDir; vDir2.y = 0.0f;
	vDir2 = glm::normalize(vDir2);
	float fAngle = acos(glm::dot(vDir2, vDir))*(180.0f / PI);
	if (vDir.y < 0)fAngle *= -1.0f;
	return fAngle;
}


void Camera::SetMovingKeys(int a_iForw, int a_iBack, int a_iLeft, int a_iRight){
	iForw = a_iForw;
	iBack = a_iBack;
	iLeft = a_iLeft;
	iRight = a_iRight;
}

void Camera::keyMove(int key, int action){
	// Get view direction
	glm::vec3 vMove = vView - vEye;
	vMove = glm::normalize(vMove);
	vMove *= fSpeed;

	glm::vec3 vStrafe = glm::cross(vView - vEye, vUp);
	vStrafe = glm::normalize(vStrafe);
	vStrafe *= fSpeed;

	int iMove = 0;
	glm::vec3 vMoveBy;

	// Get vector of move
	if (key == GLFW_KEY_W && (action == GLFW_REPEAT || action == GLFW_PRESS)) vMoveBy += vMove * (0.1f * getFrameInterval());
	if (key == GLFW_KEY_S && (action == GLFW_REPEAT || action == GLFW_PRESS)) vMoveBy -= vMove * (0.1f * getFrameInterval());
	if (key == GLFW_KEY_A && (action == GLFW_REPEAT || action == GLFW_PRESS)) vMoveBy -= vStrafe * (0.1f * getFrameInterval());
	if (key == GLFW_KEY_D && (action == GLFW_REPEAT || action == GLFW_PRESS)) vMoveBy += vStrafe * (0.1f * getFrameInterval());
	vEye += vMoveBy; vView += vMoveBy;

	//printf("Eye (%f,%f,%f)\nView(%f,%f,%F)\n", vEye.x, vEye.y, vEye.z, vView.x, vView.y, vView.z);
}


void Camera::Update(){
	this->RotateWithMouse();
}


void Camera::ResetMouse(){
	/*RECT rRect; GetWindowRect(appMain.hWnd, &rRect);
	int iCentX = (rRect.left + rRect.right) >> 1,
		iCentY = (rRect.top + rRect.bottom) >> 1;
	SetCursorPos(iCentX, iCentY);*/
}


glm::mat4 Camera::Look(){
	return glm::lookAt(vEye, vView, vUp);
}