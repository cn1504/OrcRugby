#version 430

uniform mat4 ViewMatrix;
uniform float iGlobalTime;           // shader playback time (in seconds)

uniform float MaxDepth;

layout(location = 0) in vec3 wsVertex;

layout(location = 0) out vec4 outColor;

vec4 pack(float depth)
{
	const vec4 bit_shift = vec4(256.0*256.0*256.0, 256.0*256.0, 256.0, 1.0);
    const vec4 bit_mask  = vec4(0.0, 1.0/256.0, 1.0/256.0, 1.0/256.0);
    vec4 res = fract(depth * bit_shift);
    res -= res.xxyz * bit_mask;
    return res;
}

void main() {
	vec3 viewVertex = (ViewMatrix * vec4(wsVertex, 1.0)).xyz;

	float linearDepth = dot(viewVertex, viewVertex) / (MaxDepth * MaxDepth);
	outColor = pack(linearDepth);
	
	//outColor = vec4(linearDepth, linearDepth, linearDepth, 1);
}