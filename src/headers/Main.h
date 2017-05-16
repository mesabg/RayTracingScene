#pragma once
#ifndef __MAIN_H_
#define __MAIN_H_
#include <ConcreteRenderController.h>

//-- Global Variables
unsigned int amountOfEntities = 2;
unsigned int amountOfLights = 0;
unsigned int Gwidth = 1440;
unsigned int Gheight = 900;

//-- Global Functions
float getFrameTimeSeconds();
float getCurrentTime();
vec2 getMousePosition();
vec2 getDisplaySize();
float getFrameInterval();
vec2 getScreenCenter();

#endif