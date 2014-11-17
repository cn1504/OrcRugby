#version 410

uniform mat4 ProjectionMatrix;

uniform vec3 SpherePosition;
uniform float Radius;

uniform float MaxDepth;

layout(location = 0) in vec3 viewVertex;
layout(location = 1) in vec3 viewNormal;
layout(location = 2) in vec2 uv;

layout(location = 0) out vec4 outColor;

vec4 pack(float depth)
{
	const vec4 bias = vec4(1.0 / 255.0, 1.0 / 255.0, 1.0 / 255.0, 0.0);
	float r = depth;
	float g = fract(r * 255.0);
	float b = fract(g * 255.0);
	float a = fract(b * 255.0);
	vec4 color = vec4(r, g, b, a);

	return color - (color.yzww * bias);
}

void main(void)
{	
	float r = Radius;	
	vec3 q0 = vec3(0.0, 0.0, 0.0);
	vec3 v = normalize(viewVertex);
	vec3 p = SpherePosition;
	
	float sc = dot(v, p);
	vec3 qc = q0 + sc*v;
	vec3 wc = qc - p;
	float d = length(wc);
	if(d > r)
		discard;
	
	vec3 qi = q0 + (sc - sqrt(r*r - d*d)) * v;
		
	//Set the depth based on the new cameraPos.
	vec4 clipPos = ProjectionMatrix * vec4(qi, 1.0);
	float ndcDepth = clipPos.z / clipPos.w;
	gl_FragDepth = ((gl_DepthRange.diff * ndcDepth) + gl_DepthRange.near + gl_DepthRange.far) / 2.0;
	
	float linearDepth = length(viewVertex) / MaxDepth;
	outColor = pack(linearDepth);
}