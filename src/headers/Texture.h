#pragma once
#ifndef __TEXTURE_H__
#define __TEXTURE_H__

//-- Defines
#define FOR(q,n) for(int q=0;q<n;q++)
#define SFOR(q,s,e) for(int q=s;q<=e;q++)
#define RFOR(q,n) for(int q=n;q>=0;q--)
#define RSFOR(q,s,e) for(int q=s;q>=e;q--)
#define ESZ(elem) (int)elem.size()

//-- Includes
#include <glew\glew.h>
#include <SOIL\SOIL.h>
#include <FreeImage.h>
#include <iostream>
#include <string>
#include <stdio.h>

//-- Namespaces
using std::string;

//-- 
enum ETextureFiltering{
	TEXTURE_FILTER_MAG_NEAREST = 0, // Nearest criterion for magnification
	TEXTURE_FILTER_MAG_BILINEAR, // Bilinear criterion for magnification
	TEXTURE_FILTER_MIN_NEAREST, // Nearest criterion for minification
	TEXTURE_FILTER_MIN_BILINEAR, // Bilinear criterion for minification
	TEXTURE_FILTER_MIN_NEAREST_MIPMAP, // Nearest criterion for minification, but on closest mipmap
	TEXTURE_FILTER_MIN_BILINEAR_MIPMAP, // Bilinear criterion for minification, but on closest mipmap
	TEXTURE_FILTER_MIN_TRILINEAR, // Bilinear criterion for minification on two closest mipmaps, then averaged
};


class Texture{
private:
	int iWidth, iHeight, iBPP; // Texture width, height, and bytes per pixel
	unsigned int uiTexture; // Texture name
	unsigned int uiSampler; // Sampler name
	bool bMipMapsGenerated;
	int tfMinification, tfMagnification;
	string sPath;

public:
	void CreateEmptyTexture(int a_iWidth, int a_iHeight, GLenum format);
	void CreateFromData(unsigned char* bData, int a_iWidth, int a_iHeight, int a_iBPP, GLenum format, bool bGenerateMipMaps = false);

	bool ReloadTexture();

	bool LoadTexture2D(string a_sPath, bool bGenerateMipMaps = false);
	void BindTexture(int iTextureUnit = 0);

	void SetFiltering(int a_tfMagnification, int a_tfMinification);
	void SetSamplerParameter(GLenum parameter, GLenum value);

	int GetMinificationFilter();
	int GetMagnificationFilter();

	int GetWidth();
	int GetHeight();
	int GetBPP();

	unsigned int GetTextureID();

	string GetPath();

	void DeleteTexture();

	Texture();
};

#define NUMTEXTURES 1
extern Texture tTextures[NUMTEXTURES];
void LoadAllTextures();
#endif

