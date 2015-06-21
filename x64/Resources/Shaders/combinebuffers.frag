#version 430

uniform sampler2D DiffuseLightTexture;
uniform sampler2D SpecularLightTexture;

layout(location = 0) in vec2 texCoord;

layout(location = 0) out vec4 outColor;

void main(void)
{
	vec3 diffuse   = texture(DiffuseLightTexture, texCoord).xyz;
	vec3 specular  = texture(SpecularLightTexture, texCoord).xyz;
	
	vec3 baseColor = diffuse;	
	baseColor     += specular;		
		
	outColor.xyz   = baseColor;
	outColor.a     = 1.0;	
}