#include <SideBar.h>
#include <iostream>

SideBar* SideBar::uniqueSideBar = NULL;

SideBar::SideBar(RenderController* renderController, const int width, const int height) :RenderColleague(renderController) {
	if (!TwInit(TW_OPENGL, NULL)) {
		std::cout << "Side Bar Not Initiated\n";
		exit(1);
	}

	//-- Initialize principal Bar
	this->principalPropertiesBar = TwNewBar("Principal");

	TwDefine("Principal refresh = '0.0001f'");
	TwDefine("Principal resizable = false");
	TwDefine("Principal fontresizable = false");
	TwDefine("Principal movable = false");
	TwDefine("Principal visible = true");
	TwDefine("Principal position = '20 20'");
	TwDefine("Principal size = '280 160'");

	this->showShadowMap = new bool;
	*(this->showShadowMap) = true;
}

SideBar::SideBar(RenderController* renderController):RenderColleague(renderController) {
	if (!TwInit(TW_OPENGL, NULL)) {
		std::cout << "Side Bar Not Initiated\n";
		exit(1);
	}

	//-- Initialize show type
	this->showType = 0;

	//-- Prevent default
	this->model = NULL;

	//-- Initialize principal Bar
	this->principalPropertiesBar = TwNewBar("Principal");

	TwDefine("Principal refresh = '0.0001f'");
	TwDefine("Principal resizable = false");
	TwDefine("Principal fontresizable = false");
	TwDefine("Principal movable = false");
	TwDefine("Principal visible = true");
	TwDefine("Principal position = '20 20'");
	TwDefine("Principal size = '280 190'");
	this->showShadowMap = new bool;
	*(this->showShadowMap) = true;

}

SideBar::~SideBar() {
	TwTerminate();
}

void SideBar::clear() {
	TwDefine("Principal visible = false");
}

void SideBar::principalView() {
	TwDefine("Principal visible = true");
}

void SideBar::modelView() {
	TwDefine("Model visible = true");
}

void SideBar::lightView() {
	TwDefine("Light visible = true");
}

SideBar* SideBar::Instance(RenderController* renderController) {
	if (!uniqueSideBar)
		uniqueSideBar = new SideBar(renderController);
	return uniqueSideBar;
}

SideBar * SideBar::Instance(RenderController * renderController, const int width, const int height){
	if (!uniqueSideBar)
		uniqueSideBar = new SideBar(renderController, width, height);
	return uniqueSideBar;
}

void SideBar::Destroy(){
	if (!uniqueSideBar) return;
	uniqueSideBar->~SideBar();
	delete uniqueSideBar;
}

void SideBar::Notify(string message, void* data) {
	if (message == "showShadowMap")
		this->showShadowMap = (bool*)data;
}

void SideBar::setModel(Model * model){
	this->model = model;
}

void SideBar::setSelectModelTarget(float *target){
	this->selectedModel = target;
	std::cout << this->selectedModel << std::endl;
}

void SideBar::setSelectLightTarget(float *target){
	this->selectedLight = target;
}

void SideBar::reshape(int gWidth, int gHeight) {
	TwWindowSize(gWidth, gHeight);
}

void SideBar::recalculateTransformationMatrices(){
	this->model->getTransformation()->updateTraslationMatrix();
	this->model->getTransformation()->updateRotationMatrixQuaternion();
	this->model->getTransformation()->updateScaleMatrix();

	if (*(this->model->getShadingVectorBitMapXCoord_phong()) && *(this->model->getShadingVectorBitMapYCoord_blinn())) {
		*(this->model->getShadingVectorBitMapYCoord_blinn()) = true;
		*(this->model->getShadingVectorBitMapXCoord_phong()) = false;
	}

	if ( *(this->model->getLightningTypeX_frag()) && *(this->model->getLightningTypeY_vert()) ) {
		*(this->model->getLightningTypeX_frag()) = true;
		*(this->model->getLightningTypeY_vert()) = false;
	}
}

void SideBar::bindPrincipal(){
	//-- Principal Bar
	TwAddVarRW(this->principalPropertiesBar, "ModelSelect", TW_TYPE_FLOAT, this->selectedModel, (" min=-1 max=" + std::to_string(amountOfEntities-1) + " step=1 group='Select a model' label='Model index' ").c_str());
	TwAddButton(this->principalPropertiesBar, "SelectModelButton", selectModelCallback, (void*)this, " label='Select model' group='Select a model' help='Select a model by index'");
	TwAddSeparator(this->principalPropertiesBar, "OptionsSeparator[0]", "");
	TwAddVarRW(this->principalPropertiesBar, "LightSelect", TW_TYPE_FLOAT, this->selectedLight, (" min=-1 max=" + std::to_string(amountOfLights-1) + " step=1 group='Select a light' label='Light index' ").c_str());
	TwAddButton(this->principalPropertiesBar, "SelectLightButton", selectLightCallback, (void*)this, " label='Select light' group='Select a light' help='Select a light by index'");

	//TwAddButton(this->principalPropertiesBar, "Animate", animateCallback, (void*)this, " label='Turn On/Off' group='Animation' help='Turn On/Off animation'");
	TwAddVarRW(this->principalPropertiesBar, "Show Shadow Map", TW_TYPE_BOOLCPP, this->showShadowMap, "group='Shadows' label='Show Shadow Map' opened=true help='Turn On/Off Shadow Map.' ");
}

void SideBar::bindModel() {
	//-- Model Bar initializing
	this->modelPropertiesBar = TwNewBar("Model");
	TwDefine("Model visible = false");

	TwDefine("Model refresh = '0.0001f'");
	TwDefine("Model resizable = false");
	TwDefine("Model fontresizable = false");
	TwDefine("Model movable = false");
	TwDefine("Model visible = true");
	TwDefine("Model position = '20 20'");
	TwDefine("Model size = '280 850'");

	//-- Lightning Type
	//TwAddVarRW(this->modelPropertiesBar, "SelectPerFragmentLightning", TW_TYPE_BOOLCPP, this->model->getLightningTypeX_frag(), "group='Lightning' label='Per Fragment Lightning' help='Select per fragment lightning shader.'");
	//TwAddVarRW(this->modelPropertiesBar, "SelectPerVertexLightning", TW_TYPE_BOOLCPP, this->model->getLightningTypeY_vert(), "group='Lightning' label='Per Vertex Lightning' help='Select per vertex lightning shader.'");

	//-- Select shading technic to use
	//TwAddVarRW(this->modelPropertiesBar, "SelectPhong", TW_TYPE_BOOLCPP, this->model->getShadingVectorBitMapXCoord_phong(), "group='Shading' label='Phong' help='Use Phong shading technic.'");
	//TwAddVarRW(this->modelPropertiesBar, "SelectBlinnPhong", TW_TYPE_BOOLCPP, this->model->getShadingVectorBitMapYCoord_blinn(), "group='Shading' label='Blinn-Phong' help='Use Blinn-Phong shading technic.'");
	//TwAddVarRW(this->modelPropertiesBar, "SelectOrenNayar", TW_TYPE_BOOLCPP, this->model->getShadingVectorBitMapZCoord_oren(), "group='Shading' label='Oren Nayar' help='Use Oren Nayar shading technic.'");
	//TwAddVarRW(this->modelPropertiesBar, "SelectCookTorrance", TW_TYPE_BOOLCPP, this->model->getShadingVectorBitMapWCoord_cook(), "group='Shading' label='Cook Torrance' help='Use Cook Torrance shading technic.'");


	//-- Transformations
	TwAddVarRW(this->modelPropertiesBar, "TranslationSpeedX", TW_TYPE_FLOAT, this->model->getTransformation()->getTraslationValueX(), " min=-10000 max=10000 step=0.01 group='Translation' label='Translation factor X' ");
	TwAddVarRW(this->modelPropertiesBar, "TranslationSpeedY", TW_TYPE_FLOAT, this->model->getTransformation()->getTraslationValueY(), " min=-10000 max=10000 step=0.01 group='Translation' label='Translation factor Y' ");
	TwAddVarRW(this->modelPropertiesBar, "TranslationSpeedZ", TW_TYPE_FLOAT, this->model->getTransformation()->getTraslationValueZ(), " min=-10000 max=10000 step=0.01 group='Translation' label='Translation factor Z' ");
	TwAddVarRW(this->modelPropertiesBar, "Rotation", TW_TYPE_QUAT4F, this->model->getTransformation()->getQuaternion(), " label='Rotation' opened=true help='Change the model orientation.' ");
	TwAddVarRW(this->modelPropertiesBar, "Scale", TW_TYPE_FLOAT, this->model->getTransformation()->getScaleValue() , " min=0.0 max=100000.0 step=0.001 group='Scale' label='Scale All'  ");
	TwDefine(" Model/Transforms help='Apply transforms to the selected model.' ");
	TwDefine(" Model/Translation group=Transforms help='Change the model translations.'");
	TwDefine(" Model/Rotation group=Transforms ");
	TwDefine(" Model/Scale group=Transforms help='Change the model scale.' ");

	//-- Material Colors
	TwAddVarRW(this->modelPropertiesBar, "MatAmbientReflectances", TW_TYPE_COLOR3F, this->model->getLight()->getAmbient(), "group='Material Colors' label='Ambient Reflectances' opened=true help='Change the material ambient reflectances.' ");
	TwAddVarRW(this->modelPropertiesBar, "MatDiffuseReflectances", TW_TYPE_COLOR3F, this->model->getLight()->getDiffuse(), "group='Material Colors' label='Diffuse Reflectances' opened=true help='Change the material diffuse reflectances.' ");
	TwAddVarRW(this->modelPropertiesBar, "MatSpecularReflectances", TW_TYPE_COLOR3F, this->model->getLight()->getSpecular(), "group='Material Colors' label='Specular Reflactances' opened=true help='Change the material specular reflectances.' ");

	//-- Light Properties
	TwAddVarRW(this->modelPropertiesBar, "MatShininess", TW_TYPE_FLOAT, this->model->getMaterialProperties()->getShininess(), " min=0.01 max=0.99 step=0.01 group='Light Properties' label='Shininess factor' help='Change the shininess factor.'");
	//TwAddVarRW(this->modelPropertiesBar, "MatRoughness", TW_TYPE_FLOAT, this->model->getMaterialProperties()->getRoughness(), " min=0.1 max=100.0 step=0.01 group='Light Properties' label='Roughness factor' help='Change the roughness factor.'");
	//TwAddVarRW(this->modelPropertiesBar, "MatFresnel", TW_TYPE_FLOAT, this->model->getMaterialProperties()->getFresnel(), " min=0.1 max=100.0 step=0.01 group='Light Properties' label='Fresnel factor' help='Change the fresnel factor.'");
	//TwAddVarRW(this->modelPropertiesBar, "MatAlbedo", TW_TYPE_FLOAT, this->model->getMaterialProperties()->getAlbedo(), " min=0.1 max=100.0 step=0.01 group='Light Properties' label='Albedo factor' help='Change the albedo factor.'");

	//-- Separator
	TwAddSeparator(this->modelPropertiesBar, "OptionsSeparator[7]", "");

	TwAddVarRW(this->modelPropertiesBar, "Reflected", TW_TYPE_BOOLCPP, this->model->getIsReflected(), "group='Reflections/Refractions' label='Reflection' opened=true help='Turn On/Off the reflection.' ");
	TwAddVarRW(this->modelPropertiesBar, "Refracted", TW_TYPE_BOOLCPP, this->model->getIsRefracted(), "group='Reflections/Refractions' label='Refraction' opened=true help='Turn On/Off the refraction.' ");
	TwAddVarRW(this->modelPropertiesBar, "RefractionFactor", TW_TYPE_FLOAT, this->model->getRefractedIndex(), " min=0.01 max=0.99 step=0.01 group='Reflections/Refractions' label='Refraction Factor' help='Change the refraction factor.'");

	//-- Separator
	TwAddSeparator(this->modelPropertiesBar, "OptionsSeparator[7]", "");
	
	//-- Normal and parallax mapping
	if (this->model->getRoutes()->isNormalMapped) {
		//TwAddVarRW(this->modelPropertiesBar, "None", TW_TYPE_BOOLCPP, this->model->getIsRefracted(), "group='Reflections/Refractions' label='Refraction' opened=true help='Turn On/Off the refraction.' ");
		TwAddVarRW(this->modelPropertiesBar, "Normal", TW_TYPE_BOOLCPP, this->model->getIsNormalMapped(), "group='Texture Mapping' label='Normal Mapping' opened=true help='Turn On/Off the normal mapped.' ");
		TwAddVarRW(this->modelPropertiesBar, "Parallax", TW_TYPE_BOOLCPP, this->model->getIsParallaxMapped(), "group='Texture Mapping' label='Parallax Mapping' opened=true help='Turn On/Off the parallax mapped.' ");
		TwAddVarRW(this->modelPropertiesBar, "ParallaxHeight", TW_TYPE_FLOAT, this->model->getParallaxMapHeight(), " min=0.01 max=400.0 step=0.01 group='Texture Mapping' label='Parallax Height' help='Change the height parallax factor.'");
	}

	//-- Activate textures
	TwAddVarRW(this->modelPropertiesBar, "TextureSwitch", TW_TYPE_BOOLCPP, this->model->getTexture()->isActive(), "group='Texture Switch' label='Turn On/Off' opened=true help='Turn On/Off the texture.' ");

	//-- Separator
	TwAddSeparator(this->modelPropertiesBar, "OptionsSeparator[2]", "");

	//-- Done Button
	TwAddButton(this->modelPropertiesBar, "DoneButton", doneCallback, (void*)this, " label='Done' help='Modification done'");
}


void SideBar::bindLight() {
	//-- Light Bar initializing
	this->lightPropertiesBar = TwNewBar("Light");
	TwDefine("Light visible = false");

	TwDefine("Light refresh = '0.0001f'");
	TwDefine("Light resizable = false");
	TwDefine("Light fontresizable = false");
	TwDefine("Light movable = false");
	TwDefine("Light visible = true");
	TwDefine("Light position = '20 20'");
	TwDefine("Light size = '280 850'");

	//-- Transformations
	TwAddVarRW(this->lightPropertiesBar, "TranslationSpeedXLight", TW_TYPE_FLOAT, this->model->getLight()->getPositionX(), " min=-10000 max=10000 step=0.01 group='Translation' label='Translation factor X' ");
	TwAddVarRW(this->lightPropertiesBar, "TranslationSpeedYLight", TW_TYPE_FLOAT, this->model->getLight()->getPositionY(), " min=-10000 max=10000 step=0.01 group='Translation' label='Translation factor Y' ");
	TwAddVarRW(this->lightPropertiesBar, "TranslationSpeedZLight", TW_TYPE_FLOAT, this->model->getLight()->getPositionZ(), " min=-10000 max=10000 step=0.01 group='Translation' label='Translation factor Z' ");

	//-- Colors
	TwAddVarRW(this->lightPropertiesBar, "LightAmbientReflectances", TW_TYPE_COLOR3F, this->model->getLight()->getAmbient(), "group='Light Colors' label='Ambient Reflectances' opened=true help='Change the light ambient reflectances.' ");
	TwAddVarRW(this->lightPropertiesBar, "LightDiffuseReflectances", TW_TYPE_COLOR3F, this->model->getLight()->getDiffuse(), "group='Light Colors' label='Diffuse Reflectances' opened=true help='Change the light diffuse reflectances.' ");
	TwAddVarRW(this->lightPropertiesBar, "LightSpecularReflectances", TW_TYPE_COLOR3F, this->model->getLight()->getSpecular(), "group='Light Colors' label='Specular Reflactances' opened=true help='Change the light specular reflectances.' ");

	//-- Light Attenuation
	TwAddVarRW(this->lightPropertiesBar, "AttenuationSpeedXLight", TW_TYPE_FLOAT, this->model->getLight()->getAttenuationX(), " min=0.01 max=0.99 step=0.01 group='Attenuation' label='Constant' ");
	TwAddVarRW(this->lightPropertiesBar, "AttenuationSpeedYLight", TW_TYPE_FLOAT, this->model->getLight()->getAttenuationY(), " min=0.01 max=0.99 step=0.01 group='Attenuation' label='Linear' ");
	TwAddVarRW(this->lightPropertiesBar, "AttenuationSpeedZLight", TW_TYPE_FLOAT, this->model->getLight()->getAttenuationZ(), " min=0.01 max=0.99 step=0.01 group='Attenuation' label='Cuadratic' ");

	

	//-- Separator
	TwAddSeparator(this->lightPropertiesBar, "OptionsSeparator[3]", "");

	//-- Activate textures
	TwAddVarRW(this->lightPropertiesBar, "LightSwitch", TW_TYPE_BOOLCPP, this->model->getLight()->active(), "group='Light Switch' label='Turn On/Off' opened=true help='Turn On/Off the light.' ");

	//-- Light Type
	TwAddVarRW(this->lightPropertiesBar, "LightTypeDirectional", TW_TYPE_BOOLCPP, this->model->getLight()->getTypeXDirectional(), "group='Light Type' label='Directional' opened=true help='Turn On/Off the directional light.' ");
	TwAddVarRW(this->lightPropertiesBar, "LightTypePoint", TW_TYPE_BOOLCPP, this->model->getLight()->getTypeYPoint(), "group='Light Type' label='Point' opened=true help='Turn On/Off the point light.' ");
	TwAddVarRW(this->lightPropertiesBar, "LightTypeSpot", TW_TYPE_BOOLCPP, this->model->getLight()->getTypeZSpot(), "group='Light Type' label='Spot' opened=true help='Turn On/Off the spot light.' ");

	//-- Spotlight
	TwAddVarRW(this->lightPropertiesBar, "SpotVector", TW_TYPE_DIR3F, this->model->getLight()->getDirection(), " group='Spotlight' opened=true label='Direction' help='Change the light direction.'");
	TwAddVarRW(this->lightPropertiesBar, "SpotExp", TW_TYPE_FLOAT, this->model->getLight()->getSpotExp(), " min=0.01 max=20.0 step=0.0001 group='Spotlight' label='Spot Exp' ");
	TwAddVarRW(this->lightPropertiesBar, "SpotCosCutOff", TW_TYPE_FLOAT, this->model->getLight()->getSpotCosCutOff(), " min=-1.0 max1.0 step=0.01 group='Spotlight' opened=true label='Inner Cosine' help='Change the light Cut Off factor.'");
	TwAddVarRW(this->lightPropertiesBar, "SpotCosOuterCutOff", TW_TYPE_FLOAT, this->model->getLight()->getSpotCosOuterCutOff(), " min=-1.0 max1.0 step=0.01 group='Spotlight' opened=true label='Outer Cosine' help='Change the light Outer Cut Off factor.'");

	//-- Separator
	TwAddSeparator(this->lightPropertiesBar, "OptionsSeparator[4]", "");

	//-- Done Button
	TwAddButton(this->lightPropertiesBar, "DoneButton", doneCallback, (void*)this, " label='Done' help='Modification done'");
}

void SideBar::bindLightWithModel(){
	this->model->getTransformation()->setTraslationMatrix( *(this->model->getLight()->getPosition()) );
}

void SideBar::recalculateLight(){
	if (*(this->model->getIsNormalMapped()))
		*(this->model->getIsParallaxMapped()) = false;

	if (*(this->model->getIsParallaxMapped()))
		*(this->model->getIsNormalMapped()) = false;
}


void SideBar::update() {
	//-- Select view to be rendered
	this->clear();
	if (this->showType == 0) {
		this->principalView();
		this->updatePrincipal();
	}else if (this->showType == 1) {
		this->modelView();
		this->recalculateTransformationMatrices();
	}
	else if (this->showType == 2) {
		this->bindLightWithModel();
		this->lightView();
		this->recalculateLight();
	}

	//-- Update Screen
	TwDraw();
}

void SideBar::selectModelCall() {
	*(this->selectedLight) = -1.0f;
	if (*(this->selectedModel) != -1.0f) {
		this->Send("SendSelectedModelProperties", NULL);
		this->bindModel();
		this->showType = 1;
	}
}

void SideBar::selectLightCall(){
	*(this->selectedModel) = -1.0f;
	if (*(this->selectedLight) != -1.0f) {
		this->Send("SendSelectedLightProperties", NULL);
		this->bindLight();
		this->showType = 2;
	}
}

void SideBar::doneCall(){
	*(this->selectedModel) = -1.0f;
	*(this->selectedLight) = -1.0f;
	this->showType = 0;
	TwDeleteBar(this->modelPropertiesBar);
	TwDeleteBar(this->lightPropertiesBar);
}

void SideBar::textureCall(){
	*(this->model->getTexture()->isActive()) = !*(this->model->getTexture()->isActive());
}

void SideBar::selectPhongCall(){
	this->model->getShadingVectorBitMap()->x = !this->model->getShadingVectorBitMap()->x;
	this->model->getShadingVectorBitMap()->y = 0.0f;
}

void SideBar::selectBlinnPhongCall(){
	this->model->getShadingVectorBitMap()->y = !this->model->getShadingVectorBitMap()->y;
	this->model->getShadingVectorBitMap()->x = 0.0f;
}

void SideBar::selectOrenNayarCall(){
	this->model->getShadingVectorBitMap()->z = !this->model->getShadingVectorBitMap()->z;
}

void SideBar::selectCookTorranceCall(){
	this->model->getShadingVectorBitMap()->w = !this->model->getShadingVectorBitMap()->w;
}

void SideBar::selectPerFragmentLightningCall(){
	this->model->getLightningType()->x = !this->model->getLightningType()->x;
	this->model->getLightningType()->y = 0;
}

void SideBar::selectPerVertexLightningCall(){
	this->model->getLightningType()->y = !this->model->getLightningType()->y;
	this->model->getLightningType()->x = 0;
}

void SideBar::activeLightsCall(){

}

void SideBar::animateCall(){
	this->Send("Animate", NULL);
}

void SideBar::updatePrincipal(){
	this->Send("updateSetShadowMap", this->showShadowMap);
}


//-- Tweakbar callbacks
void TW_CALL selectModelCallback(void * clientData) {
	SideBar* localSideBar = (SideBar*)clientData;
	localSideBar->selectModelCall();
}

void TW_CALL selectLightCallback(void * clientData){
	SideBar* localSideBar = (SideBar*)clientData;
	localSideBar->selectLightCall();
}

void TW_CALL doneCallback(void * clientData){
	SideBar* localSideBar = (SideBar*)clientData;
	localSideBar->doneCall();
}

void TW_CALL textureCallback(void * clientData){
	SideBar* localSideBar = (SideBar*)clientData;
	localSideBar->textureCall();
}

void TW_CALL selectPhongCallback(void * clientData){
	SideBar* localSideBar = (SideBar*)clientData;
	localSideBar->selectPhongCall();
}

void TW_CALL selectBlinnPhongCallback(void * clientData){
	SideBar* localSideBar = (SideBar*)clientData;
	localSideBar->selectBlinnPhongCall();
}

void TW_CALL selectOrenNayarCallback(void * clientData){
	SideBar* localSideBar = (SideBar*)clientData;
	localSideBar->selectOrenNayarCall();
}

void TW_CALL selectCookTorranceCallback(void * clientData){
	SideBar* localSideBar = (SideBar*)clientData;
	localSideBar->selectCookTorranceCall();
}

void TW_CALL selectPerFragmentLightningCallback(void * clientData){
	SideBar* localSideBar = (SideBar*)clientData;
	localSideBar->selectPerFragmentLightningCall();
}

void TW_CALL selectPerVertexLightningCallback(void * clientData){
	SideBar* localSideBar = (SideBar*)clientData;
	localSideBar->selectPerVertexLightningCall();
}

void TW_CALL activeLightsCallback(void * clientData){
	SideBar* localSideBar = (SideBar*)clientData;
	localSideBar->activeLightsCall();
}

void TW_CALL animateCallback(void * clientData){
	SideBar* localSideBar = (SideBar*)clientData;
	localSideBar->animateCall();
}
