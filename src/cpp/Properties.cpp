#include <Properties.h>

Properties::Properties(vec3 diffuse, vec3 specular, vec3 ambient, float shininess){
	this->diffuse = diffuse;
	this->specular = specular;
	this->ambient = ambient;
	this->shininess = shininess;
}

Properties::~Properties(){
}
