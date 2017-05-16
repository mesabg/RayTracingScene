#include <Main.h>

int main() {
	ConcreteRenderController* renderController = new ConcreteRenderController(0.8f);
	renderController->infinity();
	renderController->finish();
}

float getFrameTimeSeconds(){
	return (float)ConcreteRenderController::getFrameTimeSeconds();
}

float getCurrentTime() {
	return (float)ConcreteRenderController::getCurrentTime()/1000.0;
}

vec2 getMousePosition(){
	return Mouse::getPosition();
}

vec2 getDisplaySize(){
	return vec2(GUI::getWidth(), GUI::getHeight());
}

float getFrameInterval(){
	return GUI::getFrameInterval();
}

vec2 getScreenCenter(){
	return GUI::getScreenCenter();
}
