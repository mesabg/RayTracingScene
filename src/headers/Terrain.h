#pragma once
#ifndef __TERRAIN_H__
#define __TERRAIN_H__

class Terrain {
private:
	float SIZE = 800.0f;
	int VERTEX_COUNT = 128;

	float x, y;

public:
	Terrain();
	~Terrain();
};


#endif