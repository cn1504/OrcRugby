#version 430

uniform mat4 ModelViewProjectionMatrix;

layout(location = 0) in vec3 Vertex;
layout(location = 1) in vec3 Normal;
layout(location = 2) in vec2 Uv;

layout(location = 0) out vec3 TexCoord0;

void main()
{
    vec4 WVP_Pos = ModelViewProjectionMatrix * vec4(Vertex, 1.0);
    gl_Position = WVP_Pos.xyww;
    TexCoord0 = Vertex;
}