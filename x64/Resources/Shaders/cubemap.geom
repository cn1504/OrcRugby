#version 430

uniform mat4 CubeMapViews[6];
uniform mat4 CubeMapProjections[6];

layout(triangles, invocations = 1) in;
layout(triangle_strip, max_vertices = 18) out;

layout(location = 0) in vec3 pos[];
layout(location = 1) in vec3 Normal[];
layout(location = 2) in vec2 Uv[];

layout(location = 0) out vec3 frag_pos;
layout(location = 1) out vec3 viewNormal;
layout(location = 2) out vec2 uv;

void main() {
    
    for(int j = 0; j < 6; ++j) {
        gl_Layer = j;
        for(int i = 0; i < 3; ++i) {
            frag_pos = (CubeMapViews[j] * vec4(pos[i], 1)).xyz;
			viewNormal = (CubeMapViews[j] * vec4(normalize(Normal[i]), 0)).xyz;
			uv = Uv[i];
            gl_Position = CubeMapProjections[j] * vec4(pos[i], 1);
            EmitVertex();
        }
        EndPrimitive();
    }
}