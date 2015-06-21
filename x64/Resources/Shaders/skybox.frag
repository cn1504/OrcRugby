#version 430

uniform samplerCube gCubemapTexture;

layout(location = 0) in vec3 Vertex;

layout(location = 0) out vec4 outBase;
layout(location = 1) out vec4 outNormal;
layout(location = 2) out vec4 outMSR;
layout(location = 4) out vec4 outEmissive;
layout(location = 5) out vec4 outSkybox;

void main()
{
	outBase = vec4(0.0, 0.0, 0.0, 1.0);
	outNormal = vec4(0.0, 0.0, 0.0, 1.0);
	outMSR = vec4(0.0, 0.0, 0.0, 1.0);
	outEmissive = vec4(0.0, 0.0, 0.0, 1.0);
    outSkybox = texture(gCubemapTexture, Vertex);
} 