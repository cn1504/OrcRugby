#version 430

uniform sampler2D LightTexture;

layout(location = 0) in vec2 texCoord;

layout(location = 0) out vec4 outColor;


vec3 SRGBToLinear(vec3 sRGBCol)
{
	vec3   linearRGBLo = sRGBCol / 12.92;
	vec3   linearRGBHi = pow((sRGBCol + 0.055) / 1.055, vec3(2.4));
	vec3   linearRGB;
		linearRGB.r = (sRGBCol.r <= 0.04045) ? linearRGBLo.r : linearRGBHi.r;
		linearRGB.g = (sRGBCol.g <= 0.04045) ? linearRGBLo.g : linearRGBHi.g;
		linearRGB.b = (sRGBCol.b <= 0.04045) ? linearRGBLo.b : linearRGBHi.b;
	return linearRGB;
}

vec3 LinearToSRGB(vec3 linearCol)
{
	vec3   sRGBLo = linearCol * 12.92;
	vec3   sRGBHi = (pow(abs(linearCol), vec3(1.0/2.4)) * 1.055) - 0.055;
	vec3   sRGB;
		sRGB.r = (linearCol.r <= 0.0031308) ? sRGBLo.r : sRGBHi.r;
		sRGB.g = (linearCol.g <= 0.0031308) ? sRGBLo.g : sRGBHi.g;
		sRGB.b = (linearCol.b <= 0.0031308) ? sRGBLo.b : sRGBHi.b;
	return sRGB;
}


const float A = 0.22;
const float B = 0.30;
const float C = 0.10;
const float D = 0.20;
const float E = 0.01;
const float F = 0.30;
const float W = 1.0; 
vec3 Uncharted2Tonemap(vec3 x)
{
	return ((x*(A*x+C*B)+D*E)/(x*(A*x+B)+D*F))-E/F;
}

void main(void)
{
	vec3 light = texture(LightTexture, texCoord).xyz;
			
	// HDR Tonemapping	
	vec3 texColor = light;

	float ExposureBias = 2.0f;
	vec3 curr = Uncharted2Tonemap(ExposureBias*texColor);
	 
	vec3 whiteScale = 1.0f/Uncharted2Tonemap(vec3(W));
	vec3 color = curr*whiteScale;
	 
	//vec3 retColor = pow(color, vec3(1.0/2.2));
		
	outColor.xyz   = color;
	outColor.a     = dot(outColor.xyz, vec3(0.299, 0.587, 0.114)); // compute luma;	
}