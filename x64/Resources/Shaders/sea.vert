#version 430

uniform float iGlobalTime;           // shader playback time (in seconds)

uniform mat4 ViewProjectionMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;

layout(location = 0) in vec3 Vertex;

layout(location = 0) out vec3 wsVertex;
layout(location = 1) out vec3 wsNormal;

#define M_PI 3.1415926535897932384626433832795

struct Wave_Params
{
	vec2 Direction; 
	float Amplitude;
	float k;
	float Velocity;
};

void GerstnerWave(vec2 pos, Wave_Params w, float t, 
	out vec3 pos_sum, out vec3 norm_sum)
{
	float d = w.k * (dot(w.Direction, pos) - w.Velocity * t);
	float sin_d = sin(d);
	float cos_d = cos(d);
	pos_sum.xz += w.Direction * w.Amplitude * sin_d;
	pos_sum.y += w.Amplitude * cos_d;
	norm_sum.xz += w.k * w.Direction * w.Amplitude * sin_d;
    norm_sum.y += w.k * w.Amplitude * cos_d;	
}

// Sea Constants
const vec2 MEDIAN_DIRECTION = vec2(-0.707107, 0.707107);		// Wind towards shoreline of starting town off the sea
const float MEDIAN_WAVELENGTH = 4.75117 * 4.0;					// In meters, calculated from half the velocity of a 7 knots "gentle breeze" average wind speed
const float MAXIMUM_AMPLITUDE = 1.0 / 7.0 * MEDIAN_WAVELENGTH;	// In meters, calculated maximum amplitude before waves begin to break
const float SEA_DEPTH = 4.0;									// In meters

void Map(inout vec3 pos, out vec3 normal) 
{	
	float t = iGlobalTime;
	vec3 pos_sum = vec3(0.0, 0.0, 0.0);
	vec3 norm_sum = vec3(0.0, 0.0, 0.0);
	const vec2 D = MEDIAN_DIRECTION;
	
	const int WAVE_COUNT = 8;
	Wave_Params w[WAVE_COUNT];
	float costheta = 1.0;
	float sintheta = 0.0;
	w[0].Direction = vec2(D.x * costheta - D.y * sintheta, D.x * sintheta + D.y * costheta);
	w[0].Amplitude = MAXIMUM_AMPLITUDE;
	w[0].k = 2.0 * M_PI / MEDIAN_WAVELENGTH;
	w[0].Velocity = sqrt(9.8 * w[0].k); 	
	
	costheta = 0.9;
	sintheta = 0.43589;
	w[1].Direction = vec2(D.x * costheta - D.y * sintheta, D.x * sintheta + D.y * costheta);
	w[1].Amplitude = MAXIMUM_AMPLITUDE * 0.7;
	w[1].k = 2.0 * M_PI / (MEDIAN_WAVELENGTH * 0.7);
	w[1].Velocity = sqrt(9.8 * w[1].k); 

	costheta = 0.9;
	sintheta = -0.43589;
	w[2].Direction = vec2(D.x * costheta - D.y * sintheta, D.x * sintheta + D.y * costheta);
	w[2].Amplitude = MAXIMUM_AMPLITUDE * 0.72;
	w[2].k = 2.0 * M_PI / (MEDIAN_WAVELENGTH * 0.72);
	w[2].Velocity = sqrt(9.8 * w[2].k); 	

	costheta = 0.95;
	sintheta = -0.31225;
	w[3].Direction = vec2(D.x * costheta - D.y * sintheta, D.x * sintheta + D.y * costheta);
	w[3].Amplitude = MAXIMUM_AMPLITUDE * 0.93;
	w[3].k = 2.0 * M_PI / (MEDIAN_WAVELENGTH * 0.93);
	w[3].Velocity = sqrt(9.8 * w[3].k); 		
	
	costheta = 0.95;
	sintheta = 0.31225;
	w[4].Direction = vec2(D.x * costheta - D.y * sintheta, D.x * sintheta + D.y * costheta);
	w[4].Amplitude = MAXIMUM_AMPLITUDE * 0.91;
	w[4].k = 2.0 * M_PI / (MEDIAN_WAVELENGTH * 0.91);
	w[4].Velocity = sqrt(9.8 * w[4].k); 	
	
	costheta = 0.9925;
	sintheta = -0.122245;
	w[5].Direction = vec2(D.x * costheta - D.y * sintheta, D.x * sintheta + D.y * costheta);
	w[5].Amplitude = MAXIMUM_AMPLITUDE * 0.87;
	w[5].k = 2.0 * M_PI / (MEDIAN_WAVELENGTH * 0.87);
	w[5].Velocity = sqrt(9.8 * w[5].k); 
	
	costheta = 0.8;
	sintheta = -0.6;
	w[6].Direction = vec2(D.x * costheta - D.y * sintheta, D.x * sintheta + D.y * costheta);
	w[6].Amplitude = MAXIMUM_AMPLITUDE * 0.5;
	w[6].k = 2.0 * M_PI / (MEDIAN_WAVELENGTH * 0.5);
	w[6].Velocity = sqrt(9.8 * w[6].k); 
	
	costheta = 0.8;
	sintheta = 0.6;
	w[7].Direction = vec2(D.x * costheta - D.y * sintheta, D.x * sintheta + D.y * costheta);
	w[7].Amplitude = MAXIMUM_AMPLITUDE * 0.53;
	w[7].k = 2.0 * M_PI / (MEDIAN_WAVELENGTH * 0.53);
	w[7].Velocity = sqrt(9.8 * w[7].k); 
	
	for (int i = 0; i < WAVE_COUNT; i++)
		GerstnerWave(pos.xz, w[i], t, pos_sum, norm_sum);
	
	pos.xz = pos.xz - pos_sum.xz / WAVE_COUNT;
	pos.y = pos.y + pos_sum.y / WAVE_COUNT;
	normal.xz = -norm_sum.xz / WAVE_COUNT;
	normal.y = 1.0 - norm_sum.y / WAVE_COUNT;
}


void main(void) 
{
	wsVertex = (ModelMatrix * vec4(Vertex, 1.0)).xyz;	
	Map(wsVertex, wsNormal);

	gl_Position = (ViewProjectionMatrix) * vec4(wsVertex, 1.0);
}