#version 410

uniform samplerCube gCubemapTexture;

layout(location = 0) in vec3 Vertex;

layout(location = 0) out vec4 outBase;
layout(location = 1) out vec4 outNormal;
layout(location = 2) out vec4 outMSR;
layout(location = 4) out vec4 outEmissive;

void main()
{
    outEmissive = texture(gCubemapTexture, Vertex);
} 