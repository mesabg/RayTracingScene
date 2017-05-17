#version 330

struct ColorModel{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    //-- Coefficients
    float shininess;
};

struct Light {
    vec3 color;
    vec3 direction;
    float ambient;
    ColorModel model;
};

struct Sphere{
    vec3 position;
    vec3 normal;
    float radio;
    ColorModel model;
};

// Uniforms
uniform vec2 unResolution;
uniform vec3 uCameraPosition;
uniform vec3 uCameraDirection;
uniform mat4 uCameraMatrix;
uniform mat4 uProjectionMatrix;
uniform float time;
uniform Light uLight;

out vec4 finalColor;

//-- Defining spheres
Sphere spheres[3];

//-- Defining main light
Light mainLight;

//-- Sphere
float iSphere(in vec3 ro, in vec3 rd, in Sphere sphere, inout vec3 normal, inout vec3 material, float closestHit){
    //-- Some mathematics
    vec3 oc = ro - sphere.position; 
    float b = dot(rd, oc);
    float c = dot(oc, oc) - sphere.radio*sphere.radio;
    float h = b*b - c;
    float t = -b - sqrt(abs(h));
    float st = step(0.0, min(t,h)) * step(t, closestHit);

    //-- Update values
    closestHit = mix(closestHit, t, st);
	normal = normalize(sphere.position - (ro + rd * t));
	material = mix(material, sphere.model.diffuse, st);
    return closestHit;
}

//-- Scene intersection
float scene (float t, vec3 ro, vec3 rd, inout vec3 normal, inout vec3 material, float dist){
    //-- Check intersection with 3 spheres
    dist = iSphere(ro, rd, spheres[0], normal, material, dist);
    dist = iSphere(ro, rd, spheres[1], normal, material, dist);
    dist = iSphere(ro, rd, spheres[2], normal, material, dist);
    return dist;
}

//-- Background (Sun Color)
vec3 background(float t, vec3 rd){
    vec3 sunColor = vec3(2.0, 1.6, 1.0);
	vec3 skyColor = vec3(0.5, 0.6, 0.7);
	vec3 sunDir = normalize(vec3(sin(t), cos(t)*sin(t), cos(t)));

    vec3 sunColor2 = vec3(1.0);
	vec3 skyColor2 = vec3(0.5, 0.6, 0.3);
	vec3 sunDir2 = normalize(vec3(-600.0, sin(t), cos(t)));
	return
		pow(max(0.0, dot(sunDir, rd)), 256.0)*sunColor + 
		1.5*pow(max(0.0, dot(sunDir, rd)), 2.0)*sunColor + 
		pow(max(0.0, -dot(vec3(0.0, 1.0, 0.0), rd)), 1.0)*(1.0-skyColor) +
		pow(max(0.0, dot(vec3(0.0, 1.0, 0.0), rd)), 1.0)*skyColor +
        pow(max(0.0, dot(sunDir2, rd)), 128.0)*sunColor2 + 
		2.0*pow(max(0.0, dot(sunDir2, rd)), 2.0)*sunColor2 + 
		pow(max(0.0, -dot(vec3(0.0, 1.0, 0.0), rd)), 1.0)*(1.0-skyColor2) +
		pow(max(0.0, dot(vec3(0.0, 1.0, 0.0), rd)), 1.0)*skyColor2;
}

//-- Some Blinn Phong Lightning
// -- returns intensity of reflected ambient lighting
vec3 ambientLighting(ColorModel light, ColorModel material){
	return material.ambient * light.ambient;
}


// -- returns intensity of diffuse reflection
vec3 diffuseLighting(in vec3 N, in vec3 L, ColorModel light, ColorModel material){
   return max(dot(N, L), 0.0) * material.diffuse * light.diffuse;
}

// -- returns intensity of specular reflection
vec3 specularLightingBlinn(in vec3 N, in vec3 L, in vec3 V, ColorModel light, ColorModel material){
   float specularTerm = 0;

   if(dot(N, L) > 0){
      // -- half vector
      vec3 H = normalize(L + V);
      specularTerm = max(pow(dot(N, H), material.shininess),0.0);
   }

   return specularTerm * material.specular * light.specular;
}

vec3 BlinnPhong(in vec3 N, in vec3 L, in vec3 V, in ColorModel light, in ColorModel material){
    vec3 color = vec3(0.0); 

    color += ambientLighting(light, material);
    color += diffuseLighting(N, L, light, material);
    color += specularLightingBlinn(N, L, V, light, material);

    return color;
}

void main(){
    //-- Define Main light color
    mainLight = uLight;
    mainLight.model.ambient = vec3(1.0);
    mainLight.model.diffuse = vec3(1.0);
    mainLight.model.specular = vec3(1.0);

    //-- Define Spheres
    //-- Sphere 0
    spheres[0].position = vec3(-20.0, 250.0, 10.0);
    spheres[0].normal = vec3(0.0, 0.0, 0.0);
    spheres[0].radio = 60.0;
    spheres[0].model.ambient = vec3(0.0, 0.0, 0.0);
    spheres[0].model.diffuse = vec3(0.3, 0.0, 0.0);
    spheres[0].model.specular = vec3(1.0);
    spheres[0].model.shininess = 0.5;

    //-- Sphere 1
    spheres[1].position = vec3(-220.0, 250.0, 10.0);
    spheres[1].normal = vec3(0.0, 0.0, 0.0);
    spheres[1].radio = 60.0;
    spheres[1].model.ambient = vec3(0.0, 0.0, 0.0);
    spheres[1].model.diffuse = vec3(0.0, 0.3, 0.0);
    spheres[1].model.specular = vec3(1.0);
    spheres[1].model.shininess = 30.5;

    //-- Sphere 2
    spheres[2].position = vec3(-420.0, 250.0, 10.0);
    spheres[2].normal = vec3(0.0, 0.0, 0.0);
    spheres[2].radio = 60.0;
    spheres[2].model.ambient = vec3(0.5, 0.5, 0.5);
    spheres[2].model.diffuse = vec3(0.0, 0.0, 0.3);
    spheres[2].model.specular = vec3(0.3, 0.2, 0.1);
    spheres[2].model.shininess = 10.0;

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

    //-- Moving spheres
    //spheres[0].position.x *= 0.5*sin(time);
    spheres[0].position.y *= 0.5*cos(time);

    //spheres[1].position.y *= sin(time);
    //spheres[1].position.z *= cos(time);

    //spheres[2].position.x *= 0.5*sin(time);
    spheres[2].position.y *= 0.2*sin(time);

    //-- Generating Ray, Origin ro, direction rd
    vec3 ro = uCameraPosition;
    vec3 rd = world;

    //-- Iterations (Intersecting Rays)
    const int count = 6; //-- Ray Iterations
    float epsilon = 0.001;
	float maxDist = 1e5;
    float t = time;
    vec3 transmit = vec3(1.0);
    float alpha = 0.0;
    vec3 color = vec3 (0.0);

    t = time;
    transmit = vec3(1.0);
    ro = uCameraPosition;
    rd = world;
    for (int i=0; i<count; i++) {
        vec3 material = vec3(1.0), normal = vec3(1.0);
        float dist = scene(t, ro, rd, normal, material, maxDist);

        if (dist < maxDist) { //-- Object hitted.
            alpha = 1.0;
            transmit *= material;       
            ro += rd*dist;         
            //rd = refract(rd, normal, 0.32); 
            rd = refract(rd, normal, 0.32);
            ro += rd*epsilon; //-- Reflect Ray Position
        }else{
            color += transmit * background(t,rd);
            break;
        }
    }

    //-- Final Color
    finalColor = vec4(color, alpha);
}