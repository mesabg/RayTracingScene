#include <Model.h>

VertexBufferObject Model::vboModelData;
unsigned int Model::uiVAO;
vector<Texture> Model::tTextures;

string GetDirectoryPath(string sFilePath){
	// Get directory path
	string sDirectory = "";
	RFOR(i, ESZ(sFilePath) - 1)if (sFilePath[i] == '\\' || sFilePath[i] == '/'){
		sDirectory = sFilePath.substr(0, i + 1);
		break;
	}
	return sDirectory;
}


vector<unsigned int> Model::getIndexes(){
	return this->indexes;
}

Model::Model(vector<unsigned int> indexes){
	bLoaded = false;
	this->indexes = indexes;
	this->properties = properties;
}


bool Model::LoadModelFromFile(const char * sFilePath){
	if (vboModelData.GetBufferID() == 0){
		vboModelData.CreateVBO();
		tTextures.reserve(50);
	}
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(sFilePath,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);

	if (!scene) {
		printf("Couldn't load model Error Importing Asset");
		exit(0);
		return false;
	}

	const int iVertexTotalSize = sizeof(aiVector3D) * 2 + sizeof(aiVector2D);

	int iTotalVertices = 0;

	UFOR(i, scene->mNumMeshes) {
		aiMesh* mesh = scene->mMeshes[i];
		int iMeshFaces = mesh->mNumFaces;
		iMaterialIndices.push_back(mesh->mMaterialIndex);
		int iSizeBefore = vboModelData.GetCurrentSize();
		iMeshStartIndices.push_back(iSizeBefore / iVertexTotalSize);
		FOR(j, iMeshFaces) {
			const aiFace& face = mesh->mFaces[j];
			FOR(k, 3) {
				aiVector3D pos = mesh->mVertices[face.mIndices[k]];
				aiVector3D uv = mesh->mTextureCoords[0][face.mIndices[k]];
				aiVector3D normal = mesh->HasNormals() ? mesh->mNormals[face.mIndices[k]] : aiVector3D(1.0f, 1.0f, 1.0f);
				vboModelData.AddData(&pos, sizeof(aiVector3D));
				vboModelData.AddData(&uv, sizeof(aiVector2D));
				vboModelData.AddData(&normal, sizeof(aiVector3D));
			}
		}
		int iMeshVertices = mesh->mNumVertices;
		iTotalVertices += iMeshVertices;
		iMeshSizes.push_back((vboModelData.GetCurrentSize() - iSizeBefore) / iVertexTotalSize);
	}
	iNumMaterials = scene->mNumMaterials;

	vector<int> materialRemap(iNumMaterials);

	FOR(i, iNumMaterials) {
		const aiMaterial* material = scene->mMaterials[i];
		int a = 5;
		int texIndex = 0;
		aiString path;  // filename

		if (material->GetTexture(aiTextureType_DIFFUSE, texIndex, &path) == AI_SUCCESS) {
			string sDir = GetDirectoryPath(sFilePath);
			string sTextureName = path.data;
			string sFullPath = sDir + sTextureName;
			int iTexFound = -1;
			FOR(j, ESZ(tTextures))if (sFullPath == tTextures[j].GetPath()) {
				iTexFound = j;
				break;
			}
			if (iTexFound != -1)materialRemap[i] = iTexFound;
			else {
				Texture tNew;
				tNew.LoadTexture2D(sFullPath, true);
				materialRemap[i] = ESZ(tTextures);
				tTextures.push_back(tNew);
			}
		}
	}

	FOR(i, ESZ(iMeshSizes)) {
		int iOldIndex = iMaterialIndices[i];
		iMaterialIndices[i] = materialRemap[iOldIndex];
	}

	return bLoaded = true;
}

void Model::FinalizeVBO(){
	glGenVertexArrays(1, &uiVAO);
	glBindVertexArray(uiVAO);
	vboModelData.BindVBO();
	vboModelData.UploadDataToGPU(GL_STATIC_DRAW);
	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(aiVector3D) + sizeof(aiVector2D), 0);
	// Texture coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(aiVector3D) + sizeof(aiVector2D), (void*)sizeof(aiVector3D));
	// Normal vectors
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(aiVector3D) + sizeof(aiVector2D), (void*)(sizeof(aiVector3D) + sizeof(aiVector2D)));
}


void Model::BindModelsVAO(){
	glBindVertexArray(uiVAO);
}

void Model::RenderModel(){
	if (!bLoaded)return;
	int iNumMeshes = ESZ(iMeshSizes);
	FOR(i, iNumMeshes) {
		int iMatIndex = iMaterialIndices[i];
		tTextures[iMatIndex].BindTexture();
		glDrawArrays(GL_TRIANGLES, iMeshStartIndices[i], iMeshSizes[i]);
	}
}