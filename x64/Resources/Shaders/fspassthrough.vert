#version 410

layout(location = 0) in vec3 Vertex;

layout(location = 0) out vec2 texCoord;

void main(void)
{
	gl_Position.xyz = Vertex;
    gl_Position.w = 1.0;
	texCoord = Vertex.xy * 0.5 + 0.5;
}