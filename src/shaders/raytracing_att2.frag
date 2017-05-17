//-- RAYTRACING
//-- By: Moisés Berenguer


//-- Declaring structures and constants
#version 330
const float ZMAX = 99999.0;
const float EPSILON = 0.001;
const int MAX_BOUNCES = 5;
const int SPHERES = 3;

struct Color{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Properties{
    bool reflected;
    bool refracted;
    float refractedIndex; 
};

struct Intersection{
	vec3 point;
	float dist;
	vec3 normal;
    Color color;
    Properties properties;
};
	
struct Ray{
	vec3 origin;
	vec3 direction;
};

struct Light{
	vec3 position;
    float radius;
	Color color;
};

struct Sphere{
	vec3 center;
	float radius;
    Color color;
    Properties properties;
};


//-- Some uniform inputs
uniform vec2 unResolution;
uniform vec3 uCameraPosition;
uniform mat4 uCameraMatrix;
uniform mat4 uProjectionMatrix;
uniform float time;

//-- Functions

//-- Generals
float saturate(float f){ return clamp(f,0.0,1.0); }
vec3 saturate(vec3 v){ return clamp(v,vec3(0,0,0),vec3(1,1,1)); }
float rand(vec2 co){ return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453 + time); }


/**
 * Ray - Sphere intersection
 */
Intersection RaySphere(Ray ray, Sphere sphere){
	Intersection intersection;

    //-- Init values
	intersection.dist = ZMAX;
	vec3 center = sphere.center;
	float radius = sphere.radius;
	vec3 epsilon = center - ray.origin;
	float a = dot(epsilon, ray.direction);
	float b = radius*radius - dot(epsilon, epsilon) + a*a;
	if( b > 0.0 ){
		float f = sqrt(b);
		float t = a - f;
		if(t > EPSILON){
            //-- Sphere hit
			intersection.point = ray.origin + ray.direction*t;
			intersection.normal = normalize(intersection.point - center);
			intersection.dist = t;
            intersection.color = sphere.color;
            intersection.properties = sphere.properties;
		}
	}

	return intersection;
}


/**
 * Minimum intersection
 */
Intersection MinIntersection(Intersection a, Intersection b){
	if(a.dist < b.dist) return a;
	else return b;
}


/**
 * Generate Material
 */
vec3 Material(Intersection intersection){
	float value = 0.0;
	value = mod(floor(intersection.point.x) + floor(intersection.point.z), 2.0);
	return vec3(value, value, value)*0.6;
}


/**
-------- Global primary functions --------
**/

/**
------------ Global variables ------------
**/

Light globalLight;
Sphere globalSpheres[SPHERES];


/**
 * Scene intersection
 */
Intersection SceneIntersection(Ray ray){
	Intersection iOut;
	iOut.dist = ZMAX;

	for(int i = 0; i < SPHERES; i++){
		Intersection iSphere = RaySphere(ray, globalSpheres[i]);
		iOut = MinIntersection(iOut, iSphere);
	}
	
	return iOut;
}


/**
 * Calculate Lightning
 */
vec3 CalcLighting(Light light, Intersection intersection, Ray viewRay){
    //-- Variable order
    vec3 color = vec3(0.0);
	vec3 normal = intersection.normal;
	vec3 point = intersection.point;
	vec3 l = normalize(light.position - point);
	vec3 v = normalize(viewRay.origin - point);
	vec3 h = normalize(l+v);

    //-- Ilumination model
	float NdotL = saturate(dot(normal, l));
	float NdotH = saturate(dot(normal, h));
	vec3 diffuse = NdotL * intersection.color.diffuse;
	vec3 specular = pow(NdotH,8.0) * intersection.color.specular;
	float distA = 1.0-saturate(length(light.position - point)/light.radius);
	color = (diffuse + specular) * distA * light.color.ambient;
	
    //-- Calculating shadow (Ray from point to light source)
	float shadow = 1.0;
	Ray shadowRay;
	shadowRay.origin = intersection.point;
	float lightDist = length(light.position - intersection.point);
	shadowRay.direction = (light.position - intersection.point)/lightDist;
	Intersection shadowIntersection = SceneIntersection(shadowRay);
	if(shadowIntersection.dist < lightDist){
        //-- Completely occluded pixel
		shadow = 0.0;
	}
	color *= shadow;
	
    //-- Final color
	return color;
}


/**
 * Get Lightning
 */
vec3 GetLighting(Intersection intersection, Ray viewRay){
    //-- Default color
	vec3 color = vec3(0,0,0);
	color += CalcLighting(globalLight, intersection, viewRay);	
	return color;
}


/**
 * Get Final Color (Color to be rendered on screen)
 */
vec4 GetColor(Ray ray){
    //-- Default color
	vec3 color = vec3(0.0, 0.0, 0.0);
    float alpha = 0.0;

    //-- Some variables
    Intersection intersection;
    vec3 specular;
	vec3 incident;
	
    //-- Iterate througth BOUNCES
    for(int i = 0; i < MAX_BOUNCES; i++){
        //-- Intersect, accumulate color and change ray direction
        intersection = SceneIntersection(ray);
        if(intersection.dist < ZMAX-EPSILON){
            alpha = 1.0;
            color += GetLighting(intersection, ray);
            incident = normalize(intersection.point - ray.origin);

            //-- Ray direction
            if (intersection.properties.reflected){
                //-- Reflect ray
                ray.direction = reflect(incident, intersection.normal);
            }else if (intersection.properties.refracted){
                //-- Refracted ray
                ray.direction = refract(incident, intersection.normal, intersection.properties.refractedIndex);
            }

            ray.origin = intersection.point + ray.direction*EPSILON;
        }else{
            //-- Nothing is hit (Here you can put a dynamic cube map to get a color and make it look great)
            break;
        }
    }

    //-- Return final color
	return vec4(color, alpha);
}


/**
 * Init Ray
 */
Ray InitViewRay(){
    //-- Coordinates
    //-- Viewport to normalized 
    vec2 viewport = gl_FragCoord.xy / unResolution.xy;
    viewport = (2.0 * viewport - 1.0);
    //-- Normalized to clip
    vec4 clip = vec4 (viewport, -1.0, 1.0);
    //-- Clip to eye
    vec4 eye = inverse(uProjectionMatrix)*clip;
    eye = vec4(eye.xy, -1.0, 0.0);
    //-- Eye to world
    vec3 world = normalize((inverse(uCameraMatrix)*eye).xyz); 

    //-- Ray
    Ray ray;

    //-- Generating Ray, Origin ro, direction rd
    ray.origin = uCameraPosition;
    ray.direction = world;

    return ray;
}


/**
 * Init Lights
 */
void InitLights(){
    globalLight.position = /*normalize(vec3(sin(time), cos(time)*sin(time), cos(time)))*/ vec3(10.0, 350.0, 10.0);
    globalLight.radius = 5000.0;
    globalLight.color.ambient = vec3(1.0);
}


/**
 * Init Spheres
 */
void InitSpheres(){
    //-- Define Spheres
    //-- Sphere 0
    globalSpheres[0].center = vec3(-20.0, 250.0, 10.0);
    globalSpheres[0].radius = 60.0;
    globalSpheres[0].color.diffuse = vec3(0.3, 0.0, 0.0);
    globalSpheres[0].color.specular = vec3(1.0);
    globalSpheres[0].properties.reflected = false;
    globalSpheres[0].properties.refracted = true;
    globalSpheres[0].properties.refractedIndex = 0.7;
    

    //-- Sphere 1
    globalSpheres[1].center = vec3(-220.0, 250.0, 10.0);
    globalSpheres[1].radius = 60.0;
    globalSpheres[1].color.diffuse = vec3(0.0, 0.3, 0.0);
    globalSpheres[1].color.specular = vec3(1.0);
    globalSpheres[1].properties.reflected = true;
    globalSpheres[2].properties.refracted = false;
    

    //-- Sphere 2
    globalSpheres[2].center = vec3(-420.0, 250.0, 10.0);
    globalSpheres[2].radius = 60.0;
    globalSpheres[2].color.diffuse = vec3(0.0, 0.0, 0.3);
    globalSpheres[2].color.specular = vec3(0.3, 0.2, 0.1);
    globalSpheres[2].properties.reflected = true;
    globalSpheres[2].properties.refracted = false;

    //-- Moving Spheres
    globalSpheres[0].center.y *= 0.5*cos(time);
    globalSpheres[2].center.y *= 0.2*sin(time);
}


/** Main **/
out vec4 finalColor;
void main(){
    InitSpheres();
    InitLights();
    finalColor = GetColor(InitViewRay());
}