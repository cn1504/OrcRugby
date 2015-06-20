#version 410

uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;
uniform float iGlobalTime;           // shader playback time (in seconds)

layout(location = 0) in vec3 wsVertex;

layout(location = 0) out vec4 outNormal;
layout(location = 1) out vec4 outBase;
layout(location = 2) out vec4 outMSR;

vec4 EncodeNormal (vec3 n)
{
    return vec4(n.xyz * 0.5 + 0.5, 1.0);
}


const float EPSILON	= 1e-3;

// colors
const vec3 SKY_COLOR = vec3(0.439216, 0.576471, 0.72549);
const vec3 SEA_BASE = vec3(0.011765,0.078431,0.105882);
const vec3 SEA_WATER_COLOR = vec3(0.2,0.3,0.4);

// sea
const int ITER_GEOMETRY = 3;
const int ITER_FRAGMENT = 5;
const float SEA_HEIGHT = 0.15;
const float SEA_CHOPPY = 2.0;
const float SEA_SPEED = 0.4;
const float SEA_FREQ = 0.16;
float SEA_TIME = iGlobalTime * SEA_SPEED;
mat2 octave_m = mat2(1.6,1.2,-1.2,1.6);


float hash( vec2 p ) {
	float h = dot(p,vec2(127.1,311.7));	
    return fract(sin(h)*43758.5453123);
}
float noise( vec2 p ) {
    vec2 i = floor( p );
    vec2 f = fract( p );	
	vec2 u = f*f*(3.0-2.0*f);
    return -1.0+2.0*mix( mix( hash( i + vec2(0.0,0.0) ), 
                     hash( i + vec2(1.0,0.0) ), u.x),
                mix( hash( i + vec2(0.0,1.0) ), 
                     hash( i + vec2(1.0,1.0) ), u.x), u.y);
}

float sea_octave(vec2 uv, float choppy) {
    uv += noise(uv);        
    vec2 wv = 1.0-abs(sin(uv));
    vec2 swv = abs(cos(uv));    
    wv = mix(wv,swv,wv);
    return pow(1.0-pow(wv.x * wv.y,0.65),choppy);
}

float Map(vec3 p) {
    float freq = SEA_FREQ;
    float amp = SEA_HEIGHT;
    float choppy = SEA_CHOPPY;
    vec2 uv = p.xz; uv.x *= 0.75;
    
    float d, h = 0.0;    
    for(int i = 0; i < ITER_FRAGMENT; i++) {        
    	d = sea_octave((uv+SEA_TIME)*freq,choppy);
    	d += sea_octave((uv-SEA_TIME)*freq,choppy);
        h += d * amp;        
    	uv *= octave_m; freq *= 1.9; amp *= 0.22;
        choppy = mix(choppy,1.0,0.2);
    }
    return p.y - h;
}

vec3 getNormal(vec3 p, float eps) {
    vec3 n;
    n.y = Map(p);    
    n.x = Map(vec3(p.x+eps,p.y,p.z)) - n.y;
    n.z = Map(vec3(p.x,p.y,p.z+eps)) - n.y;
    n.y = eps;
    return normalize(n);
}


void main() {

	vec3 n = getNormal(wsVertex, EPSILON);
	
	//vec3 n = Map_Normal(wsVertex);
	vec3 viewVertex = (ViewMatrix * vec4(wsVertex, 1.0)).xyz;
	vec3 viewNormal = normalize((ViewMatrix * vec4(n, 0.0)).xyz);
	vec3 eye = normalize(viewVertex);
	
    float fresnel = 1.0 - max(dot(viewNormal,-eye),0.0);
    fresnel = pow(fresnel,2.0);
    vec3 reflected = SKY_COLOR;
    vec3 refracted = SEA_BASE; // + (1.0 - fresnel) * SEA_WATER_COLOR * 0.03; 
	vec3 color = mix(refracted, reflected, fresnel);
	
    //float atten = max(1.0 - dot(viewVertex,viewVertex) * 0.001, 0.0);
    //color += SEA_WATER_COLOR * (wsVertex.y + 2.0 / SEA_HEIGHT) * 0.2 * atten;
	
	outBase = vec4(color, 1.0);//vec4(pow(color,vec3(0.75)), 1.0);
	outNormal = EncodeNormal(viewNormal);
	outMSR = vec4(0.0, 0.2, 0.5, 1.0);
}