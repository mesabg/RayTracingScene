#include <Player.h>

Player::Player(){
	this->currentSpeed = 0.0f;
	this->currentTurnSpeed = 0.0f;
}

Player::Player(Model* model) {
	this->currentSpeed = 0.0f;
	this->currentTurnSpeed = 0.0f;
	this->model = model;
	this->transformation = model->getTransformation();
}

Player::~Player(){

}

void Player::keyboardInputs(int key, int action){
	if (key == GLFW_KEY_W && action == GLFW_PRESS) this->currentSpeed = -RUN_SPEED;
	else if (key == GLFW_KEY_S && action == GLFW_PRESS) this->currentSpeed = RUN_SPEED;
	else if (action == GLFW_RELEASE) this->currentSpeed = 0.0f;

	if (key == GLFW_KEY_D && action == GLFW_PRESS) this->currentTurnSpeed = -TURN_SPEED;
	else if (key == GLFW_KEY_A && action == GLFW_PRESS) this->currentTurnSpeed = TURN_SPEED;
	else if (action == GLFW_RELEASE) this->currentTurnSpeed = 0.0f;
}

void Player::move(){
	this->model->getTransformation()->increaseRotationMatrix( this->currentTurnSpeed * getFrameTimeSeconds() );
	float distance = this->currentSpeed * getFrameTimeSeconds();
	float dx = distance * glm::sin(glm::radians(this->model->getTransformation()->getRotationY()) /** getFrameTimeSeconds()*/);
	float dz = distance * glm::cos(glm::radians(this->model->getTransformation()->getRotationY()) /** getFrameTimeSeconds()*/);
	this->model->getTransformation()->increaseTraslationMatrix(dx, 0.0f, -dz);
}

void Player::setModel(Model* model) {
	this->model = model;
}

float Player::getRotationY(){
	return this->model->getTransformation()->getRotationY();
}

glm::vec3 Player::getPosition(){
	return this->model->getBoundingBox()->getCenter();
}
