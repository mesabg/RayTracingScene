#include <Light.h>

Light::Light(){
	vColor = glm::vec3(1.0f, 1.0f, 1.0f);
	vDirection = glm::vec3(0.0f, -1.0f, 0.0f);
	fAmbient = 0.25f;
}


Light::Light(glm::vec3 a_vColor, glm::vec3 a_vDirection, float a_fAmbient){
	vColor = a_vColor;
	vDirection = a_vDirection;
	fAmbient = a_fAmbient;
}