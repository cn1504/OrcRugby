#version 410

uniform sampler2D DepthTexture;
uniform sampler2D NormalTexture;
uniform samplerCube ShadowTexture;
uniform sampler2D BaseTexture;
uniform sampler2D MSRTexture;

uniform vec2      PixelSize;
uniform mat4      ProjectionInverse;
uniform mat4      ViewInverse;

uniform float     LightRadius;
uniform vec3      LightPosition;
uniform vec4      LightColor;
uniform mat4      LightViewMatrix;
uniform mat4      LightProjectionMatrix;

layout(location = 0) in vec3 viewVertex;
layout(location = 1) in vec3 viewNormal;

layout(location = 0) out vec4 outDiffuse;
layout(location = 1) out vec4 outSpecular;

#define M_PI 3.1415926535897932384626433832795

float unpack(vec3 color)
{
	const vec3 scale = vec3(65536.0, 256.0, 1.0) / 65793.0;
	return dot(color, scale);
}

float G1V(float dotNV, float k)
{
	return 1.0f/(dotNV*(1.0f-k)+k);
}

float LightingFuncGGX(vec3 N, vec3 V, vec3 L, float roughness, float F0)
{
	float alpha = roughness*roughness;

	vec3 H = normalize(V+L);

	float dotNL = clamp(dot(N,L), 0.0, 1.0);
	float dotNV = clamp(dot(N,V), 0.0, 1.0);
	float dotNH = clamp(dot(N,H), 0.0, 1.0);
	float dotLH = clamp(dot(L,H), 0.0, 1.0);

	float F, D, vis;

	// D
	float alphaSqr = alpha*alpha;
	float pi = 3.14159f;
	float denom = dotNH * dotNH *(alphaSqr-1.0) + 1.0f;
	D = alphaSqr/(pi * denom * denom);

	// F
	float dotLH5 = pow(1.0f-dotLH,5);
	F = F0 + (1.0-F0)*(dotLH5);

	// V
	float k = alpha/2.0f;
	vis = G1V(dotNL,k)*G1V(dotNV,k);

	float specular = dotNL * D * F * vis;
	return specular;
}

void main(void)
{
	// Extract buffered pixel position and normal from textures
	vec3 pos 		= vec3((gl_FragCoord.x * PixelSize.x), 
						   (gl_FragCoord.y * PixelSize.y), 0.0);
	pos.z           = texture(DepthTexture, pos.xy).r;
	vec3 normal     = normalize(texture(NormalTexture, pos.xy).xyz * 2.0 - 1.0);
	vec4 BaseColor	= texture(BaseTexture, pos.xy);	
	vec4 MSR  		= texture(MSRTexture, pos.xy);	
	vec4 clip       = ProjectionInverse * vec4(pos * 2.0 - 1.0, 1.0);
	pos             = clip.xyz / clip.w;
	
	// Calculate light amount
	vec3 lightDir   = LightPosition - pos;
	float dist      = dot(lightDir, lightDir);	
	float distRel   = dist / (LightRadius * LightRadius);
	//float atten     = min(1.0 - distRel, 1.0);
	float atten =  1.0 / (dist);
	
	vec3 incident   = normalize(lightDir);
		
	if(atten <= 0.0) 
	{
		discard;
	}
	if (dot(normal, incident) <= 0.0) 
    {
		discard;
    }
		
	vec3 viewDir    = normalize(- pos);
	vec3 halfDir	= normalize(viewDir + incident);
	float dotLN		= clamp(dot(incident, normal), 0.0, 1.0);
	float dotVN		= clamp(dot(viewDir, normal), 0.0, 1.0);
	float dotLH		= clamp(dot(halfDir, incident), 0.0, 1.0);
	
	float F0 = MSR.y * 0.2;
	float thetaD = acos(dotLH);
	float Fd90 = 0.5 + 1.0 * cos(thetaD * thetaD) * MSR.z;
	//float Fd90 = 0.5 + 2.0 * cos(thetaD * thetaD) * MSR.z;
	vec3 diffuse = (mix(BaseColor.xyz, vec3(0.0), MSR.x) / M_PI) * (1.0 + (Fd90 - 1.0) * pow(1.0 - dotLN, 5.0)) * (1.0 + (Fd90 - 1.0) * pow(1.0 - dotVN, 5.0));
	
	float specFactor = LightingFuncGGX(normal, viewDir, incident, MSR.z, F0);
	
	
	// shadow map test
    //float bias = 0.005*tan(acos(lambert)); // cosTheta is dot( n,l ), clamped between 0 and 1
	//bias = 1.0 - clamp(bias, 0.0, 0.01);
	
	vec4 position_ls = LightViewMatrix * ViewInverse * vec4(pos, 1.0); 
	vec4 sms = texture(ShadowTexture, position_ls.xyz);
	float depth = unpack(sms.xyz);
	float bias = 0.96;
	float result = (distRel * bias > depth) ? 0.0 : 1.0;
	
	//vec4 abs_position = abs(position_ls);
	//float fs_z = -max(abs_position.x, max(abs_position.y, abs_position.z));
	//vec4 lclip = LightProjectionMatrix * vec4(0.0, 0.0, fs_z, 1.0);
	//float depth = (lclip.z / lclip.w) * 0.5 + 0.5;
	//float result = texture(ShadowTexture, vec4(position_ls.xyz, depth * 0.99999));
		
	//float fShadow = texture(ShadowTexture, -posW).r;
	//fShadow = (length(lightDir / LightRadius) > depth) ? 0.3 : 1.0;
	
	//float vShadowSample = texture(ShadowTexture, -incident).r;
	//float fShadow = (dot(lightDir, lightDir) - fDepthBias - vShadowSample / LightRadius < 0.0f) ? 1.0f : 0.3f;
	
	// ESM
	//const float c = 60.0; // Sharp shadows good for interior scenes
	//const float c = 5.0; 	// Soft shadows, good for day time exterior scenes
	//float result = clamp(exp(-c * ( dist - vShadowSample )), 0.0, 1.0);

	
	outDiffuse = vec4(result * LightColor.xyz * atten * diffuse, 1.0);
	outSpecular = vec4(result * mix(LightColor.xyz, LightColor.xyz * BaseColor.xyz, MSR.x) * atten * specFactor, 1.0);
}