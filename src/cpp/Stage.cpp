#include <Stage.h>
#include <iostream>

using namespace std;

Stage* Stage::uniqueStage = NULL;

void Stage::buildModels(){
	//-- Declare models
	vector<Routes*> routes(amountOfEntities, NULL);
	vector< vector<unsigned int> > indexes;

	//-- Models
	routes[0] = (new Routes("../models/CrytekSponza/sponza.obj"));
	routes[1] = (new Routes("../models/futbol/futbol.obj"));

	//-- Transformations
	//-- Scale, Position, Angle, Rotation Direction

	//-- For Sponza
	transformations.push_back(new Transformation( 0.3f, glm::vec3(0.0f, 0.0f, 0.0f), 0.0f, glm::vec3(0.0f, 1.0f, 0.0f)));

	//-- For Balls
	transformations.push_back(new Transformation(0.5f, glm::vec3(-100.0f, 120.0f, 0.0f), 0.0f, glm::vec3(0.0f, 1.0f, 0.0f)));
	transformations.push_back(new Transformation(0.5f, glm::vec3(0.0f, 100.0f, 0.0f), 0.0f, glm::vec3(0.0f, 1.0f, 0.0f)));
	transformations.push_back(new Transformation(0.5f, glm::vec3(100.0f, 80.0f, 0.0f), 0.0f, glm::vec3(0.0f, 1.0f, 0.0f)));

	//-- Properties


	//-- Indexes
	//-- For Sponza
	indexes.push_back({0});

	//-- For Balls
	indexes.push_back({1, 2, 3});

	//-- Constructing models
	for (unsigned int i = 0; i < 1; i++) {
		this->entities.push_back(new Model(indexes[i]));
		this->entities.back()->LoadModelFromFile(routes[i]->model.c_str());
	}
}

Stage::Stage(RenderController* renderController, const float width, const float height):RenderColleague(renderController) {
	//-- Init local structures
	this->projection = new Projection(45.0f, getDisplaySize().x / getDisplaySize().y, 1.0f, 2000.0f);

	//-- Init Shaders 
	this->initGLSLPrograms();
	if (!PrepareShaderPrograms()){
		printf("Shaders Error\n");
		exit(0);
		return;
	}

	//-- Clear Screen
	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0);

	//-- Init Camera
	this->camera = new Camera(glm::vec3(319.0f, 60.0f, -10.0f), glm::vec3(305.0f, 60.0f, -13.0f), glm::vec3(0.0f, 1.0f, 0.0f), 800.0f, 0.001f);
	//this->camera = new Camera(vec3(319.0f, 60.0f, -10.0f), vec3(305.0f, 60.0f, -13.0f), vec3(0.0f, 1.0f, 0.0f), 20.0f, 0.001f);
	this->camera->SetMovingKeys('W', 'S', 'A', 'D');

	//-- Init skybox
	this->skybox.LoadSkybox("../skyboxes/elbrus/", "elbrus_front.jpg", "elbrus_back.jpg", "elbrus_right.jpg", "elbrus_left.jpg", "elbrus_top.jpg", "elbrus_top.jpg");
	
	//-- Init sun lightning
	dlSun = Light(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(sqrt(2.0f) / 2, -sqrt(2.0f) / 2, 0), 0.5f);
	
	//-- Init Models
	this->buildModels();
	
	//-- Finalize VBO Structure
	Model::FinalizeVBO();
}

Stage::~Stage() {

}

void Stage::initGLSLPrograms(){
	//-- Graphic Card Info
	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
	std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;

	//-- Structure to initialize
	this->illuminationPrograms = new map<string, CGLSLProgram*>();
	vector< map<string, string>* > *routes = new vector< map<string, string>* >({
		new map<string, string>({ { "name", "RayTracing" },{ "vertex", "../src/shaders/raytracing.vert" }, { "geometry", "" }, { "fragment", "../src/shaders/raytracing_att2.frag" } })
	});

	//-- Initialize Shader Programs
	std::vector< map<string, string>* >::iterator programRoute;
	for (programRoute = routes->begin(); programRoute != routes->end(); programRoute++){
		std::cout << "\nLoading " << (*programRoute)->at("name") << " shading program...\n";

		this->illuminationPrograms->insert_or_assign(
			(*programRoute)->at("name"),
			new CGLSLProgram()
		);


		this->illuminationPrograms->at( (*programRoute)->at("name") )->loadShader(
			(*programRoute)->at("vertex"),
			CGLSLProgram::VERTEX
		);

		//-- Loading Geometry shader if its possible
		if ((*programRoute)->at("geometry") != "") {
			this->illuminationPrograms->at((*programRoute)->at("name"))->loadShader(
				(*programRoute)->at("geometry"),
				CGLSLProgram::GEOMETRY
			);
		}

		this->illuminationPrograms->at((*programRoute)->at("name"))->loadShader(
			(*programRoute)->at("fragment"),
			CGLSLProgram::FRAGMENT
		);

		this->illuminationPrograms->at((*programRoute)->at("name"))->create_link();
	}
}

Stage * Stage::Instance(RenderController * renderController, float width, float height) {
	if (!uniqueStage)
		uniqueStage = new Stage(renderController, width, height);
	return uniqueStage;
}

void Stage::Destroy() {
	if (!uniqueStage) return;
	uniqueStage->~Stage();
}

Projection * Stage::getProjection(){
	return this->projection;
}

void Stage::Notify(string message, void* data) {
	if (message == "init VBOs") {}
		//this->renderFacade->initVBOs();
	else if (message == "width/height") {

	}

	//-- Event Management
	else if (message == "eventKeyPressed") {
		int key = ((int*)data)[0];
		int action = ((int*)data)[1];
		this->camera->keyMove(key, action);
	} else if (message == "eventMouseMove") {
		//this->camera->RotateWithMouse();
	}
	
	/*else if (message == "eventScroll")
		this->camera->calculateZoom( *((int*)data) );
	else if (message == "mouseButton") {
		if (((int*)data)[0] == GLFW_MOUSE_BUTTON_LEFT && ((int*)data)[1] == GLFW_PRESS && !this->clicked) {
			//-- Save first click
			this->clicked = true;
			this->xPos = ((int*)data)[2];
			this->yPos = ((int*)data)[3];
			this->xPosFirst = this->xPos;
			this->yPosFirst = this->yPos;

			this->camera->calculatePitch(0, 0, 0, this->yPos);
			this->camera->calculateAngleAroundPlayer(0, 0, this->xPos, 0);
		}
		else if (((int*)data)[0] == GLFW_MOUSE_BUTTON_LEFT && ((int*)data)[1] == GLFW_PRESS) {
			//-- Updating click values
			this->xPos = this->xPosFirst - ((int*)data)[2];
			this->yPos = this->yPosFirst - ((int*)data)[3];
			this->camera->calculatePitch(0, 0, 0, this->yPos);
			this->camera->calculateAngleAroundPlayer(0, 0, this->xPos, 0);
		}
		else if (((int*)data)[0] == GLFW_MOUSE_BUTTON_LEFT && ((int*)data)[1] == GLFW_RELEASE) {
			//-- Key released
			this->clicked = false;
		}*/
	//}
}

void Stage::render() {
	//-- Render Scene

	//-- Enable functions
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	//-- Functions
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glDepthRange(0.0f, 1.0f);
	glCullFace(GL_BACK);
	
	spMain.UseProgram();

	spMain.SetUniform("matrices.projMatrix", projection->getMatrix());
	spMain.SetUniform("matrices.viewMatrix", camera->Look());

	spMain.SetUniform("gSampler", 0);

	spMain.SetUniform("matrices.modelMatrix", glm::mat4(1.0));
	spMain.SetUniform("matrices.normalMatrix", glm::mat4(1.0));
	spMain.SetUniform("vColor", glm::vec4(1, 1, 1, 1));

	spMain.SetLightData(dlSun, "sunLight");

	spMain.SetUniform("matrices.modelMatrix", glm::translate(glm::mat4(1.0), camera->vEye));
	skybox.RenderSkybox();

	Model::BindModelsVAO();
	for each (Model* model in this->entities) 
		for each (unsigned int index in model->getIndexes()) {
			spMain.SetModelAndNormalMatrix("matrices.modelMatrix", "matrices.normalMatrix", this->transformations[index]->getTransformMatrix());
			model->RenderModel();
		}


	//-- Raytracing Basic Render
	unsigned int id = this->illuminationPrograms->at("RayTracing")->getProgramID();
	this->illuminationPrograms->at("RayTracing")->enable();

	//-- Bind Uniforms
	glUniform1f(glGetUniformLocation(id, "time"), getCurrentTime());
	glUniform2f(glGetUniformLocation(id, "unResolution"), getDisplaySize().x, getDisplaySize().y);
	glUniform3f(glGetUniformLocation(id, "uCameraPosition"), camera->vEye.x, camera->vEye.y, camera->vEye.z);
	glUniformMatrix4fv(glGetUniformLocation(id, "uCameraMatrix"), 1, GL_FALSE, glm::value_ptr(camera->Look()));
	glUniformMatrix4fv(glGetUniformLocation(id, "uProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(projection->getMatrix()));
	glUniform3f(glGetUniformLocation(id, "uLight.color"), dlSun.vColor.x, dlSun.vColor.y, dlSun.vColor.z);
	glUniform3f(glGetUniformLocation(id, "uLight.direction"), dlSun.vDirection.x, dlSun.vDirection.y, dlSun.vDirection.z);
	glUniform1f(glGetUniformLocation(id, "uLight.ambient"), dlSun.fAmbient);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	this->illuminationPrograms->at("RayTracing")->disable();

	//-- Move Camera
	this->camera->Update();

	//-- Disable functions
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);

	glDisable(GL_BLEND);
}
