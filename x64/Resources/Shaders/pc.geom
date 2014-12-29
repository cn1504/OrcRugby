#version 410

layout(points, invocations = 1) in;
layout(points, max_vertices = 1) out;

layout(location = 0) in vec3 pos[];
layout(location = 1) in vec3 Normal[];
layout(location = 2) in vec2 Uv[];

layout(location = 0) out vec3 frag_pos;
layout(location = 1) out vec3 viewNormal;
layout(location = 2) out vec2 uv;

void main(void)
{    
	int i;
	
	for (i = 0; i < gl_in.length(); i++)
    {
		if(dot(pos[i], Normal[i]) <= 0.0)
		{
			frag_pos = pos[i];
			viewNormal = Normal[i];
			uv = Uv[i];
			gl_Position = gl_in[i].gl_Position;
			EmitVertex();
			EndPrimitive();
		}
	}
}