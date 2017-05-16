#include <Transformation.h>

Transformation::Transformation() {
	this->TInverse = mat4(1.0f);
	this->T = mat4(1.0f);
	this->S = mat4(1.0f);
	this->R = mat4(1.0f);
	this->setTraslationMatrix(0.0f, 0.0f, 0.0f);
	this->setScaleMatrix(1.0f);
	this->setRotationMatrix(0.0f, 1.0f, 0.0f, 0.0f);
}

Transformation::Transformation(float tx, float ty, float tz, float scale, float angle, float rx, float ry, float rz) {
	this->TInverse = mat4(1.0f);
	this->T = mat4(1.0f);
	this->S = mat4(1.0f);
	this->R = mat4(1.0f);
	this->setTraslationMatrix(tx, ty, tz);
	this->setScaleMatrix(scale);
	this->setRotationMatrix(angle, rx, ry, rz);
}

Transformation::Transformation(float scale, vec3 position, float angle, vec3 rotationDir){
	this->TInverse = mat4(1.0f);
	this->T = mat4(1.0f);
	this->S = mat4(1.0f);
	this->R = mat4(1.0f);
	this->setTraslationMatrix(position);
	this->setRotationMatrix(angle, rotationDir);
	this->setScaleMatrix(scale);
}

Transformation::~Transformation() {

}

vec3* Transformation::getPosition(){
	return &(this->t);
}

mat4 Transformation::getTransformMatrix() {
	return this->T*this->R*this->S;
}

mat4 Transformation::getTraslationInverseMatrix() {
	return this->TInverse;
}

mat4 Transformation::getTraslationMatrix() {
	return this->T;
}

mat4 Transformation::getScaleMatrix() {
	return this->S;
}

quat * Transformation::getQuaternion(){
	return &this->quaternion;
}

float * Transformation::getScaleValue(){
	return &this->scale;
}

vec3 * Transformation::getTraslationValues(){
	return &this->r;
}

float * Transformation::getTraslationValueX(){
	return &this->t.x;
}

float * Transformation::getTraslationValueY() {
	return &this->t.y;
}

float * Transformation::getTraslationValueZ() {
	return &this->t.z;
}

mat4 Transformation::getRotationMatrix() {
	return this->R;
}

float Transformation::getRotationY(){
	return this->rotY;
}

void Transformation::setTraslationMatrix(float tx, float ty, float tz) {
	this->t = vec3(tx, ty, tz);
	this->T = glm::translate(this->t);
}

void Transformation::setTraslationMatrix(vec3 t){
	this->t = t;
	this->T = glm::translate(this->t);
}

void Transformation::setScaleMatrix(float scale) {
	this->scale = scale;
	this->S = glm::scale(vec3(this->scale, this->scale, this->scale));
}

void Transformation::setRotationMatrix(float angle, float rx, float ry, float rz) {
	this->r = vec3(rx, ry, rz);
	this->angle = angle;
	this->rotY = 0.0f;
	this->R = glm::rotate(this->angle, this->r);
}

void Transformation::setRotationMatrix(float angle, vec3 tr){
	this->r = tr;
	this->angle = angle;
	this->rotY = 0.0f;
	this->R = glm::rotate(this->angle, this->r);
}

void Transformation::increaseTraslationMatrix(float tx, float ty, float tz) {
	this->t.x += tx;
	this->t.y += ty;
	this->t.z += tz;
	this->T = glm::translate(this->t);
}

void Transformation::addScaleMatrix(float scale) {
	for (int i = 0; i < 3; i++) this->S[i][i] += scale;
}

void Transformation::increaseRotationMatrix(float angle) {
	//angle = glm::radians(angle);
	this->rotY = this->angle - angle;
	this->angle += angle;
	this->R = glm::rotate(glm::radians(this->angle), this->r);
}

void Transformation::updateTraslationMatrix(){
	this->T = glm::translate(this->t);
}

void Transformation::updateRotationMatrix(){
	this->R = glm::rotate(this->angle, this->r);
}

void Transformation::updateRotationMatrixQuaternion(){
	//vec4 rotatedPoint = vec4(this->quaternion.x, this->quaternion.y, this->quaternion.z, this->quaternion.w) * vec4(this->t, 0.0f);
	//this->R = glm::toMat4(glm::quat(rotatedPoint.x, rotatedPoint.y, rotatedPoint.z, rotatedPoint.w));
	this->R = glm::toMat4(this->quaternion);
	//-- First Row
	/*this->R[0][0] = 1.0f - 2.0f * (quaternion[1] * quaternion[1] + quaternion[2] * quaternion[2]);
	this->R[0][1] = 2.0f * (quaternion[0] * quaternion[1] - quaternion[3] * quaternion[2]);
	this->R[0][2] = 2.0f * (quaternion[0] * quaternion[2] + quaternion[3] * quaternion[1]);
	this->R[0][3] = 0.0f;

	//-- Second Row
	this->R[1][0] = 2.0f * (quaternion[0] * quaternion[1] + quaternion[3] * quaternion[2]);
	this->R[1][1] = 1.0f - 2.0f * (quaternion[0] * quaternion[0] + quaternion[2] * quaternion[2]);
	this->R[1][2] = 2.0f * (quaternion[1] * quaternion[2] - quaternion[3] * quaternion[0]);
	this->R[1][3] = 0.0f;

	//-- Third Row
	this->R[2][0] = 2.0f * (quaternion[0] * quaternion[2] - quaternion[3] * quaternion[1]);
	this->R[2][1] = 2.0f * (quaternion[1] * quaternion[2] + quaternion[3] * quaternion[0]);
	this->R[2][2] = 1.0f - 2.0f * (quaternion[0] * quaternion[0] + quaternion[1] * quaternion[1]);
	this->R[2][3] = 0.0f;

	//-- Fourth Row 
	this->R[3][0] = 0.0f;
	this->R[3][1] = 0.0f;
	this->R[3][2] = 0.0f;
	this->R[3][3] = 1.0f;*/
}

void Transformation::updateScaleMatrix(){
	this->S = glm::scale(vec3(this->scale, this->scale, this->scale));
}
