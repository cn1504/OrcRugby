#version 410

uniform mat4 ProjectionMatrix;

uniform float MaxDepth;

layout(location = 0) in vec3 viewVertex;
layout(location = 1) in vec3 viewNormal;
layout(location = 2) in vec2 uv;

layout(location = 0) out vec4 outColor;

vec3 pack(float depth)
{
	const vec3 scale = vec3(1.0, 256.0, 65536.0);
	const vec2 ogb = vec2(65536.0, 256.0) / 16777215.0;
	const float normal	= 256.0 / 255.0;
	
	vec3 unit = vec3(depth);					// Avoid Precision Errors
	unit.gb -= floor(unit.gb / ogb) * ogb;
	
	vec3 color = unit * scale;					// Scale Up
	color = fract(color);						// Use Fraction to emulate Modulo
	color *= normal;							// Normalize Range
	color.rg -= color.gb / 256.0;				// Mask Noise

	return color;
}

void main(void) {
	float linearDepth = min(dot(viewVertex, viewVertex) / (MaxDepth * MaxDepth), 1);
	outColor = vec4(pack(linearDepth), 1);
}