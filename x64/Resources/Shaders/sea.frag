#version 430

uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;
uniform float iGlobalTime;           // shader playback time (in seconds)

layout(location = 0) in vec3 wsVertex;
layout(location = 1) in vec3 wsNormal;

layout(location = 0) out vec4 outNormal;
layout(location = 1) out vec4 outBase;
layout(location = 2) out vec4 outMSR;

//
//  Noise Perlin2D
//  Return value range of -1.0->1.0
//
float Noise_Perlin2D( vec2 P )
{
    //  https://github.com/BrianSharpe/Wombat/blob/master/Perlin2D.glsl

    // establish our grid cell and unit position
    vec2 Pi = floor(P);
    vec4 Pf_Pfmin1 = P.xyxy - vec4( Pi, Pi + 1.0 );

    // calculate the hash
    vec4 Pt = vec4( Pi.xy, Pi.xy + 1.0 );
    Pt = Pt - floor(Pt * ( 1.0 / 71.0 )) * 71.0;
    Pt += vec2( 26.0, 161.0 ).xyxy;
    Pt *= Pt;
    Pt = Pt.xzxz * Pt.yyww;
    vec4 hash_x = fract( Pt * ( 1.0 / 951.135664 ) );
    vec4 hash_y = fract( Pt * ( 1.0 / 642.949883 ) );

    // calculate the gradient results
    vec4 grad_x = hash_x - 0.49999;
    vec4 grad_y = hash_y - 0.49999;
    vec4 grad_results = inversesqrt( grad_x * grad_x + grad_y * grad_y ) * ( grad_x * Pf_Pfmin1.xzxz + grad_y * Pf_Pfmin1.yyww );

    // Classic Perlin Interpolation
    grad_results *= 1.4142135623730950488016887242097;  // scale things to a strict -1.0->1.0 range  *= 1.0/sqrt(0.5)
    vec2 blend = Pf_Pfmin1.xy * Pf_Pfmin1.xy * Pf_Pfmin1.xy * (Pf_Pfmin1.xy * (Pf_Pfmin1.xy * 6.0 - 15.0) + 10.0);
    vec4 blend2 = vec4( blend, vec2( 1.0 - blend ) );
    return dot( grad_results, blend2.zxzx * blend2.wwyy );
}
		
const vec3 SEA_WATER_COLOR = vec3(0.0, 0.117647, 0.156863);

void main() 
{
	vec3 viewVertex = (ViewMatrix * vec4(wsVertex, 1.0)).xyz;	
	vec3 viewNormal = normalize((ViewMatrix * vec4(normalize(wsNormal), 0.0)).xyz);
	vec3 eye = normalize(viewVertex);
	
	
	outBase = vec4(SEA_WATER_COLOR, 1.0);//vec4(pow(color,vec3(0.75)), 1.0);
	outNormal = vec4(viewNormal, 1.0);
	outMSR = vec4(0.0, 0.3, 0.5, 1.0);
}