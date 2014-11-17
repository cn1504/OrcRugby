#version 410

uniform sampler2D NormalTexture;
uniform sampler2D DepthTexture;
uniform sampler2D HardnessTexture;
uniform samplerCube ShadowTexture;

uniform vec2      PixelSize;
uniform mat4      ProjectionInverse;
uniform mat4      ViewInverse;

uniform float     LightRadius;
uniform vec3      LightPosition;
uniform vec4      LightColor;
uniform mat4      LightViewMatrix;
uniform mat4      LightProjectionMatrix;

layout(location = 0) in vec3 viewVertex;
layout(location = 1) in vec3 viewNormal;

layout(location = 0) out vec4 outDiffuse;
layout(location = 1) out vec4 outSpecular;

float unpack(vec3 color)
{
	const vec3 scale = vec3(65536.0, 256.0, 1.0) / 65793.0;
	return dot(color, scale);
}

void main(void)
{
	// Extract buffered pixel position and normal from textures
	vec3 pos 		= vec3((gl_FragCoord.x * PixelSize.x), 
						   (gl_FragCoord.y * PixelSize.y), 0.0);
	pos.z           = texture(DepthTexture, pos.xy).r;
	vec3 normal     = normalize(texture(NormalTexture, pos.xy).xyz * 2.0 - 1.0);
	float hardness  = texture(HardnessTexture, pos.xy).r * 512;
	vec4 clip       = ProjectionInverse * vec4(pos * 2.0 - 1.0, 1.0);
	pos             = clip.xyz / clip.w;
	
	// Calculate light amount
	vec3 lightDir   = LightPosition - pos;
	float dist      = dot(lightDir, lightDir);	
	float distRel   = dist / (LightRadius * LightRadius);
	float atten     = clamp(1.0 - distRel, 0.0, 1.0);
	vec3 incident   = normalize(lightDir);
	float lambert   = clamp(dot(incident, normal), 0.0, 1.0);

	if(atten == 0.0 || lambert < 0.0) 
	{
		discard;
	}
	
	// shadow map test
    //float bias = 0.005*tan(acos(lambert)); // cosTheta is dot( n,l ), clamped between 0 and 1
	//bias = 1.0 - clamp(bias, 0.0, 0.01);
	
	vec4 position_ls = LightViewMatrix * ViewInverse * vec4(pos, 1.0); 
	vec4 sms = texture(ShadowTexture, position_ls.xyz);
	float depth = unpack(sms.xyz);
	float bias = 0.96;
	float result = (distRel * bias > depth) ? 0.0 : 1.0;
	
	//vec4 abs_position = abs(position_ls);
	//float fs_z = -max(abs_position.x, max(abs_position.y, abs_position.z));
	//vec4 lclip = LightProjectionMatrix * vec4(0.0, 0.0, fs_z, 1.0);
	//float depth = (lclip.z / lclip.w) * 0.5 + 0.5;
	//float result = texture(ShadowTexture, vec4(position_ls.xyz, depth * 0.99999));
	
	
	//float fShadow = texture(ShadowTexture, -posW).r;
	//fShadow = (length(lightDir / LightRadius) > depth) ? 0.3 : 1.0;
	
	//float vShadowSample = texture(ShadowTexture, -incident).r;
	//float fShadow = (dot(lightDir, lightDir) - fDepthBias - vShadowSample / LightRadius < 0.0f) ? 1.0f : 0.3f;
	
	// ESM
	//const float c = 60.0; // Sharp shadows good for interior scenes
	//const float c = 5.0; 	// Soft shadows, good for day time exterior scenes
	//float result = clamp(exp(-c * ( dist - vShadowSample )), 0.0, 1.0);
	
	vec3 viewDir    = normalize(- pos);
	float sFactor = pow(max(0.0, dot(reflect(-incident, normal), viewDir)), hardness);

	outDiffuse = vec4(LightColor.xyz * result * lambert * atten, 1.0);
	outSpecular = vec4(LightColor.xyz * result * sFactor * atten, 1.0);
	
	//outDiffuse = vec4(depth, depth, depth, 1.0);
}