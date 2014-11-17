#version 410

const int MaxBoneConnectionsPerVertex = 4;
const int MaxBones = 64;
uniform mat4 BoneTransform[MaxBones];

uniform mat4 ModelViewProjectionMatrix;
uniform mat4 ModelViewMatrix;

layout(location = 1) in vec3 Vertex;
layout(location = 2) in vec3 Normal;
layout(location = 3) in vec2 Uv;
layout(location = 4) in ivec4 BoneID;
layout(location = 5) in vec4 BoneWeight;

layout(location = 0) out vec3 viewVertex;
layout(location = 1) out vec3 viewNormal;
layout(location = 2) out vec2 uv;
 
void main(void)
{
	mat4 tx = (BoneWeight.x > 0) ? BoneTransform[BoneID.x] * BoneWeight.x : mat4(0);
	mat4 ty = (BoneWeight.y > 0) ? BoneTransform[BoneID.y] * BoneWeight.y : mat4(0);
	mat4 tz = (BoneWeight.z > 0) ? BoneTransform[BoneID.z] * BoneWeight.z : mat4(0);
	mat4 tw = (BoneWeight.w > 0) ? BoneTransform[BoneID.w] * BoneWeight.w : mat4(0);
	mat4 transform = tx + ty + tz + tw;
	
	mat4 finalViewTransform = ModelViewMatrix * transform;
	
	viewVertex	= ((finalViewTransform) * vec4(Vertex, 1.0)).xyz;
	viewNormal	= ((finalViewTransform) * vec4(Normal, 0.0)).xyz;
    uv = Uv;
	
	gl_Position = ModelViewProjectionMatrix * transform * vec4(Vertex, 1.0);
}