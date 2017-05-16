#include <MousePicker.h>

MousePicker::MousePicker(Camera* camera, Projection* projection){
	this->camera = camera;
	this->projection = projection;
}

MousePicker::~MousePicker(){

}

vec3 MousePicker::getCurrentMouseRay(){
	return this->currentRay;
}

void MousePicker::calculateRay(){
	vec2 mouseXY = getMousePosition();
	vec2 display = getDisplaySize();
	vec2 normalizedCoords = vec2( ((2.0f*float(mouseXY.x)) / display.x - 1.0f), - ((2.0f*float(mouseXY.y)) / display.y - 1.0f) );
	vec4 clipCoords = vec4(normalizedCoords.x, normalizedCoords.y, -1.0f, 1.0f);
	vec4 eyeCoordsTemp = inverse(this->projection->getMatrix()) * clipCoords;
	vec4 eyeCoords = vec4(eyeCoordsTemp.x, eyeCoordsTemp.y, -1.0f, 0.0f);
	vec4 worldCoords = inverse(this->camera->getMatrix()) * eyeCoords;
	this->currentRay = normalize(vec3(worldCoords.x, worldCoords.y, worldCoords.z));
}

