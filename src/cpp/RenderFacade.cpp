#include <RenderFacade.h>

void RenderFacade::checkAssertions(){
	assert(this->shaders != NULL && "No shaders loaded, load shaders first");
	assert(this->model_entities.size() != 0 && "No models loaded, load some models first");
	assert(this->light_entities.size() != 0 && "No lights loaded, load some lights first");
	assert(this->camera != NULL && "No camera loaded, load a camera first");
	assert(this->projection != NULL && "No projection loaded, load a projection first");
	assert(this->frameBufferObject != NULL && "No FBO loaded, load a FBO first");
	assert(this->lowQualityRenderShader != "" && "No Low Quality Shader Name identified");
	assert(this->depthShader != "" && "No Depth Shader Name identified");
}

void RenderFacade::switchFace(Camera * cam, const int index){
	if (index == 0) cam->setPitchYaw(0.0f, 90.0f);
	else if (index == 1) cam->setPitchYaw(0.0f, -90.0f);
	else if (index == 2) cam->setPitchYaw(-90.0f, 180.0f);
	else if (index == 3) cam->setPitchYaw(90.0f, 180.0f);
	else if (index == 4) cam->setPitchYaw(0.0f, 180.0f);
	else if (index == 5) cam->setPitchYaw(0.0f, 0.0f);
}

RenderFacade::RenderFacade(){
	this->shaders = NULL;
	this->frameBufferObject = NULL;
	this->projection = NULL;
	this->camera = NULL;

	//-- To draw shadow map
	GLfloat planeVertex[] =
	{
		4.6f, 2.0f, 0.0f, 1.0f, 0.0f,
		2.0f, 4.6f, 0.0f, 0.0f, 1.0f,
		2.0f, 2.0f, 0.0f, 0.0f, 0.0f,

		4.6f, 2.0f, 0.0f, 1.0f, 0.0f,
		4.6f, 4.6f, 0.0f, 1.0f, 1.0f,
		2.0f, 4.6f, 0.0f, 0.0f, 1.0f
	};

	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);
	glBindVertexArray(planeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertex), &planeVertex, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glBindVertexArray(0);

	texture_program.loadShader("../src/shaders/texture.vert", CGLSLProgram::VERTEX);
	texture_program.loadShader("../src/shaders/texture.frag", CGLSLProgram::FRAGMENT);

	texture_program.create_link();

	texture_program.enable();

	texture_program.addAttribute("position");
	texture_program.addAttribute("texcoord");

	texture_program.addUniform("projection");
	texture_program.addUniform("view");
	texture_program.addUniform("model");
	texture_program.addUniform("texture1");

	texture_program.disable();
}

RenderFacade::~RenderFacade(){

}

void RenderFacade::initModelEntities(
	const vector<Routes*> routes, 
	const vector<Transformation*> transformations, 
	const vector<Light*> lights, 
	const vector<MaterialProperties*> materialProperties){
	for (int i = 0; i < (int)routes.size(); i++) {
		this->model_entities.push_back(new Reader(routes[i]));
		this->model_entities.back()->setTransformation(transformations[i]);
		this->model_entities.back()->setLight(lights[i]);
		this->model_entities.back()->setMaterialProperties(materialProperties[i]);
	}
}

void RenderFacade::initLightEntities(
	const vector<Routes*> routes, 
	const vector<Transformation*> transformations,
	const vector<Light*> lights, 
	const vector<MaterialProperties*> materialProperties){
	for (int i = 0; i < (int)routes.size(); i++) {
		this->light_entities.push_back(new Reader(routes[i]));
		this->light_entities.back()->setTransformation(transformations[i]);
		this->light_entities.back()->setLight(lights[i]);
		this->light_entities.back()->setMaterialProperties(materialProperties[i]);
	}
}

void RenderFacade::bindProjection(Projection * projection){
	this->projection = projection;
}

void RenderFacade::bindCamera(Camera * camera){
	this->camera = camera;
}

void RenderFacade::bindShaderPrograms(map<string, CGLSLProgram*>* shaders){
	this->shaders = shaders;
}

void RenderFacade::bindFBO(FrameBuffer * FBO){
	this->frameBufferObject = FBO;
}

void RenderFacade::render(string shaderName){
	this->checkAssertions();
	this->FBOrender_shadow_map();
	this->buildCubeMap();
	CGLSLProgram* shader = this->shaders->at(shaderName);
	shader->enable();
	this->camera->bind(shader);
	this->projection->bind(shader);
	int light_index = 0;
	for (Model* light : this->light_entities) {
		light->getLight()->bind(shader, false, light_index);
		light_index++;
	}
	for (Model* model : this->model_entities) {
		model->bind(shader);
		model->render(shader);
	}
	for (Model* light : this->light_entities) {
		light->bind(shader);
		light->render(shader);
	}
	shader->disable();
}

void RenderFacade::render(Projection * projection, Camera * camera, string shaderName){
	this->checkAssertions();
	this->FBOrender_shadow_map();
	CGLSLProgram* shader = this->shaders->at(shaderName);
	shader->enable();
	camera->bind(shader);
	projection->bind(shader);
	int light_index = 0;
	for (Model* light : this->light_entities) {
		light->getLight()->bind(shader, false, light_index);
		light_index++;
	}
	for (Model* model : this->model_entities) {
		model->bind(shader);
		model->render(shader);
	}
	for (Model* light : this->light_entities) {
		light->bind(shader);
		light->render(shader);
	}
	shader->disable();
}

void RenderFacade::low_quality_render(Model* modelExclude){
	this->checkAssertions();
	CGLSLProgram* shader = this->shaders->at(this->lowQualityRenderShader);
	shader->enable();
	this->camera->bind(shader);
	this->projection->bind(shader);
	int light_index = 0;
	for (Model* light : this->light_entities) {
		light->getLight()->bind(shader, false, light_index);
		light_index++;
	}
	for (Model* model : this->model_entities) {
		if (model != modelExclude) {
			model->bind(shader);
			model->render(shader);
		}
	}
	for (Model* light : this->light_entities) {
		light->bind(shader);
		light->render(shader);
	}
	shader->disable();
}

Model * RenderFacade::getModel(const int index){
	return this->model_entities.at(index);
}

Model * RenderFacade::getLightModel(const int index){
	return this->light_entities.at(index);
}

void RenderFacade::initVBOs(){
	for (Model* model : this->model_entities)
		model->initGLDataBinding();

	for (Model* light : this->light_entities)
		light->initGLDataBinding();
}
