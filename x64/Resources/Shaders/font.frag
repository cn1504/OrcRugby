#version 430
 
uniform sampler2D tex;
uniform vec4 color;

layout(location = 0) in vec2 texcoord;

layout(location = 0) out vec4 outColor;
 
void main(void) {
	outColor = texture(tex, texcoord) * color;
}