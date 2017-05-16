#include <ModelCollection.h>

ModelCollection::ModelCollection(
	vector<Routes*> routes, 
	vector<Transformation*> transformations, 
	vector<Light*> lights, 
	vector<MaterialProperties*> materialProperties){

	for (int i = 0; i < (int)routes.size(); i++) {
		this->entities.push_back(new Reader(routes[i]));
		this->entities.back()->setTransformation(transformations[i]);
		this->entities.back()->setLight(lights[i]);
		this->lightSet.push_back(lights[i]);
		this->entities.back()->setMaterialProperties(materialProperties[i]);
		this->reflectionMap.push_back(false);
	}
}

ModelCollection::~ModelCollection(){
	for (Model* model : this->entities)
		model->~Model();
}

void ModelCollection::render(Projection* projection, Camera* camera, vector<Light*>* lights, CGLSLProgram* shader){
	unsigned int index = 0;
	for (Model* model : this->entities) {
		model->setShader(shader);
		//model->isReflect(this->reflectionMap[index]);
		model->render(projection, camera, lights);
		index++;
	}
}

void ModelCollection::render(Projection * projection, Camera * camera, vector<Light*>* lights, map<string, CGLSLProgram*> *shaders, vector<string> position){
	unsigned int index = 0;
	for (Model* model : this->entities) {
		model->setShader(shaders->at(position[index]));
		//model->isReflect(this->reflectionMap[index]);
		model->render(projection, camera, lights);
		index++;
	}
}

void ModelCollection::render(Projection * projection, Camera * camera, vector<Light*>* lights, map<string, CGLSLProgram*>* shaders, vector<string> position, GLuint cubeTex){
	unsigned int index = 0;
	for (Model* model : this->entities) {
		model->setShader(shaders->at(position[index]));
		shaders->at(position[index])->enable();
		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_2D, cubeTex);
		glUniform1i(shaders->at(position[index])->getLocation("cube_shadow"), 5);
		
		model->render(projection, camera, lights);
		shaders->at(position[index])->disable();
		index++;
	}
}

void ModelCollection::low_render(glm::mat4 depthMVP, CGLSLProgram * shader){
	for (Model* model : this->entities) {
		model->setShader(shader);
		model->lowRender(depthMVP);
	}
}

void ModelCollection::initVBOs(){
	for (Model* model : this->entities)
		model->initGLDataBinding();
}

void ModelCollection::setSkyBox(SkyBox * skybox){
	for (Model* model : this->entities)
		model->setSkyBox(skybox);
}

void ModelCollection::setReflectionMap(vector<bool> reflectionMap){
	this->reflectionMap = reflectionMap;
}

void ModelCollection::setShadow(glm::mat4 depthBiasMVP, GLuint ShadowMapId){
	for (Model* model : this->entities) {
		model->setDepthBiasMVP(depthBiasMVP);
		model->setShadowMapId(ShadowMapId);
	}
}

Model * ModelCollection::getEntity(const unsigned int index){
	return this->entities[index];
}

vector<Light*>* ModelCollection::getLightSet(){
	return &this->lightSet;
}
