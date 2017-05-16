#include <GUI.h>
#include <iostream>
using namespace std;

GUI* GUI::uniqueGUI = NULL;
GLFWwindow* GUI::gWindow = NULL;
int GUI::gWidth = 1440;
int GUI::gHeight = 900;
float GUI::percent = 0.7f;
float GUI::fFrameInterval = 0.0f;
bool GUI::isFullScreen = false;


GUI::GUI(RenderController* renderController, const float percent) : RenderColleague(renderController) {
	//const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	/*GUI::gWidth = int(mode->width * percent);
	GUI::gHeight = int(mode->height * percent);*/
	GUI::percent = percent;
	if (!initGlfw() || glewInit() != GLEW_OK)
		exit(1);
}


GUI::~GUI() {
	glfwDestroyWindow(GUI::gWindow);
	glfwTerminate();
}

GUI* GUI::Instance(RenderController* renderController, float percent) {
	if (!uniqueGUI)
		uniqueGUI = new GUI(renderController, percent);
	return uniqueGUI;
}

void GUI::Destroy() {
	if (!uniqueGUI) return;
	uniqueGUI->~GUI();
}


float GUI::getHeight() {
	return (float)GUI::gHeight;
}


float GUI::getWidth() {
	return (float)GUI::gWidth;
}

float GUI::getFrameInterval(){
	return fFrameInterval;
}


GLFWwindow * GUI::getWindow() {
	return GUI::gWindow;
}

void GUI::setSize(const float width, const float height) {
	reshape(GUI::gWindow, (int)width, (int)height);
}

void GUI::fullscreen(){
	const GLFWvidmode * vidMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	if (GUI::isFullScreen) {
		//-- Go to normal state
		GUI::gWidth = int(vidMode->width * GUI::percent);
		GUI::gHeight = int(vidMode->height * GUI::percent);
	} else {
		//-- Go to fullscreen mode
		GUI::gWidth = int(vidMode->width);
		GUI::gHeight = int(vidMode->height);
	}
	glfwSetWindowSize(GUI::gWindow, GUI::gWidth, GUI::gHeight);
	GUI* localGUI = GUI::Instance(NULL);
	localGUI->renderController->Send("Reshape", (void*)(new float[2]{ (float)GUI::gWidth, (float)GUI::gHeight }), localGUI);
}

void GUI::ResetTimer(){
	tLastFrame = clock();
	GUI::fFrameInterval = 0.0f;
}

void GUI::UpdateTimer(){
	clock_t tCur = clock();
	GUI::fFrameInterval = float(tCur - tLastFrame) / float(CLOCKS_PER_SEC);
	tLastFrame = tCur;
}

vec2 GUI::getScreenCenter(){
	const GLFWvidmode * vidMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	return vec2((vidMode->width - GUI::gWidth) >> 1, (vidMode->height - GUI::gHeight) >> 1);
}

void GUI::Notify(string message, void* data) {

}


bool GUI::initGlfw() {
	if (!glfwInit())
		return false;

	glfwWindowHint(GLFW_SAMPLES, 4);
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

	GUI::gWindow = glfwCreateWindow(GUI::gWidth, GUI::gHeight, "RayTracing MB", NULL, NULL);

	if (!GUI::gWindow) {
		glfwTerminate();
		return false;
	}

	const GLFWvidmode * vidMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	GUI::gWidth = int(vidMode->width * GUI::percent);
	GUI::gHeight = int(vidMode->height * GUI::percent);
	glfwSetWindowSize(GUI::gWindow, GUI::gWidth, GUI::gHeight);
	glfwSetWindowPos(GUI::gWindow, (vidMode->width - GUI::gWidth) >> 1, (vidMode->height - GUI::gHeight) >> 1);

	glfwMakeContextCurrent(GUI::gWindow);
	glfwSetInputMode(GUI::gWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	//-- GLFW Callbacks
	glfwSetWindowSizeCallback(GUI::gWindow, reshape);
	glfwSetKeyCallback(GUI::gWindow, keyInput);
	glfwSetMouseButtonCallback(GUI::gWindow, mouseButton);
	glfwSetCursorPosCallback(GUI::gWindow, cursorPos);
	glfwSetScrollCallback(GUI::gWindow, scrollCallback);
	glfwSetCharCallback(GUI::gWindow, charCallback);

	return true;
}


void GUI::reshape(GLFWwindow *window, int width, int height) {
	GUI* localGUI = GUI::Instance(NULL);
	GUI::gWidth = width;
	GUI::gHeight = height;
	localGUI->renderController->Send("Reshape", (void*)(new float[2]{ (float)GUI::gWidth, (float)GUI::gHeight }), localGUI);
}


/*Visual Callbacks*/
void GUI::cursorPos(GLFWwindow* window, double x, double y) {
	//-- Prevent default
	if (TwEventMousePosGLFW(int(x), int(y))) return;

	GUI *localUserInterface = GUI::Instance(NULL);
	int x1 = (int)x;
	int y1 = (int)y;
	localUserInterface->renderController->Send("eventMouseMove", (void*)(new int[2]{ x1, y1 }), localUserInterface);
}


void GUI::mouseButton(GLFWwindow* window, int button, int action, int mods) {
	//-- Prevent default
	if (TwEventMouseButtonGLFW(button, action)) return;
}


void GUI::keyInput(GLFWwindow * window, int key, int scancode, int action, int mods) {
	//-- Prevent default
	if (TwEventKeyGLFW(key, action)) return;

	GUI *localUserInterface = GUI::Instance(NULL);
	/*Key pressed an key release*/
	localUserInterface->renderController->Send("eventKeyPressed", (void*)(new int[3]{key , action, mods}), localUserInterface);
}

void GUI::scrollCallback(GLFWwindow * window, double mod, double scroll) {
	//-- Prevent default
	if (TwEventMouseWheelGLFW((int)scroll)) return;

	GUI *localUserInterface = GUI::Instance(NULL);
	int scroll_value = (int)scroll;
	localUserInterface->renderController->Send("eventScroll", (void*)&scroll_value, localUserInterface);
}

void GUI::charCallback(GLFWwindow * window, unsigned int scanChar){
	if (TwEventCharGLFW(scanChar, GLFW_PRESS)) return;
}

//-- Mouse Class
vec2 Mouse::position = vec2();
double Mouse::x = 0.0f;
double Mouse::y = 0.0f;

Mouse::Mouse() {}

Mouse::~Mouse() {}

vec2 Mouse::getPosition() {
	glfwGetCursorPos(GUI::getWindow(), &Mouse::x, &Mouse::y);
	Mouse::position.x = (float)Mouse::x;
	Mouse::position.y = (float)Mouse::y;
	return position;
}
