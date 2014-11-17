#version 410

uniform sampler2D DiffuseTexture;
uniform sampler2D SpecularTexture;
uniform sampler2D DiffuseLightTexture;
uniform sampler2D SpecularLightTexture;
uniform sampler2D GlowTexture;
uniform sampler2D EmissiveTexture;
uniform vec3 AmbientLight; 

layout(location = 0) in vec2 texCoord;

layout(location = 0) out vec4 outColor;

void main(void)
{
	vec3 diffuse   = texture(DiffuseTexture, texCoord).xyz;
	vec3 specularC = texture(SpecularTexture, texCoord).xyz;
	vec3 light     = texture(DiffuseLightTexture, texCoord).xyz;
	vec3 specular  = texture(SpecularLightTexture, texCoord).xyz;
	vec3 glow      = texture(GlowTexture, texCoord).xyz;
	vec3 emissive  = texture(EmissiveTexture, texCoord).xyz;
	
	vec3 baseColor = diffuse * AmbientLight + 				// ambient
					 diffuse * light;						// diffused light
	
	baseColor     += specularC * specular;					// specular light
	baseColor     += glow;
	
	baseColor     += emissive;
		
	outColor.xyz   = baseColor;
	outColor.a     = 1.0;	
	
	//outColor = vec4(light, 1.0);
	//outColor = vec4(specular, 1.0);
}