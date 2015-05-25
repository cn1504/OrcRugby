#version 410

uniform float iGlobalTime;           // shader playback time (in seconds)

uniform mat4 ViewProjectionMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;

layout(location = 0) in vec3 Vertex;

layout(location = 0) out vec3 wsVertex;


const float EPSILON	= 1e-3;

// sea
const int ITER_GEOMETRY = 3;
const int ITER_FRAGMENT = 5;
const float SEA_HEIGHT = 0.3;
const float SEA_CHOPPY = 4.0;
const float SEA_SPEED = 0.8;
const float SEA_FREQ = 0.16;
const vec3 SEA_BASE = vec3(0.1,0.19,0.22);
const vec3 SEA_WATER_COLOR = vec3(0.8,0.9,0.6);
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

/*

const int WAVE_COUNT = 5;
const float WAVE_LENGTH = 20.0;
const float WAVE_AMP = 0.3;
// const float WAVE_SPEED = 1.75464; // sqrt(9.8 * 2 * PI / L)
const float WAVE_STEEPNESS = 0.5;
mat2 octave_m = mat2(1.6,1.2,-1.2,1.6);

#define M_PI 3.1415926535897932384626433832795

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

vec3 Map(vec3 p)
{
	vec2 uv = p.xz;
	uv += noise(uv);
	vec2 Wave_Dir = normalize(vec2(1.0, -1.0));
	float amp = WAVE_AMP;
	float l = WAVE_LENGTH;
	float Qi, d, h = 0.0;
	vec2 uvDelta = vec2(0.0, 0.0);
	float speed = sqrt(9.801 * 2.0 * M_PI / l);
	
	for (int i = 0; i < WAVE_COUNT; i++)
	{
		float freq = 2.0 * M_PI / l;
		Qi = WAVE_STEEPNESS / (freq * amp * WAVE_COUNT);  
		d = freq * dot(Wave_Dir, uv) + speed * freq * iGlobalTime;
		uvDelta.x += Qi * amp * Wave_Dir.x * cos(d);
		uvDelta.y += Qi * amp * Wave_Dir.y * cos(d);
		h += amp * sin(d);
		
		Wave_Dir = normalize(octave_m * Wave_Dir);
		l *= 0.526316; amp *= 0.44;
	}	
	p += vec3(uvDelta.x, h, uvDelta.y);
	return p;
}
*/

void main(void) 
{
	wsVertex = (ModelMatrix * vec4(Vertex, 1.0)).xyz;	
	wsVertex.y = Map(wsVertex);

	gl_Position = (ViewProjectionMatrix) * vec4(wsVertex, 1.0);
}