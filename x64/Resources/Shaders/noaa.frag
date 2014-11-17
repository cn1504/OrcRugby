#version 410

uniform sampler2D sourceTexture;

layout(location = 0) in vec2 texCoord;

layout(location = 0) out vec4 outColor;

void main(void)
{	
	outColor = texture2D(sourceTexture, texCoord);
}