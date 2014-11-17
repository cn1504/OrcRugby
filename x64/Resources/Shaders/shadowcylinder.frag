#version 410

uniform mat4 ProjectionMatrix;

uniform vec3 Direction;
uniform vec3 Center;
uniform float Length;
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
	
	vec3 o = vec3(0.0, 0.0, 0.0);
	vec3 v = normalize(viewVertex);
	vec3 p0 = Center - (Length/2)*Direction;
	vec3 p1 = Center + (Length/2)*Direction;
	vec3 u = normalize(p1 - p0);
	
	if((abs(dot(u, normalize(viewNormal))) > 0.9999) &&
		(length(viewVertex - p0) < r || length(viewVertex - p1) < r))
	{		
		//Set the depth based on the new cameraPos.
		vec4 clipPos = ProjectionMatrix * vec4(viewVertex, 1.0);
		float ndcDepth = clipPos.z / clipPos.w;
		gl_FragDepth = ((gl_DepthRange.diff * ndcDepth) + gl_DepthRange.near + gl_DepthRange.far) / 2.0;
	}
	else
	{
		vec3 AB = (p1 - p0);
		vec3 AO = (o - p0);
		vec3 AOxAB = cross(AO, AB);
		vec3 VxAB  = cross(v, AB);
		float ab2  = dot(AB, AB);
		float a    = dot(VxAB, VxAB);
		float b    = 2 * dot(VxAB, AOxAB);
		float c    = dot(AOxAB, AOxAB) - (r*r * ab2);
		
		float det = b*b - 4*a*c;
		if(det < 0)
			discard;
		
		float sqrtDet = sqrt(det);
		float t1 = (-b - sqrtDet)/(2*a);
		float t2 = (-b + sqrtDet)/(2*a);
		float t = min(t1, t2);
		
		vec3 qi = v*t;
		float pi = dot(qi - p0, u);
		
		if(pi < 0 || pi > Length)
		{
			discard;
		}
		
		vec4 clip_space_pos = ProjectionMatrix * vec4(qi, 1.0);
		float ndc_depth = clip_space_pos.z / clip_space_pos.w;
		float depth = (((gl_DepthRange.far-gl_DepthRange.near) * ndc_depth) + gl_DepthRange.near + gl_DepthRange.far) / 2.0;
		gl_FragDepth = depth;
	}
	
	float linearDepth = length(viewVertex) / MaxDepth;
	outColor = pack(linearDepth);
}