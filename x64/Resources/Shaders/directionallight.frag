#version 430

uniform sampler2D DepthTexture;
uniform sampler2D NormalTexture;
uniform sampler2D BaseTexture;
uniform sampler2D MSRTexture;

uniform mat4      ProjectionInverse;
uniform mat4      ViewInverse;

uniform sampler2D ShadowTexture;
uniform mat4      LightView;
uniform mat4      LightProjection;
uniform float     MaxDepth;

uniform vec3      LightDirection;
uniform vec4      LightColor;
uniform float     LightIntensity;

layout(location = 0) in vec2 texCoord;

layout(location = 0) out vec4 outLuminance;

// Shadow Mapping Functions
//-------------------------------------------------------------------------------------------
float ShadowMap(float LdotN, float invSqrRadius, vec3 fragPosition)
{
	vec4 vsPosition = LightView * ViewInverse * vec4(fragPosition, 1.0);
	vec4 shadowCoord = LightProjection * vsPosition;
	shadowCoord.xyz = shadowCoord.xyz / shadowCoord.w;
	shadowCoord.xyz = shadowCoord.xyz * 0.5 + 0.5;
	float depthmap = texture(ShadowTexture, shadowCoord.xy).r;
	float result = (depthmap < shadowCoord.z) ? 0.0 : 1.0;
	
	//float bias = 0.0;	
	//float bias = 0.0000000001*tan(acos(LdotN)); // cosTheta is dot( n,l ), clamped between 0 and 1
	//bias = 1.0 - clamp(bias, 0.0, 1.0);
	//float result = (distRel > depth) ? 0.0 : 1.0;
	
	// ESM
	//const float c = 60.0; // Sharp shadows good for interior scenes
	//const float c = 5.0; 	// Soft shadows, good for day time exterior scenes
	//float result = clamp(exp(-c * ( distRel - depth )), 0.0, 1.0);
	
	return result;
}

#define M_PI 3.1415926535897932384626433832795

// BRDF Functions
//-------------------------------------------------------------------------------------------
vec3 F_Schlick(vec3 f0, float f90, float u)
{
	return f0 + (f90 - f0) * pow(1.0 - u, 5.0);
}

float V_SmithGGXCorrelated(float NdotL, float NdotV, float alphaG)
{
	float alphaG2 = alphaG * alphaG;
	float Lambda_GGXV = NdotL * sqrt((-NdotV * alphaG2 + NdotV) * NdotV + alphaG2);
	float Lambda_GGXL = NdotV * sqrt((-NdotV * alphaG2 + NdotL) * NdotL + alphaG2);
	return 0.5 / (Lambda_GGXV + Lambda_GGXL);
}

float D_GGX(float NdotH, float m)
{
	// Divide by PI is applied later
	float m2 = m * m;
	float f = (NdotH * m2 - NdotH) * NdotH + 1.0;
	return m2 / (f * f);
}

float Fd_DisneyDiffuse(float NdotV, float NdotL, float LdotH, float linearRoughness)
{
	float energyBias 	= mix(0.0, 0.5, linearRoughness);
	float energyFactor 	= mix(1.0, 1.0 / 1.51, linearRoughness);
	float fd90 			= energyBias + 2.0 * LdotH * LdotH * linearRoughness;
	vec3 f0 			= vec3(1.0);
	float lightScatter 	= F_Schlick(f0, fd90, NdotL).r;
	float viewScatter 	= F_Schlick(f0, fd90, NdotV).r;
	
	return lightScatter * viewScatter * energyFactor;
}

// Lighting Functions
//-------------------------------------------------------------------------------------------

vec3 getSkyColor(vec3 e) {
    e.y = max(e.y,0.0);
    vec3 ret;
    ret.x = pow(1.0-e.y,2.0);
    ret.y = 1.0-e.y;
    ret.z = 0.6+(1.0-e.y)*0.4;
    return ret;
}

void main(void)
{	
	// Extract buffered pixel position and normal from textures
	vec3 pos 		= vec3(texCoord, 0.0);
	pos.z           = texture(DepthTexture, pos.xy).r;
	vec3 normal     = texture(NormalTexture, pos.xy).rgb;
	vec4 BaseColor	= texture(BaseTexture, pos.xy);	
	vec4 MSR  		= texture(MSRTexture, pos.xy);	
	vec4 clip       = ProjectionInverse * vec4(pos * 2.0 - 1.0, 1.0);
	pos             = clip.xyz / clip.w;
	
	// Calculate light amount
	vec3 lightDir   = LightDirection;
	vec3 incident   = normalize(lightDir);
		
	if(dot(incident, normal) < -1e-5f) 
	{	
		discard;
	}
		
	vec3 viewDir    = normalize(- pos);
	vec3 halfDir	= normalize(viewDir + incident);
	float NdotV		= abs(dot(viewDir, normal));
	float NdotH		= clamp(dot(normal, halfDir), 0.0, 1.0);
	float LdotN		= clamp(dot(incident, normal), 0.0, 1.0);
	float LdotH		= clamp(dot(halfDir, incident), 0.0, 1.0);
	
	float roughness = MSR.z * MSR.z * MSR.z * MSR.z;
	float reflectance = 0.16 * MSR.y * MSR.y;
	vec3 f0 = mix(vec3(reflectance), BaseColor.xyz, MSR.x);
	
	// Specular BRDF
	vec3  F   = F_Schlick(f0, 0.0, LdotH);
	float Vis = V_SmithGGXCorrelated(NdotV, LdotN, MSR.z);
	float D   = D_GGX(NdotH, roughness);
	vec3  Fr  = D * F * Vis / M_PI;
	
	// Diffuse BRDF
	float Fd = Fd_DisneyDiffuse(NdotV, LdotN, LdotH, MSR.z) / M_PI;	
		
	// Shadow Mapping Result
	float shadow = ShadowMap(LdotN, MaxDepth, pos);
	
	vec3 outDiffuse = (0.25 + shadow * 0.75) * LightColor.xyz * mix(BaseColor.xyz, vec3(0.0), MSR.x) * Fd * LightIntensity;
	vec3 outSpecular = shadow * LightColor.xyz * Fr * LightIntensity;
	outLuminance = vec4(outDiffuse + outSpecular, LightColor.w);
	
	// Distant Light Probe Approx for ambient light
	//outSpecular.xyz += LightColor.xyz * pow(1.0 - NdotV, 10.0);
}