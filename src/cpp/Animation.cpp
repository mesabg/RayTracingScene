#include <Animation.h>

Animation::Animation(Transformation * transformation){
	this->transformation = transformation;
	this->isOn = false;
	this->index = 0;
}

Animation::Animation(Transformation * transformation, vector<State*>* state){
	this->state = state;
	this->transformation = transformation;
	this->isOn = false;
	this->index = 0;
	this->iterations = 0;
	this->reverse = false;
}

Animation::~Animation(){

}

void Animation::setOn(bool isOn){
	this->isOn = isOn;
}

void Animation::setOnRet(){
	this->isOn = !this->isOn;
}

void Animation::animate(){
	if (!this->isOn) return;

	start:
	if (!this->reverse) goto front;
	else goto back;

	front:
	if (this->iterations == this->state->at(this->index)->iterations) {
		this->state->at(this->index)->done = true;
		this->iterations = 0;
		if (this->allDone()) {
			this->reverse = true;
			this->index = this->state->size() - 1;
			goto start;
		}
		this->index++;
	}
	this->transformation->increaseTraslationMatrix(this->state->at(this->index)->direction.x, this->state->at(this->index)->direction.y, this->state->at(this->index)->direction.z);
	this->iterations++;
	return;

	back:
	if (this->iterations == this->state->at(this->index)->iterations) {
		this->state->at(this->index)->done = false;
		this->iterations = 0;
		if (!this->allNoDone()) {
			this->reverse = false;
			this->index = 0;
			goto start;
		}
		this->index --;
	}

	this->transformation->increaseTraslationMatrix(-this->state->at(this->index)->direction.x, -this->state->at(this->index)->direction.y, -this->state->at(this->index)->direction.z);
	this->iterations++;
	return;
	/*
	if (this->transformation->getPosition()->z < 26.2f )
		this->transformation->increaseTraslationMatrix(0.0f, 0.0f, 0.1f);
	else if (this->transformation->getPosition()->x > 2.30f)
		this->transformation->increaseTraslationMatrix(-0.1f, 0.0f, 0.0f);*/
}

bool Animation::allDone(){
	bool ret = true;
	for (int i = 0; i < (int)this->state->size(); i++)
		ret = ret && this->state->at(i)->done;
	return ret;
}

bool Animation::allNoDone(){
	bool ret = false;
	for (int i = 0; i < (int)this->state->size(); i++)
		ret = ret || this->state->at(i)->done;
	return ret;
}

State::State(){

}

State::State(vec3 nextPosition, vec3 direction, bool done, int iterations){
	this->nextPosition = nextPosition;
	this->direction = direction;
	this->done = done;
	this->iterations = iterations;
}

State::~State(){

}
