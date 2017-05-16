#include <Projection.h>

void Projection::refresh() {
	this->projection = glm::perspective(this->fieldOfView, this->aspectRatio, this->near_, this->far_);
}

Projection::Projection() {
	this->fieldOfView = 45.0f;
	this->aspectRatio = 1440.0f / 900.0f;
	this->near_ = 1.0f;
	this->far_ = 1000.0f;
	this->refresh();
}

Projection::Projection(float fieldOfView, float aspectRatio, float near_, float far_) {
	this->fieldOfView = fieldOfView;
	this->aspectRatio = aspectRatio;
	this->near_ = near_;
	this->far_ = far_;
	this->refresh();
}

Projection::~Projection() {

}

mat4 Projection::getMatrix() {
	return this->projection;
}

mat4 * Projection::getProjection(){
	return &this->projection;
}

float Projection::getFieldOfView() {
	return this->fieldOfView;
}

float Projection::getAspectRatio() {
	return this->aspectRatio;
}

float Projection::getNear() {
	return this->near_;
}

float Projection::getFar() {
	return this->far_;
}

void Projection::setFieldOfView(float fieldOfView) {
	this->fieldOfView = fieldOfView;
	this->refresh();
}

void Projection::setAspectRatio(float aspectRatio) {
	this->aspectRatio = aspectRatio;
	this->refresh();
}

void Projection::setNearAndFar(float near_, float far_) {
	this->near_ = near_;
	this->far_ = far_;
	this->refresh();
}

void Projection::setNear(float near_) {
	this->near_ = near_;
	this->refresh();
}

void Projection::setFar(float far_) {
	this->far_ = far_;
	this->refresh();
}

void Projection::addFieldOfView(float fieldOfView) {
	this->fieldOfView += fieldOfView;
	this->refresh();
}

void Projection::addAspectRatio(float aspectRatio) {
	this->aspectRatio += aspectRatio;
	this->refresh();
}

void Projection::addNearAndFar(float near_, float far_) {
	this->near_ += near_;
	this->far_ += far_;
	this->refresh();
}

void Projection::addNear(float near_) {
	this->near_ = near_;
	this->refresh();
}

void Projection::addFar(float far_) {
	this->far_ = far_;
	this->refresh();
}
