#version 330
//-- GLSL Ray Interaction

const int iterations = 5;
const float maxDist = 1000.0;
const vec3 amb = vec3(1.0);
const float eps = 1e-3;
const int SPHERES = 11;
const int PLANES = 1;

//-- Uniforms
uniform float time;
uniform vec2 unResolution;
uniform vec3 uCameraPosition;
uniform mat4 uCameraMatrix;
uniform mat4 uProjectionMatrix;
uniform float uImALight;

struct Camera
{
	vec3 up, right, forward;
	vec3 position;
};

Camera cam;
vec4 spheres[SPHERES];
vec4 Scolors[SPHERES];
vec2 Smaterials[SPHERES];

vec4 planes[PLANES];
vec4 Pcolors[PLANES];
vec2 Pmaterials[PLANES];

/**
 * Generate Material
 */
vec3 Material()
{
	float value = 0.0;
	value = mod(time, 2.0);
	return vec3(value, value, value)*0.6;
}

float snoise(vec3 uv, float res)
{
	const vec3 s = vec3(1e0, 1e2, 1e4);
	
	uv *= res;
	
	vec3 uv0 = floor(mod(uv, res))*s;
	vec3 uv1 = floor(mod(uv+vec3(1.), res))*s;
	
	vec3 f = fract(uv); f = f*f*(3.0-2.0*f);
	
	vec4 v = vec4(uv0.x+uv0.y+uv0.z, uv1.x+uv0.y+uv0.z,
		      	  uv0.x+uv1.y+uv0.z, uv1.x+uv1.y+uv0.z);
	
	vec4 r = fract(sin(v*1e-3)*1e5);
	float r0 = mix(mix(r.x, r.y, f.x), mix(r.z, r.w, f.x), f.y);
	
	r = fract(sin((v + uv1.z - uv0.z)*1e-3)*1e5);
	float r1 = mix(mix(r.x, r.y, f.x), mix(r.z, r.w, f.x), f.y);
	
	return mix(r0, r1, f.z)*2.-1.;
}

// vec3(-420.0, 250.0, 10.0);
void init(){
    // X Y Z Radius
    spheres[0] = vec4(      2.5,    2.2,    0.6,    0.5);//-- Lights
    spheres[1] = vec4(      -2.5,    2.2,    -1.5,    0.5);

    spheres[2] = vec4(      2.0,    0.5,    0.1,    0.9);//-- Spheres
    spheres[3] = vec4(      0.0,    1.0,    0.1,    0.9);
    spheres[4] = vec4(      -2.0,    1.5,    0.1,    0.9);

	spheres[5] = vec4( 1001.0 ,       0,       0, 1000.0 - 3.55); 
    spheres[6] = vec4(-1001.0 ,       0,       0, 1000.0 - 2.7);
    spheres[7] = vec4(      0,  1001.0 ,       0, 1000.0 - 4.2); 
    spheres[8] = vec4(      0, -1001.0 ,       0, 1000.0 - 1.4);
    spheres[9] = vec4(      0,       0, 1002.0, 1000.0 - 2.05);
	spheres[10] = vec4(      0,       0, -1002.0, 1000.0 );

    //R G B Diffuse
    Scolors[0] = vec4(1.0, 0.8, 0.0,-1.0);
    Scolors[1] = vec4(0.0, 0.1, 0.8,-1.0);

    Scolors[2] = vec4(1.0, 1.0, 1.0, 0.5);
    Scolors[3] = vec4(0.8, 0.1, 0.1, 1.0);
    Scolors[4] = vec4(1.0, 0.5, 0.0, 0.2);

	Scolors[5] = vec4(1.0, 1.0, 1.0, 0.7); //-- Pared
    Scolors[6] = vec4(1.0, 1.0, 1.0, 0.7);
    Scolors[7] = vec4(1.0, 1.0, 1.0, 0.7);
    Scolors[8] = vec4(1.0, 1.0, 1.0, 0.7);
    Scolors[9] = vec4(1.0, 1.0, 1.0, 0.7);
	Scolors[10] = vec4(1.0, 1.0, 1.0, 0.7);

    //Reflection Coeff, Refraction index
    Smaterials[0] = vec2 (0.0, 0.0);
    Smaterials[1] = vec2 (0.0, 0.0);	

    Smaterials[2] = vec2 (0.01, 0.25);	
    Smaterials[3] = vec2 (0.01, 0.0);	
    Smaterials[4] = vec2 (0.5, 0.01);

	Smaterials[5] = vec2 (0.5, 0.0);				
    Smaterials[6] = vec2 (0.5, 0.0);				
    Smaterials[7] = vec2 (0.5, 0.0);				
    Smaterials[8] = vec2 (0.5, 0.0);				
    Smaterials[9] = vec2 (0.5, 0.0);	

	//-- Planes
	planes[0] = vec4(0.0, 1.0, 0.0, 6.0);

	 //R G B Diffuse
    Pcolors[0] = vec4(1.0, 0.8, 0.0, 0.5);

    //Reflection Coeff, Refraction index
    Pmaterials[0] = vec2 (0.8, 0.35);
}

vec3 getRayDir(vec2 fragCoord){
    //-- Viewport to normalized 
    vec2 viewport = fragCoord.xy / unResolution.xy;
    viewport = (2.0 * viewport - 1.0);
    //-- Normalized to clip
    vec4 clip = vec4 (viewport, -1.0, 1.0);
    //-- Clip to eye
    vec4 eye = inverse(uProjectionMatrix)*clip;
    eye = vec4(eye.xy, -1.0, 0.0);
    //-- Eye to world
    vec3 world = normalize((inverse(uCameraMatrix)*eye).xyz); 
    return world;
}

bool intersectSphere(vec3 ro, vec3 rd, vec4 sp, float tm, out float t)
{
    bool r = false;
	vec3 v = ro - sp.xyz;
	float b = dot(v,rd);
	float c = dot(v,v) - sp.w*sp.w;
	t = b*b-c;
    if( t > 0.0 )
    {
        t = -b-sqrt(t);
        r = (t > 0.0) && (t < tm);
    }
    return r;
}

bool intPlane( in vec3 ro, in vec3 rd, in vec4 pl, in float tm, out float t )
{
    t = -(dot(pl.xyz,ro)+pl.w)/dot(pl.xyz,rd);
    return (t > 0.0) && (t < tm);
}

float calcInter(vec3 ro, vec3 rd, out vec4 ob, out vec4 col,out vec2 mat, out int index)
{
	float tm = maxDist;
	float t;

	if(intersectSphere(ro,rd,spheres[0],tm,t)) { ob = spheres[0]; col = Scolors[0]; tm = t; mat = Smaterials[0]; index = 0;}
	if(intersectSphere(ro,rd,spheres[1],tm,t)) { ob = spheres[1]; col = Scolors[1]; tm = t; mat = Smaterials[1]; index = 1;}

	if(intersectSphere(ro,rd,spheres[2],tm,t)) { ob = spheres[2]; col = Scolors[2]; tm = t; mat = Smaterials[2]; index = 2;}
	if(intersectSphere(ro,rd,spheres[3],tm,t)) { ob = spheres[3]; col = Scolors[3]; tm = t; mat = Smaterials[3]; index = 3;}
	if(intersectSphere(ro,rd,spheres[4],tm,t)) { ob = spheres[4]; col = Scolors[4]; tm = t; mat = Smaterials[4]; index = 4;}

	if(intersectSphere(ro,rd,spheres[5],tm,t)) { ob = spheres[5]; col = Scolors[5]; tm = t; mat = Smaterials[5]; index = 5;}
	if(intersectSphere(ro,rd,spheres[6],tm,t)) { ob = spheres[6]; col = Scolors[6]; tm = t; mat = Smaterials[6]; index = 6;}
	if(intersectSphere(ro,rd,spheres[7],tm,t)) { ob = spheres[7]; col = Scolors[7]; tm = t; mat = Smaterials[7]; index = 7;}
	if(intersectSphere(ro,rd,spheres[8],tm,t)) { ob = spheres[8]; col = Scolors[8]; tm = t; mat = Smaterials[8]; index = 8;}
	if(intersectSphere(ro,rd,spheres[9],tm,t)) { ob = spheres[9]; col = Scolors[9]; tm = t; mat = Smaterials[9]; index = 9;}
	if(intersectSphere(ro,rd,spheres[10],tm,t)) { ob = spheres[10]; col = Scolors[10]; tm = t; mat = Smaterials[10]; index = 10;}

	//if(intPlane(ro,rd,planes[0],tm,t)) { ob = planes[0]; col = Pcolors[0]; tm = t; mat = Pmaterials[0]; index = 0;}

	return tm;
}

bool inShadow(vec3 ro,vec3 rd,float d)
{
	float t;
	bool ret = false;

	if(intersectSphere(ro,rd,spheres[2],d,t)){ ret = true; }
	if(intersectSphere(ro,rd,spheres[3],d,t)){ ret = true; }
	if(intersectSphere(ro,rd,spheres[4],d,t)){ ret = true; }

	if(intersectSphere(ro,rd,spheres[5],d,t)) { ret = true;}
	if(intersectSphere(ro,rd,spheres[6],d,t)) { ret = true;}
	if(intersectSphere(ro,rd,spheres[7],d,t)) { ret = true;}
	if(intersectSphere(ro,rd,spheres[8],d,t)) { ret = true;}
	if(intersectSphere(ro,rd,spheres[9],d,t)) { ret = true;}
	if(intersectSphere(ro,rd,spheres[10],d,t)) { ret = true;}

	//if(intPlane(ro,rd,planes[0],d,t)){ ret = true; }
	return ret;
}

vec3 calcShade(vec3 pt, vec4 ob, vec4 col,vec2 mat,vec3 n)
{

	float dist,diff;
	vec3 lcol,l;

	vec3 color = vec3(0.0);
	vec3 ambcol = amb * (1.0-col.w) * col.rgb;
	vec3 scol = col.w * col.rgb;

	if(col.w > 0.0) //If its not a light
	{
		l = spheres[0].xyz - pt;
		dist = length(l);
		l = normalize(l);
		lcol = Scolors[0].rgb;
		diff = clamp(dot(n,l),0.0,1.0);
		color += (ambcol * lcol + lcol * diff * scol) / (1.0+dist*dist);
		if(inShadow(pt,l,dist))
			color *= 5;

		l = spheres[1].xyz - pt;
		dist = length(l);
		l = normalize(l);
		lcol = Scolors[1].rgb;
		diff = clamp(dot(n,l),0.0,1.0);
		color += (ambcol * lcol + lcol * diff * scol) / (1.0+dist*dist);

		if(inShadow(pt,l,dist))
			color *= 6;

        if (uImALight == 1.0){
            l = uCameraPosition - pt;
            dist = length(l);
            l = normalize(l);
            lcol = vec3(1.0);
            diff = clamp(dot(n,l),0.0,1.0);
            color += (ambcol * lcol + lcol * diff * scol) / (1.0+dist*dist);
            if(inShadow(pt,l,dist))
                color *= 10;
        }
	}
	else
		color = col.rgb;

	return color;
}

float getFresnel(vec3 n,vec3 rd,float r0)
{
    float ndotv = clamp(dot(n, -rd), 0.0, 1.0);
	return r0 + (1.0 - r0) * pow(1.0 - ndotv, 5.0);
}

vec3 getReflection(vec3 ro,vec3 rd)
{
	vec3 color = vec3(0);
	vec4 ob,col;
    vec2 mat;
    int index;
	float tm = calcInter(ro,rd,ob,col,mat, index);
	if(tm < maxDist)
	{
		vec3 pt = ro + rd*tm;
		vec3 n = normalize(pt - ob.xyz);
		color = calcShade(pt,ob,col,mat,n);
	}
	return color;
}

void rotObjects()
{
	spheres[0].x += sin(time) * 0.4;
    spheres[0].z += cos(time) * 0.4;
    
    spheres[1].x += sin(time) * -0.3;
    spheres[1].z += cos(time) * -0.3;

    spheres[3].z += sin(time) * -0.3;
    spheres[3].y += cos(time) * -0.3;
}

vec4 mainImage( in vec2 fragCoord ){
    init();
	float fresnel,tm;
	vec4 ob,col;
    vec2 mat;
	vec3 pt,refCol,n,refl;

	vec3 mask = vec3(1.0);
	vec3 color = vec3(0.0);
	vec3 ro = uCameraPosition;
	vec3 rd = getRayDir(fragCoord);
    float alpha = 1.0;
    int index;

    rotObjects();
	
	for(int i = 0; i < iterations; i++)
	{
		tm = calcInter(ro,rd,ob,col,mat,index);
		if(tm < maxDist)
		{
			//color = vec3(1.0);
			
			pt = ro + rd*tm;
			n = normalize(pt - ob.xyz);
			fresnel = getFresnel(n,rd,mat.x);
			mask *= fresnel;
            if (ro == uCameraPosition && index >= 5) alpha = 0.0; //-- If it is wall
            

			if(mat.y > 0.0) // Refractive
			{
				ro = pt - n*eps;
				refl = reflect(rd,n);
				refCol = getReflection(ro, refl);
				color += refCol * mask;
				mask = col.rgb * (1.0 - fresnel) * (mask / fresnel);
				rd = refract(rd, n, mat.y);
			}
			else if(mat.x > 0.0) // Reflective
			{
				color += calcShade(pt,ob,col,mat,n) * (1.0 - fresnel) * mask / fresnel;
				ro = pt + n*eps;
				rd = reflect(rd, n);
			}
			else // Diffuse
            {
				color += calcShade(pt,ob,col,mat,n) * mask/fresnel;
                break;
            }

            
		}
		else{
			//if (ro == uCameraPosition) alpha = 0.0;
			//alpha = 0.0;
		}
	}
	return vec4(color, alpha);
}

out vec4 fragColor;
void main(){
    fragColor = mainImage(gl_FragCoord.xy);
}