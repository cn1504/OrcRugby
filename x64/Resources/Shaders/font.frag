#version 410
 
uniform sampler2D tex;
uniform vec4 color;

layout(location = 0) in vec2 texcoord;

layout(location = 0) out vec4 outColor;
 
void main(void) {
	outColor = vec4(1, 1, 1, texture(tex, texcoord).a) * color;
}