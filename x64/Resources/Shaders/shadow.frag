#version 430

uniform mat4 ProjectionMatrix;

uniform float MaxDepth;

layout(location = 0) in vec3 viewVertex;
layout(location = 1) in vec3 viewNormal;
layout(location = 2) in vec2 uv;

layout(location = 0) out vec4 outColor;

vec4 pack(float depth)
{
	const vec4 bit_shift = vec4(256.0*256.0*256.0, 256.0*256.0, 256.0, 1.0);
    const vec4 bit_mask  = vec4(0.0, 1.0/256.0, 1.0/256.0, 1.0/256.0);
    vec4 res = fract(depth * bit_shift);
    res -= res.xxyz * bit_mask;
    return res;
}

void main(void) {
	float linearDepth = dot(viewVertex, viewVertex) / (MaxDepth * MaxDepth);
	outColor = pack(linearDepth);
	
	//outColor = vec4(linearDepth, linearDepth, linearDepth, 1);
}