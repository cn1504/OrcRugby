#version 410

uniform vec4  Color;
uniform vec4  MSR;

layout(location = 0) in vec3 viewVertex;
layout(location = 1) in vec3 viewNormal;
layout(location = 2) in vec2 uv;

layout(location = 0) out vec4 outNormal;
layout(location = 1) out vec4 outBase;
layout(location = 2) out vec4 outMSR;

vec4 EncodeNormal (vec3 n)
{
    return vec4(n.xyz * 0.5 + 0.5, 1.0);
}

void main(void)
{		
	outBase 	= Color;
	outNormal 	= EncodeNormal(normalize(viewNormal));
	outMSR 		= MSR;  //vec4(Metallic, Specular, Roughness, 1.0);
}