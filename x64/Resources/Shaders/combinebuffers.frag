#version 410

uniform sampler2D BaseTexture;
uniform sampler2D MSRTexture;
uniform sampler2D DiffuseLightTexture;
uniform sampler2D SpecularLightTexture;
uniform sampler2D GlowTexture;
uniform sampler2D EmissiveTexture;
uniform sampler2D SkyboxTexture;

layout(location = 0) in vec2 texCoord;

layout(location = 0) out vec4 outColor;

void main(void)
{
	vec3 Base      = texture(BaseTexture, texCoord).xyz;
	vec3 MSR       = texture(MSRTexture, texCoord).xyz;
	vec3 diffuse   = texture(DiffuseLightTexture, texCoord).xyz;
	vec3 specular  = texture(SpecularLightTexture, texCoord).xyz;
	vec3 glow      = texture(GlowTexture, texCoord).xyz;
	vec3 emissive  = texture(EmissiveTexture, texCoord).xyz;
	vec3 sky       = texture(SkyboxTexture, texCoord).xyz;
	
	vec3 baseColor = diffuse;	
	baseColor     += specular;		
	//baseColor     += glow;
	//baseColor     += emissive;
	baseColor     += sky;
		
	outColor.xyz   = baseColor;
	outColor.a     = 1.0;	
}