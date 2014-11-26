#version 410

uniform sampler2D DepthTexture;
uniform sampler2D NormalTexture;
uniform sampler2D BaseTexture;
uniform sampler2D MSRTexture;

uniform vec2      PixelSize;
uniform mat4      ProjectionInverse;

uniform float     LightRadius;
uniform vec3      LightPosition;
uniform vec4      LightColor;

layout(location = 0) in vec3 viewVertex;
layout(location = 1) in vec3 viewNormal;

layout(location = 0) out vec4 outDiffuse;
layout(location = 1) out vec4 outSpecular;

#define M_PI 3.1415926535897932384626433832795

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
	float atten     = min(1.0 - distRel, 1.0);
	
	vec3 incident   = normalize(lightDir);
	
	if(atten < 0.0) 
	{
		discard;
	}
	if (dot(normal, incident) < 0.0) 
    {
		discard;
    }
		
	vec3 viewDir    = normalize(- pos);
	vec3 halfDir	= normalize(viewDir + incident);
	float cosL		= dot(incident, normal);
	float cosV		= dot(viewDir, normal);
	float cosD		= dot(halfDir, incident);
	float cosH		= dot(halfDir, normal);
	float cosH2		= cosH * cosH;
	float sinH2		= 1.0 - cosH2;
	
	float specular = MSR.y * 0.08;
	float thetaD = acos(cosD);
	float cosDSquared = cos(thetaD * thetaD);
	float fd90 = (0.5 + 2.0 * cosDSquared * MSR.z);
	float diffuse = (1.0 + (fd90 - 1.0) * pow(1 - cosL, 5.0)) * (1.0 + (fd90 - 1.0) * pow(1.0 - cosV, 5.0)) / M_PI;

	float specD = 1.0 / pow(MSR.z * MSR.z * MSR.z * MSR.z * cosH2 + sinH2, 2.0);
	// float specDClear = 1.0 / pow(MSR.z * MSR.z * MSR.z * MSR.z * cosH2 + sinH2, 1.0);
	
	vec3 f0 = mix(LightColor.xyz, LightColor.xyz * BaseColor.xyz, MSR.x);
	vec3 specF = f0 + (1.0 - f0) * pow(1 - cosD, 5.0);
	
	float specG = (0.5 + 0.5 * MSR.z);
	specG = specG * specG;
	
	vec3 specFactor = specF;
	
	outDiffuse = vec4(LightColor.xyz * atten * mix(BaseColor.xyz, vec3(0.0), MSR.x) * diffuse, 1.0);
	outSpecular = vec4(atten * specFactor, 1.0);
}