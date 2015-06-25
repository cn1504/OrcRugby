#version 430

uniform sampler2D DepthTexture;
uniform sampler2D NormalTexture;
uniform sampler2D BaseTexture;
uniform sampler2D MSRTexture;

uniform vec2      PixelSize;
uniform mat4      ProjectionInverse;
uniform mat4      ViewInverse;

uniform sampler2D ShadowTexture;
uniform mat4      LightViewMatrix;
uniform mat4      LightProjectionMatrix;
uniform vec3      LightPosition;
uniform vec3      LightDirection;

uniform float     LightInvSqrRadius;
uniform vec4      LightColor;
uniform float     LightIntensity;
uniform float     LightAngleScale;
uniform float     LightAngleOffset;

layout(location = 0) in vec3 viewVertex;
layout(location = 1) in vec3 viewNormal;

layout(location = 0) out vec4 outLuminance;

#define M_PI 3.1415926535897932384626433832795

// Shadow Mapping Functions
//-------------------------------------------------------------------------------------------
float unpack(vec3 color)
{
	const vec3 scale = vec3(65536.0, 256.0, 1.0) / 65793.0;
	return dot(color, scale);
}

float ShadowMap(float sqrDist, float invSqrRadius, vec3 fragPosition)
{	
    vec4 vsPosition = LightViewMatrix * ViewInverse * vec4(fragPosition, 1.0);
	vec4 position_ls = LightProjectionMatrix * vsPosition;
	position_ls.xyz = position_ls.xyz / position_ls.w;
	vec2 shadowCoord = position_ls.xy * 0.5 + 0.5;
	float depthmap = texture(ShadowTexture, shadowCoord).r;
	float result = (depthmap < position_ls.z) ? 0.0 : 1.0;
	
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
float smoothDistanceAtt(float squaredDistance, float invSqrAttRadius)
{
	float factor = squaredDistance * invSqrAttRadius;
	float smoothFactor = clamp(1.0 - factor * factor, 0.0, 1.0);
	return smoothFactor;
}

float getDistanceAtt(float sqrDist, float invSqrAttRadius)
{
	float attenuation = 1.0 / (max(sqrDist, 0.01 * 0.01));
	attenuation *= smoothDistanceAtt(sqrDist, invSqrAttRadius);
	return attenuation;
}

float getAngleAtt(vec3 normalizedLightVector, vec3 lightDirection, float lightAngleScale, float lightAngleOffset)
{
	float cd = dot(lightDirection, normalizedLightVector);
	float attenuation = clamp(cd * lightAngleScale + lightAngleOffset, 0.0, 1.0);
	attenuation *= attenuation;
	return attenuation;
}

//-------------------------------------------------------------------------------------------
// Exposure
//-------------------------------------------------------------------------------------------
const float Aperture = 16.0;			// N in f-stops.
const float ShutterTime = 1.0 / 125.0;	// How long aperture is open.
const float ISO = 100.0; 				// S in ISO
float computeEV100(float aperture, float shutterTime, float ISO)
{
	return log2(aperture * aperture / shutterTime * 100 / ISO);
}
float computeEV100FromAvgLuminance(float avgLuminance)
{
	return log2(avgLuminance * 100.0 / 12.5);
}
float convertEV100ToExposer(float EV100)
{
	float maxLuminance = 1.2 * pow(2.0, EV100);
	return 1.0 / maxLuminance;
}
float computeExposure()
{
	return convertEV100ToExposer(computeEV100(Aperture, ShutterTime, ISO));
}



void main(void)
{
	// Extract buffered pixel position and normal from textures
	vec3 pos 		= vec3((gl_FragCoord.x * PixelSize.x), 
						   (gl_FragCoord.y * PixelSize.y), 0.0);
	pos.z           = texture(DepthTexture, pos.xy).r;
	//vec3 normal     = DecodeNormal(texture(NormalTexture, pos.xy));
	vec3 normal     = texture(NormalTexture, pos.xy).rgb;
	vec4 BaseColor	= texture(BaseTexture, pos.xy);	
	vec4 MSR  		= texture(MSRTexture, pos.xy);	
	vec4 clip       = ProjectionInverse * vec4(pos * 2.0 - 1.0, 1.0);
	pos             = clip.xyz / clip.w;
	
	// Calculate light amount
	vec3 lightDir   = LightPosition - pos;
	vec3 incident   = normalize(lightDir);
	
	float sqrDist = dot(lightDir, lightDir);
	float atten = getDistanceAtt(sqrDist, LightInvSqrRadius);
	atten *= getAngleAtt(incident, LightDirection, LightAngleScale, LightAngleOffset);
	
	if(atten <= 0.0) 
	{
		discard;
	}
	atten *= LightIntensity;
		
	vec3 viewDir    = normalize(- pos);
	vec3 halfDir	= normalize(viewDir + incident);
	float NdotV		= abs(dot(viewDir, normal));// + 1e-5f;	// avoids artifacts
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
	float shadow = ShadowMap(sqrDist, LightInvSqrRadius, pos);
		
	vec3 outDiffuse = shadow * LightColor.xyz * atten * mix(BaseColor.xyz, vec3(0.0), MSR.x) * Fd;
	vec3 outSpecular = shadow * LightColor.xyz * atten * Fr;
	outLuminance = vec4((outDiffuse + outSpecular) * computeExposure(), 1.0);
}