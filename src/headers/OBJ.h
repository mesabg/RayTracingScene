#pragma once
#ifndef __OBJ_H_
#define __OBJ_H_

/*Global Includes*/
#include <fstream>
#include <iostream>

/*Class Includes*/
#include <Model.h>

/*Using spaces*/
using std::ifstream;

class OBJ : public Model
{
private:
	int index;
public:
	OBJ(Routes* routes);
	~OBJ();
	void agregarFace(vector<string> Faces);
};

#endif