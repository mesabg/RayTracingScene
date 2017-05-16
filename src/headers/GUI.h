#pragma once
#ifndef __USER_INTERFACE_H_
#define __USER_INTERFACE_H_

//-- Global Includes
#include <ctime>
#include <AntTweakBar.h>
#include <glm\glm\glm.hpp>

//-- Class Includes
#include <RenderInterface.h>

//-- Using Spaces
using glm::vec2;

class GUI : public RenderColleague{
private:
	/*Variables*/
	static GUI* uniqueGUI;
	static GLFWwindow* gWindow;
	static int gWidth;
	static int gHeight;
	static float percent;
	static float fFrameInterval;
	static bool isFullScreen;
	clock_t tLastFrame;

	/*Inicializadores*/
	GUI(RenderController* renderController, const float percent);
	~GUI();

	/*Funciones propias*/
	bool initGlfw();
public:


	//-- Get Instance
	static GUI* Instance(RenderController* renderController, float percent = 0.7f);
	static void Destroy();

	//-- @Override
	void Notify(string message, void* data);

	//-- Get functions
	static float getHeight();
	static float getWidth();
	static float getFrameInterval();
	static GLFWwindow* getWindow();

	//-- Set functions
	void setSize(const float width, const float height);

	//-- Other functions
	void fullscreen();
	void ResetTimer();
	void UpdateTimer();
	static vec2 getScreenCenter();

	//-- GLFW Callbacks
	static void reshape(GLFWwindow *window, int width, int height);
	static void cursorPos(GLFWwindow* window, double x, double y);
	static void mouseButton(GLFWwindow* window, int button, int action, int mods);
	static void keyInput(GLFWwindow *window, int key, int scancode, int action, int mods);
	static void scrollCallback(GLFWwindow* window, double x, double y);
	static void charCallback(GLFWwindow* window, unsigned int scanChar);
};


class Mouse {
private:
	static vec2 position;
	static double x, y;

public:
	Mouse();
	~Mouse();

	//-- Getters
	static vec2 getPosition();
};

#endif
