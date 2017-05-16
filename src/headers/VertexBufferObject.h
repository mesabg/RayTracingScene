#pragma once
#ifndef __VERTEX_BUFFER_OBJECT_H__
#define __VERTEX_BUFFER_OBJECT_H__

//-- Global Includes
#include <glew\glew.h>
#include <vector>

//-- Using Spaces
using std::vector;

class VertexBufferObject{
public:
	void CreateVBO(int a_iSize = 0);
	void DeleteVBO();

	void* MapBufferToMemory(int iUsageHint);
	void* MapSubBufferToMemory(int iUsageHint, unsigned int uiOffset, unsigned int uiLength);
	void UnmapBuffer();

	void BindVBO(int a_iBufferType = GL_ARRAY_BUFFER);
	void UploadDataToGPU(int iUsageHint);

	void AddData(void* ptrData, unsigned int uiDataSize);

	void* GetDataPointer();
	unsigned int GetBufferID();

	int GetCurrentSize();

	VertexBufferObject();

private:
	unsigned int uiBuffer;
	int iSize;
	int iCurrentSize;
	int iBufferType;
	vector<unsigned char> data;

	bool bDataUploaded;
};

#endif
