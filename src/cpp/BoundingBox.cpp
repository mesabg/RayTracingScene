#include <BoundingBox.h>
#include <iostream>

BoundingBox::BoundingBox() {

}

BoundingBox::BoundingBox(glm::vec3 center){
	this->center = center;
}

BoundingBox::BoundingBox(glm::vec3 min, glm::vec3 max){
	this->setBorder(min, max);

	//-- Saving vertices
	this->vertices = new vector<vec3>(8, vec3());
	this->vertices->at(0) = vec3(max.x, max.y, max.z);
	this->vertices->at(1) = vec3(max.x, max.y, min.z);
	this->vertices->at(2) = vec3(max.x, min.y, max.z);
	this->vertices->at(3) = vec3(max.x, min.y, min.z);
	this->vertices->at(4) = vec3(min.x, max.y, max.z);
	this->vertices->at(5) = vec3(min.x, max.y, min.z);
	this->vertices->at(6) = vec3(min.x, min.y, max.z);
	this->vertices->at(7) = vec3(min.x, min.y, min.z);

	//-- Saving faces
	this->faces = new vector<vec4>(6, vec4(0));
	this->faces->at(0) = vec4(4, 6, 2, 0);
	this->faces->at(1) = vec4(5, 7, 6, 4);
	this->faces->at(2) = vec4(1, 3, 7, 5);
	this->faces->at(3) = vec4(0, 2, 3, 1);
	this->faces->at(4) = vec4(5, 4, 0, 1);
	this->faces->at(5) = vec4(7, 6, 2, 0);

	//-- Saving normals
	this->normals = new vector<vec3>(6, vec3());
	for (int i = 0; i < 6; i++)
		this->normals->at(i) = glm::normalize(glm::cross(
			this->vertices->at(this->faces->at(i).x) - this->vertices->at(this->faces->at(i).y), 
			this->vertices->at(this->faces->at(i).z) - this->vertices->at(this->faces->at(i).y)));

	//-- Calculatin Center
	this->center = (this->min + this->max) / 2.0f;
}

BoundingBox::~BoundingBox() {}

void BoundingBox::move(mat4 transformation){
	this->min = vec3(transformation * vec4(this->min, 1.0f));
	this->max = vec3(transformation * vec4(this->max, 1.0f));
	this->center = (this->min + this->max) / 2.0f;
	//std::cout << this->center.x << " " << this->center.y << " " << this->center.z << std::endl;
}

bool BoundingBox::checkIntersection(vec3 direction){
	for (vec3 normal : *(this->normals))
		if (glm::dot(normal, direction) == 0.0f)
			return false;
	return true;
}

void BoundingBox::recalculate(){
	this->setBorder(min, max);

	//-- Saving vertices
	this->vertices = new vector<vec3>(8, vec3());
	this->vertices->at(0) = vec3(max.x, max.y, max.z);
	this->vertices->at(1) = vec3(max.x, max.y, min.z);
	this->vertices->at(2) = vec3(max.x, min.y, max.z);
	this->vertices->at(3) = vec3(max.x, min.y, min.z);
	this->vertices->at(4) = vec3(min.x, max.y, max.z);
	this->vertices->at(5) = vec3(min.x, max.y, min.z);
	this->vertices->at(6) = vec3(min.x, min.y, max.z);
	this->vertices->at(7) = vec3(min.x, min.y, min.z);

	//-- Saving faces
	this->faces = new vector<vec4>(6, vec4(0));
	this->faces->at(0) = vec4(4, 6, 2, 0);
	this->faces->at(1) = vec4(5, 7, 6, 4);
	this->faces->at(2) = vec4(1, 3, 7, 5);
	this->faces->at(3) = vec4(0, 2, 3, 1);
	this->faces->at(4) = vec4(5, 4, 0, 1);
	this->faces->at(5) = vec4(7, 6, 2, 0);

	//-- Saving normals
	this->normals = new vector<vec3>(6, vec3());
	for (int i = 0; i < 6; i++)
		this->normals->at(i) = glm::normalize(glm::cross(
			this->vertices->at(this->faces->at(i).x) - this->vertices->at(this->faces->at(i).y),
			this->vertices->at(this->faces->at(i).z) - this->vertices->at(this->faces->at(i).y)));

	//-- Calculatin Center
	this->center = (this->min + this->max) / 2.0f;
}

void BoundingBox::draw(){
	glColor3b(1, 0, 0);
	for (int i = 0; i < 6; i++){
		glBegin(GL_QUAD_STRIP);
		glVertex3f(
			this->vertices->at(this->faces->at(i).x).x, 
			this->vertices->at(this->faces->at(i).x).y, 
			this->vertices->at(this->faces->at(i).x).z);
		glVertex3f(
			this->vertices->at(this->faces->at(i).y).x,
			this->vertices->at(this->faces->at(i).y).y,
			this->vertices->at(this->faces->at(i).y).z);
		glVertex3f(
			this->vertices->at(this->faces->at(i).z).x,
			this->vertices->at(this->faces->at(i).z).y,
			this->vertices->at(this->faces->at(i).z).z);
		glVertex3f(
			this->vertices->at(this->faces->at(i).w).x,
			this->vertices->at(this->faces->at(i).w).y,
			this->vertices->at(this->faces->at(i).w).z);
		glEnd();
	}
}

glm::vec3 BoundingBox::getCenter(){
	return this->center;
}

void BoundingBox::setBorder(vec3 min, vec3 max){
	this->min = min;
	this->max = max;
}
