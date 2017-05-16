#include <ConcreteRenderController.h>

double ConcreteRenderController::lastFrameTime = ConcreteRenderController::getCurrentTime();
double ConcreteRenderController::delta = 0.0f;

ConcreteRenderController::ConcreteRenderController(float percent) {
	//-- Initializing complete GUI 
	this->gui = GUI::Instance(this, percent);

	//-- Initializing Stage
	this->stage = Stage::Instance(this, this->gui->getWidth(), this->gui->getHeight());
	this->stage->Notify("width/height", (void*)(new float[2]{ this->gui->getWidth(), this->gui->getHeight() }));
	//this->stage->Notify("init VBOs", NULL);

	//-- Reshape screen
	//this->gui->reshape(this->gui->getWindow(), (int)width, (int)height);
	this->gui->ResetTimer();
}

ConcreteRenderController::~ConcreteRenderController() {

}

void ConcreteRenderController::Send(string message, void* data, RenderColleague* renderColleague) {
	if (message == "Reshape" && renderColleague == this->gui) 
		this->stage->getProjection()->setAspectRatio(((float*)data)[0] / ((float*)data)[1]);

	//-- Event processing
	if (message == "eventKeyPressed" && renderColleague == this->gui) {
		int key = ((int*)data)[0];
		int action = ((int*)data)[1];
		int mod = ((int*)data)[2];

		//-- Default Actions
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) /*Program ends here*/ exit(this->finish());
		if (key == GLFW_KEY_F && action == GLFW_PRESS && mod == GLFW_MOD_CONTROL) {
			this->gui->fullscreen();
			return;
		}

		//-- Stage actions
		this->stage->Notify(message, data);
	}

	if (message == "eventScroll" && renderColleague == this->gui)
		this->stage->Notify(message, data);

	if (message == "eventMouseMove" && renderColleague == this->gui)
		this->stage->Notify(message, data);
}

void ConcreteRenderController::render() {
	glClearColor(0.2f, 0.2f, 0.2f, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//-- Render the stage
	this->stage->render();
}

double ConcreteRenderController::getCurrentTime(){
	return glfwGetTime()*1000/*/glfwGetTimerValue()*/;
}

double ConcreteRenderController::getFrameTimeSeconds(){
	return delta;
}

int ConcreteRenderController::infinity() {
	while (!glfwWindowShouldClose(this->gui->getWindow())) {
		//-- GUI Buffers Update
		glfwMakeContextCurrent(this->gui->getWindow());
		glfwPollEvents();
		this->render();
		glfwSwapBuffers(this->gui->getWindow());

		//-- Updating current time
		double currentFrameTime = getCurrentTime();
		delta = (currentFrameTime - lastFrameTime) / 1000.0f;
		lastFrameTime = currentFrameTime;
		this->gui->UpdateTimer();
	}
	return EXIT_SUCCESS;
}

int ConcreteRenderController::finish() {
	this->gui->Destroy();
	this->stage->Destroy();
	return EXIT_SUCCESS;
}