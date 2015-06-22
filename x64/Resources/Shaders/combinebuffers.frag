#version 430

uniform sampler2D LightTexture;

layout(location = 0) in vec2 texCoord;

layout(location = 0) out vec4 outColor;

void main(void)
{
	vec3 light   = texture(LightTexture, texCoord).xyz;
	
	outColor.xyz   = light;
	outColor.a     = dot(outColor.xyz, vec3(0.299, 0.587, 0.114)); // compute luma;	
}