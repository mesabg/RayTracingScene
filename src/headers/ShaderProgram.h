#pragma once
#ifndef __SHADER_PROGRAM_H__
#define __SHADER_PROGRAM_H__

//-- Defines
#define FOR(q,n) for(int q=0;q<n;q++)
#define UFOR(q,n) for(unsigned int q=0;q<n;q++)
#define SFOR(q,s,e) for(int q=s;q<=e;q++)
#define RFOR(q,n) for(int q=n;q>=0;q--)
#define RSFOR(q,s,e) for(int q=s;q>=e;q--)
#define ESZ(elem) (int)elem.size()

//-- Global Includes
#include <glew\glew.h>
#include <glm\glm\glm.hpp>
#include <vector>
#include <string>
#include <sstream>

//-- Class Includes
#include <Light.h>

using std::string;
using std::stringstream;
using std::vector;

class Shader{
public:
	bool LoadShader(string sFile, int a_iType);
	void DeleteShader();

	bool GetLinesFromFile(string sFile, bool bIncludePart, vector<string>* vResult);

	bool IsLoaded();
	unsigned int GetShaderID();

	Shader();

private:
	unsigned int uiShader; // ID of shader
	int iType; // GL_VERTEX_SHADER, GL_FRAGMENT_SHADER...
	bool bLoaded; // Whether shader was loaded and compiled
};




class ShaderProgram{
public:
	void CreateProgram();
	void DeleteProgram();

	bool AddShaderToProgram(Shader* shShader);
	bool LinkProgram();

	void UseProgram();
	void DisableProgram();

	unsigned int GetProgramID();

	// Setting vectors
	void SetUniform(string sName, glm::vec2* vVectors, int iCount = 1);
	void SetUniform(string sName, const glm::vec2 vVector);
	void SetUniform(string sName, glm::vec3* vVectors, int iCount = 1);
	void SetUniform(string sName, const glm::vec3 vVector);
	void SetUniform(string sName, glm::vec4* vVectors, int iCount = 1);
	void SetUniform(string sName, const glm::vec4 vVector);

	// Setting floats
	void SetUniform(string sName, float* fValues, int iCount = 1);
	void SetUniform(string sName, const float fValue);

	// Setting 3x3 matrices
	void SetUniform(string sName, glm::mat3* mMatrices, int iCount = 1);
	void SetUniform(string sName, const glm::mat3 mMatrix);

	// Setting 4x4 matrices
	void SetUniform(string sName, glm::mat4* mMatrices, int iCount = 1);
	void SetUniform(string sName, const glm::mat4 mMatrix);

	// Setting integers
	void SetUniform(string sName, int* iValues, int iCount = 1);
	void SetUniform(string sName, const int iValue);

	// Model and normal matrix setting ispretty common
	void SetModelAndNormalMatrix(string sModelMatrixName, string sNormalMatrixName, glm::mat4 mModelMatrix);
	void SetModelAndNormalMatrix(string sModelMatrixName, string sNormalMatrixName, glm::mat4* mModelMatrix);

	void SetLightData(Light l, string name);

	ShaderProgram();

private:
	unsigned int uiProgram; // ID of program
	bool bLinked; // Whether program was linked and is ready to use
};

bool PrepareShaderPrograms();

#define NUMSHADERS 6
extern Shader shShaders[NUMSHADERS];
extern ShaderProgram spMain, spOrtho2D, spFont2D, spColor, spRayTracing;

#endif