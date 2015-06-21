#version 430

uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;
uniform float iGlobalTime;           // shader playback time (in seconds)

layout(location = 0) in vec3 wsVertex;
layout(location = 1) in vec3 wsNormal;

layout(location = 0) out vec4 outNormal;
layout(location = 1) out vec4 outBase;
layout(location = 2) out vec4 outMSR;

vec4 EncodeNormal (vec3 n)
{
    return vec4(n.xyz * 0.5 + 0.5, 1.0);
}


const vec3 SEA_WATER_COLOR = vec3(0.0, 0.117647, 0.156863);

void main() 
{
	vec3 viewVertex = (ViewMatrix * vec4(wsVertex, 1.0)).xyz;
	vec3 viewNormal = normalize((ViewMatrix * vec4(normalize(wsNormal), 0.0)).xyz);
	vec3 eye = normalize(viewVertex);
	
	
	outBase = vec4(SEA_WATER_COLOR, 1.0);//vec4(pow(color,vec3(0.75)), 1.0);
	outNormal = EncodeNormal(viewNormal);
	outMSR = vec4(0.0, 0.3, 0.5, 1.0);
}