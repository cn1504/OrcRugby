#version 430

uniform sampler2D textureSample;
uniform vec2 texelSize; 

uniform int orientation; 
uniform int blurAmount; 
uniform float blurScale; 
uniform float blurStrength;

layout(location = 0) in vec2 texCoord;

layout(location = 0) out vec4 outColor;

/// <summary> 
/// Gets the Gaussian value in the first dimension. 
/// </summary> 
/// <param name="x">Distance from origin on the x-axis.</param> 
/// <param name="deviation">Standard deviation.</param> 
/// <returns>The gaussian value on the x-axis.</returns> 
float Gaussian (float x, float deviation) 
{ 
	return (1.0 / sqrt(2.0 * 3.141592 * deviation)) * exp(-((x * x) / (2.0 * deviation))); 
}

void main(void)
{
	float halfBlur = float(blurAmount) * 0.5; 
	vec4  color    = vec4(0.0); 
	vec4  texColor = vec4(0.0); 
	
	// Gaussian deviation 
	float deviation = halfBlur * 0.35; 
	deviation *= deviation; 
	float strength = 1.0 - blurStrength; 
	//float dynamicRangeModifier = 1.0 / dynamicRange;
	
	if ( orientation == 0 ) 
	{ 
		// Horizontal blur 
		for (int i = 0; i < blurAmount; ++i) 
		{ 				
			float offset = float(i) - halfBlur; 
			texColor = texture(textureSample, texCoord + vec2(offset * texelSize.x * blurScale, 0.0));
			
			//vec4 lightColor = texture(lightTexture, IN.texCoord + vec2(offset * texelSize.x * blurScale, 0.0));
			//vec4 specularColor = texture(specularTexture, IN.texCoord + vec2(offset * texelSize.x * blurScale, 0.0));
			//texColor.xyz += max((lightColor.xyz - dynamicRangeModifier) / (dynamicRange - 1.0), 0.0);
			//texColor.xyz += max((specularColor.xyz - dynamicRangeModifier) / (dynamicRange - 1.0), 0.0);
			
			texColor *= Gaussian(offset * strength, deviation); 
				
			color += texColor; 
		} 
	} 
	else 
	{ 
		// Vertical blur 
		for (int i = 0; i < blurAmount; ++i) 
		{ 			
			float offset = float(i) - halfBlur; 
			texColor = texture(textureSample, texCoord + vec2(0.0, offset * texelSize.y * blurScale)) 
				* Gaussian(offset * strength, deviation); 
			
			color += texColor; 
		} 
	} 
	
	// Apply color 
	outColor = clamp(color, 0.0, 1.0);
	outColor.w = 1.0;
}