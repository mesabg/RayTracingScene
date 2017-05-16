#include <ShaderProgram.h>

Shader::Shader(){
	bLoaded = false;
}

Shader shShaders[NUMSHADERS];
ShaderProgram spMain, spOrtho2D, spFont2D, spRayTracing;

bool PrepareShaderPrograms() {
	// Load shaders and create shader program
	printf("Shaders being loaded.\n");
	string sShaderFileNames[] = { "main_shader.vert", "main_shader.frag", "ortho2D.vert",
		"ortho2D.frag", "font2D.frag", "dirLight.frag", "raytracing.vert", "raytracing.frag"
	};

	FOR(i, NUMSHADERS) {
		string sExt = sShaderFileNames[i].substr(ESZ(sShaderFileNames[i]) - 4, 4);
		int iShaderType = sExt == "vert" ? GL_VERTEX_SHADER : (sExt == "frag" ? GL_FRAGMENT_SHADER : GL_GEOMETRY_SHADER);
		shShaders[i].LoadShader("..\\src\\shaders\\" + sShaderFileNames[i], iShaderType);
	}

	// Create shader programs

	spMain.CreateProgram();
	spMain.AddShaderToProgram(&shShaders[0]);
	spMain.AddShaderToProgram(&shShaders[1]);
	spMain.AddShaderToProgram(&shShaders[5]);

	if (!spMain.LinkProgram()) return false;

	spOrtho2D.CreateProgram();
	spOrtho2D.AddShaderToProgram(&shShaders[3]);
	spOrtho2D.AddShaderToProgram(&shShaders[4]);
	spOrtho2D.LinkProgram();

	spFont2D.CreateProgram();
	spFont2D.AddShaderToProgram(&shShaders[2]);
	spFont2D.AddShaderToProgram(&shShaders[4]);
	spFont2D.LinkProgram();

	spRayTracing.CreateProgram();
	spRayTracing.AddShaderToProgram(&shShaders[6]);
	spRayTracing.AddShaderToProgram(&shShaders[7]);
	spRayTracing.LinkProgram();

	printf("Shaders loaded succesfully.\n");
	return true;
}


bool Shader::LoadShader(string sFile, int a_iType){
	vector<string> sLines;

	if (!GetLinesFromFile(sFile, false, &sLines))return false;

	const char** sProgram = new const char*[ESZ(sLines)];
	FOR(i, ESZ(sLines))sProgram[i] = sLines[i].c_str();

	uiShader = glCreateShader(a_iType);

	glShaderSource(uiShader, ESZ(sLines), sProgram, NULL);
	glCompileShader(uiShader);

	delete[] sProgram;

	int iCompilationStatus;
	glGetShaderiv(uiShader, GL_COMPILE_STATUS, &iCompilationStatus);

	if (iCompilationStatus == GL_FALSE){
		char sInfoLog[1024];
		char sFinalMessage[1536];
		int iLogLength;
		glGetShaderInfoLog(uiShader, 1024, &iLogLength, sInfoLog);
		sprintf(sFinalMessage, "Error! Shader file %s wasn't compiled! The compiler returned:\n\n%s", sFile.c_str(), sInfoLog);
		exit(0);
		return false;
	}
	iType = a_iType;
	bLoaded = true;

	return true;
}


bool Shader::GetLinesFromFile(string sFile, bool bIncludePart, vector<string>* vResult){
	FILE* fp = fopen(sFile.c_str(), "rt");
	if (!fp)return false;

	string sDirectory;
	int slashIndex = -1;
	RFOR(i, ESZ(sFile) - 1){
		if (sFile[i] == '\\' || sFile[i] == '/'){
			slashIndex = i;
			break;
		}
	}

	sDirectory = sFile.substr(0, slashIndex + 1);

	// Get all lines from a file

	char sLine[255];

	bool bInIncludePart = false;

	while (fgets(sLine, 255, fp)){
		stringstream ss(sLine);
		string sFirst;
		ss >> sFirst;
		if (sFirst == "#include"){
			string sFileName;
			ss >> sFileName;
			if (ESZ(sFileName) > 0 && sFileName[0] == '\"' && sFileName[ESZ(sFileName) - 1] == '\"'){
				sFileName = sFileName.substr(1, ESZ(sFileName) - 2);
				GetLinesFromFile(sDirectory + sFileName, true, vResult);
			}
		}
		else if (sFirst == "#include_part")
			bInIncludePart = true;
		else if (sFirst == "#definition_part")
			bInIncludePart = false;
		else if (!bIncludePart || (bIncludePart && bInIncludePart))
			vResult->push_back(sLine);
	}
	fclose(fp);

	return true;
}


bool Shader::IsLoaded(){
	return bLoaded;
}


unsigned int Shader::GetShaderID(){
	return uiShader;
}


void Shader::DeleteShader(){
	if (!IsLoaded())return;
	bLoaded = false;
	glDeleteShader(uiShader);
}


ShaderProgram::ShaderProgram(){
	bLinked = false;
}


void ShaderProgram::CreateProgram(){
	uiProgram = glCreateProgram();
}


bool ShaderProgram::AddShaderToProgram(Shader* shShader){
	if (!shShader->IsLoaded())return false;
	glAttachShader(uiProgram, shShader->GetShaderID());
	return true;
}


bool ShaderProgram::LinkProgram(){
	glLinkProgram(uiProgram);
	int iLinkStatus;
	glGetProgramiv(uiProgram, GL_LINK_STATUS, &iLinkStatus);
	bLinked = iLinkStatus == GL_TRUE;
	return bLinked;
}


void ShaderProgram::DeleteProgram(){
	if (!bLinked)return;
	bLinked = false;
	glDeleteProgram(uiProgram);
}


void ShaderProgram::UseProgram(){
	if (bLinked)glUseProgram(uiProgram);
}


void ShaderProgram::DisableProgram() {
	glUseProgram(0);
}

unsigned int ShaderProgram::GetProgramID(){
	return uiProgram;
}


void ShaderProgram::SetUniform(string sName, float* fValues, int iCount){
	int iLoc = glGetUniformLocation(uiProgram, sName.c_str());
	glUniform1fv(iLoc, iCount, fValues);
}


void ShaderProgram::SetUniform(string sName, const float fValue){
	int iLoc = glGetUniformLocation(uiProgram, sName.c_str());
	glUniform1fv(iLoc, 1, &fValue);
}

// Setting vectors

void ShaderProgram::SetUniform(string sName, glm::vec2* vVectors, int iCount){
	int iLoc = glGetUniformLocation(uiProgram, sName.c_str());
	glUniform2fv(iLoc, iCount, (GLfloat*)vVectors);
}


void ShaderProgram::SetUniform(string sName, const glm::vec2 vVector){
	int iLoc = glGetUniformLocation(uiProgram, sName.c_str());
	glUniform2fv(iLoc, 1, (GLfloat*)&vVector);
}


void ShaderProgram::SetUniform(string sName, glm::vec3* vVectors, int iCount){
	int iLoc = glGetUniformLocation(uiProgram, sName.c_str());
	glUniform3fv(iLoc, iCount, (GLfloat*)vVectors);
}


void ShaderProgram::SetUniform(string sName, const glm::vec3 vVector){
	int iLoc = glGetUniformLocation(uiProgram, sName.c_str());
	glUniform3fv(iLoc, 1, (GLfloat*)&vVector);
}


void ShaderProgram::SetUniform(string sName, glm::vec4* vVectors, int iCount){
	int iLoc = glGetUniformLocation(uiProgram, sName.c_str());
	glUniform4fv(iLoc, iCount, (GLfloat*)vVectors);
}


void ShaderProgram::SetUniform(string sName, const glm::vec4 vVector){
	int iLoc = glGetUniformLocation(uiProgram, sName.c_str());
	glUniform4fv(iLoc, 1, (GLfloat*)&vVector);
}

// Setting 3x3 matrices

void ShaderProgram::SetUniform(string sName, glm::mat3* mMatrices, int iCount){
	int iLoc = glGetUniformLocation(uiProgram, sName.c_str());
	glUniformMatrix3fv(iLoc, iCount, GL_FALSE, (GLfloat*)mMatrices);
}


void ShaderProgram::SetUniform(string sName, const glm::mat3 mMatrix){
	int iLoc = glGetUniformLocation(uiProgram, sName.c_str());
	glUniformMatrix3fv(iLoc, 1, GL_FALSE, (GLfloat*)&mMatrix);
}

// Setting 4x4 matrices

void ShaderProgram::SetUniform(string sName, glm::mat4* mMatrices, int iCount){
	int iLoc = glGetUniformLocation(uiProgram, sName.c_str());
	glUniformMatrix4fv(iLoc, iCount, GL_FALSE, (GLfloat*)mMatrices);
}


void ShaderProgram::SetUniform(string sName, const glm::mat4 mMatrix){
	int iLoc = glGetUniformLocation(uiProgram, sName.c_str());
	glUniformMatrix4fv(iLoc, 1, GL_FALSE, (GLfloat*)&mMatrix);
}

// Setting integers

void ShaderProgram::SetUniform(string sName, int* iValues, int iCount){
	int iLoc = glGetUniformLocation(uiProgram, sName.c_str());
	glUniform1iv(iLoc, iCount, iValues);
}


void ShaderProgram::SetUniform(string sName, const int iValue){
	int iLoc = glGetUniformLocation(uiProgram, sName.c_str());
	glUniform1i(iLoc, iValue);
}


void ShaderProgram::SetModelAndNormalMatrix(string sModelMatrixName, string sNormalMatrixName, glm::mat4 mModelMatrix){
	SetUniform(sModelMatrixName, mModelMatrix);
	SetUniform(sNormalMatrixName, glm::transpose(glm::inverse(mModelMatrix)));
}


void ShaderProgram::SetModelAndNormalMatrix(string sModelMatrixName, string sNormalMatrixName, glm::mat4* mModelMatrix){
	SetUniform(sModelMatrixName, mModelMatrix);
	SetUniform(sNormalMatrixName, glm::transpose(glm::inverse(*mModelMatrix)));
}

void ShaderProgram::SetLightData(Light l, string name){
	SetUniform(name + ".vColor", l.vColor);
	SetUniform(name + ".vDirection", l.vDirection);
	SetUniform(name + ".fAmbient", l.fAmbient);
}
