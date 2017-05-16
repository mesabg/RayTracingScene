#pragma once
#ifndef __ROUTES_H__
#define __ROUTES_H__

#include <string>
#include <vector>

using std::string;
using std::vector;

class Routes {
public:
	/*Variables*/
	string model;
	string texture;
	string normalTexture;
	bool isNormalMapped;

	Routes(string model);
	Routes(string model, string texture);
	Routes(string model, vector<string> textureRoutes);

	Routes();
	~Routes();
};

#endif
