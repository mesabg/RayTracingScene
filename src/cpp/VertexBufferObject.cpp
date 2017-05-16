#include <VertexBufferObject.h>

VertexBufferObject::VertexBufferObject(){
	bDataUploaded = false;
	uiBuffer = 0;
}


void VertexBufferObject::CreateVBO(int a_iSize){
	glGenBuffers(1, &uiBuffer);
	data.reserve(a_iSize);
	iSize = a_iSize;
	iCurrentSize = 0;
}


void VertexBufferObject::DeleteVBO(){
	glDeleteBuffers(1, &uiBuffer);
	bDataUploaded = false;
	data.clear();
}


void* VertexBufferObject::MapBufferToMemory(int iUsageHint){
	if (!bDataUploaded)return NULL;
	void* ptrRes = glMapBuffer(iBufferType, iUsageHint);
	return ptrRes;
}


void* VertexBufferObject::MapSubBufferToMemory(int iUsageHint, unsigned int uiOffset, unsigned int uiLength){
	if (!bDataUploaded)return NULL;
	void* ptrRes = glMapBufferRange(iBufferType, uiOffset, uiLength, iUsageHint);
	return ptrRes;
}


void VertexBufferObject::UnmapBuffer(){
	glUnmapBuffer(iBufferType);
}


void VertexBufferObject::BindVBO(int a_iBufferType){
	iBufferType = a_iBufferType;
	glBindBuffer(iBufferType, uiBuffer);
}


void VertexBufferObject::UploadDataToGPU(int iDrawingHint){
	glBufferData(iBufferType, data.size(), &data[0], iDrawingHint);
	bDataUploaded = true;
	data.clear();
}


void VertexBufferObject::AddData(void* ptrData, unsigned int uiDataSize){
	data.insert(data.end(), (unsigned char*)ptrData, (unsigned char*)ptrData + uiDataSize);
	iCurrentSize += uiDataSize;
}


void* VertexBufferObject::GetDataPointer(){
	if (bDataUploaded)return NULL;
	return (void*)data[0];
}


unsigned int VertexBufferObject::GetBufferID(){
	return uiBuffer;
}


int VertexBufferObject::GetCurrentSize(){
	return iCurrentSize;
}