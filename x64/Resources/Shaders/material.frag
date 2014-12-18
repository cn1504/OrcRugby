#version 410

uniform mat4 ProjectionMatrix;

uniform vec4  BaseColor;
uniform float Metallic;
uniform float Specular;
uniform float Roughness;
uniform float Emissive;

layout(location = 0) in vec3 viewVertex;
layout(location = 1) in vec3 viewNormal;
layout(location = 2) in vec2 uv;

layout(location = 0) out vec4 outBase;
layout(location = 1) out vec4 outNormal;
layout(location = 2) out vec4 outMSR;
layout(location = 4) out vec4 outEmissive;
layout(location = 5) out vec4 outSkybox;

vec4 EncodeNormal (vec3 n)
{
    return vec4(n.xyz * 0.5 + 0.5, 1.0);
}

void main(void)
{		
	outBase 	= BaseColor;
	outNormal 	= EncodeNormal(normalize(viewNormal));
	outMSR 		= vec4(Metallic, Specular, Roughness, 1.0);
	outEmissive = vec4(BaseColor.rgb * Emissive, 1.0);
	outSkybox   = vec4(0.0, 0.0, 0.0, 1.0);
}