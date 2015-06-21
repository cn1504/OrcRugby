#version 430

uniform mat4 ModelViewProjectionMatrix;
uniform mat4 ModelViewMatrix;

layout(location = 0) in vec3 Vertex;
layout(location = 1) in vec3 Normal;
layout(location = 2) in vec2 Uv;

layout(location = 0) out vec3 viewVertex;
layout(location = 1) out vec3 viewNormal;
layout(location = 2) out vec2 uv;

void main(void) 
{
	viewVertex	        = vec3((ModelViewMatrix) * vec4(Vertex, 1.0));
	viewNormal			= vec3((ModelViewMatrix) * vec4(Normal, 0.0));
	uv = Uv;
	gl_Position         = (ModelViewProjectionMatrix) * vec4(Vertex, 1.0);
}