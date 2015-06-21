#version 430

uniform mat3 Transform;

layout(location = 0) in vec4 coord;

layout(location = 0) out vec2 texcoord;
 
void main(void) {
	vec3 pos = Transform * vec3(coord.xy, 1);
	gl_Position = vec4(pos.xy, 0, 1);
	texcoord = coord.zw;	
}