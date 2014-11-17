#version 410

uniform sampler2D GuiTexture;

layout(location = 0) in vec2 texCoord;

layout(location = 0) out vec4 outColor;

void main(void) {
	outColor = texture2D(GuiTexture, texCoord);
	//outColor.w = 0.2;
	
	//outColor.w *= opacity;
	//outColor = vec4(1.0,1.0,1.0,1.0);
}