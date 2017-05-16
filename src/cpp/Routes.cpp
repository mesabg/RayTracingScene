#include <Routes.h>

Routes::Routes(string model){
	this->model = model;
	this->texture = "";
}

Routes::Routes(string model, string texture) {
	this->model = model;
	this->texture = texture;
	this->isNormalMapped = false;
}

Routes::Routes(string model, vector<string> textureRoutes){
	this->model = model;
	this->texture = textureRoutes[0];
	this->normalTexture = textureRoutes[1];
	this->isNormalMapped = true;
}

Routes::Routes() {

}

Routes::~Routes() {
	this->model.~basic_string();
	this->texture.~basic_string();
	return;
}
