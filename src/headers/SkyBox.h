#pragma once
#ifndef __SKYBOX_H__
#define __SKYBOX_H__

//-- Defines
#define FOR(q,n) for(int q=0;q<n;q++)
#define UFOR(q,n) for(unsigned int q=0;q<n;q++)
#define SFOR(q,s,e) for(int q=s;q<=e;q++)
#define RFOR(q,n) for(int q=n;q>=0;q--)
#define RSFOR(q,s,e) for(int q=s;q>=e;q--)
#define ESZ(elem) (int)elem.size()

//-- Global Includes
#include <glm\glm\glm.hpp>

//-- Class Includes
#include <Texture.h>
#include <VertexBufferObject.h>

class Skybox{
public:
	void LoadSkybox(string a_sDirectory, string a_sFront, string a_sBack, string a_sLeft, string a_sRight, string a_sTop, string a_sBottom);
	void RenderSkybox();
	void DeleteSkybox();
private:
	unsigned int uiVAO;
	VertexBufferObject vboRenderData;
	Texture tTextures[6];
	string sDirectory;
	string sFront, sBack, sLeft, sRight, sTop, sBottom;
};

#endif