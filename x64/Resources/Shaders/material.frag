#version 410

const int MAX_INPUTS = 8;
const int MAX_COMMANDS = 16;
const int MAX_PARAMS = 3;

struct Value
{
	vec4 Base;
	vec4 MSR;
};
struct Texture
{
	sampler2D Base;
	sampler2D MSR;
};

struct Command
{
	ivec4 Op; // x: Op1, y: Op2, z: Sto, w: OpCode
	vec4 Params[MAX_PARAMS];
};

// Inputs
uniform Value InputValues[MAX_INPUTS];
uniform Texture InputTextures[MAX_INPUTS];
uniform int InputSource[MAX_INPUTS];

uniform Command Commands[MAX_COMMANDS];

uniform int InputCount;
uniform int CommandCount;

layout(location = 0) in vec3 viewVertex;
layout(location = 1) in vec3 viewNormal;
layout(location = 2) in vec2 uv;

layout(location = 0) out vec4 outNormal;
layout(location = 1) out vec4 outBase;
layout(location = 2) out vec4 outMSR;

vec4 EncodeNormal (vec3 n)
{
    return vec4(n.xyz * 0.5 + 0.5, 1.0);
}

#define M_PI 3.1415926535897932384626433832795

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

//
//  Cellular Noise 2D
//  produces a range of 0.0->1.0
//
float Noise_Cellular2D( vec2 P )
{
    //  https://github.com/BrianSharpe/Wombat/blob/master/Cellular2D.glsl

    //  establish our grid cell and unit position
    vec2 Pi = floor(P);
    vec2 Pf = P - Pi;

    //  calculate the hash
    vec4 Pt = vec4( Pi.xy, Pi.xy + 1.0 );
    Pt = Pt - floor(Pt * ( 1.0 / 71.0 )) * 71.0;
    Pt += vec2( 26.0, 161.0 ).xyxy;
    Pt *= Pt;
    Pt = Pt.xzxz * Pt.yyww;
    vec4 hash_x = fract( Pt * ( 1.0 / 951.135664 ) );
    vec4 hash_y = fract( Pt * ( 1.0 / 642.949883 ) );

    //  generate the 4 points
    hash_x = hash_x * 2.0 - 1.0;
    hash_y = hash_y * 2.0 - 1.0;
    const float JITTER_WINDOW = 0.25;   // 0.25 will guarentee no artifacts
    hash_x = ( ( hash_x * hash_x * hash_x ) - sign( hash_x ) ) * JITTER_WINDOW + vec4( 0.0, 1.0, 0.0, 1.0 );
    hash_y = ( ( hash_y * hash_y * hash_y ) - sign( hash_y ) ) * JITTER_WINDOW + vec4( 0.0, 0.0, 1.0, 1.0 );

    //  return the closest squared distance
    vec4 dx = Pf.xxxx - hash_x;
    vec4 dy = Pf.yyyy - hash_y;
    vec4 d = dx * dx + dy * dy;
    d.xy = min(d.xy, d.zw);
    return min(d.x, d.y) * ( 1.0 / 1.125 ); // return a value scaled to 0.0->1.0
}

// Constant function with noise
Value NoiseOnly(Value input1, Value input2, float noiseFunction, float frequency, float octaves, float lacunarity, 
	float octaveBlendFunction, float threshold, float inflection, float ridged, vec2 noiseStride, vec2 noiseOffset)
{
	float t = 0.0;
	
	for(float octave = octaves; octave > 0.5; octave -= 1.0)
	{
		float noise, t_o;
		if (noiseFunction < 0.5)
		{
			noise = 0.0;
		}
		else if (noiseFunction < 1.5)
		{
			noise = (Noise_Perlin2D(uv * frequency * noiseStride + noiseOffset) + 1) / 2.0;
		}
		else if (noiseFunction < 2.5)
		{
			noise = Noise_Cellular2D(uv * frequency * noiseStride + noiseOffset);
		}
		else
		{
			noise = 0.0;
		}
		
		t_o = noise;
		
		if (octaveBlendFunction < 0.5)	// Octave Blend Function (0: Average, 1: Difference, 2: Weighted Sum, 3: Threshold)
		{
			t += t_o / octaves;
		}
		else if (octaveBlendFunction < 1.5)
		{
			t = abs(t - t_o);
		}
		else if (octaveBlendFunction < 2.5)
		{
			t += t_o / pow(2.0, octave - octaves + 1.0);
		}
		else if (octaveBlendFunction < 3.5)
		{
			t += t_o / pow(2.0, octave - octaves + 1.0);
		}
		
		frequency *= lacunarity;
	}
	
	// Apply Threshold
	if (octaveBlendFunction < 3.5 && octaveBlendFunction > 2.5)
	{
		t = clamp((t - 0.5) / (threshold / 2.0), -0.5, 0.5) + 0.5;
	}
	
	// Apply inflector or ridged if applicable
	if (inflection > 0.5)
	{
		t = abs((t - 0.5) * 2.0);
	}
	else if (ridged > 0.5)
	{
		t = 1.0 - abs((t - 0.5) * 2.0);
	}
	
	Value result;
	result.Base = vec4(mix(input1.Base, input2.Base, t).rgb, 1.0);
	result.MSR = vec4(mix(input1.MSR, input2.MSR, t).rgb, 1.0);
	return result;
}


// One dimensional sin function with noise
Value SinX(Value input1, Value input2, float noiseFunction, float frequency, float octaves, float lacunarity, 
	float octaveBlendFunction, float threshold, float inflection, float ridged, vec2 noiseStride, vec2 noiseOffset)
{
	float t = 0.0;
	
	for(float octave = octaves; octave > 0.5; octave -= 1.0)
	{
		float noise, t_o;
		if (noiseFunction < 0.5)
		{
			noise = 0.0;
		}
		else if (noiseFunction < 1.5)
		{
			noise = (Noise_Perlin2D(uv * noiseStride + noiseOffset) + 1) / 2.0;
		}
		else if (noiseFunction < 2.5)
		{
			noise = Noise_Cellular2D(uv * noiseStride + noiseOffset);
		}
		else
		{
			noise = 0.0;
		}
		
		t_o = (1.0 + sin((uv.x + noise / 2.0) * frequency * 2.0 * M_PI)) / 2.0;
		
		if (octaveBlendFunction < 0.5)	// Octave Blend Function (0: Average, 1: Difference, 2: Weighted Sum, 3: Threshold)
		{
			t += t_o / octaves;
		}
		else if (octaveBlendFunction < 1.5)
		{
			t = abs(t - t_o);
		}
		else if (octaveBlendFunction < 2.5)
		{
			t += t_o / pow(2.0, octave - octaves + 1.0);
		}
		else if (octaveBlendFunction < 3.5)
		{
			t += t_o / pow(2.0, octave - octaves + 1.0);
		}
		
		frequency *= lacunarity;
	}
	
	// Apply Threshold
	if (octaveBlendFunction < 3.5 && octaveBlendFunction > 2.5)
	{
		t = clamp((t - 0.5) / (threshold / 2.0), -0.5, 0.5) + 0.5;
	}
	
	// Apply inflector or ridged if applicable
	if (inflection > 0.5)
	{
		t = abs((t - 0.5) * 2.0);
	}
	else if (ridged > 0.5)
	{
		t = 1.0 - abs((t - 0.5) * 2.0);
	}
	
	Value result;
	result.Base = vec4(mix(input1.Base, input2.Base, t).rgb, 1.0);
	result.MSR = vec4(mix(input1.MSR, input2.MSR, t).rgb, 1.0);
	return result;
}

void main(void)
{	
	// Read inputs into local memory
	Value values[MAX_INPUTS];
	for (int i = 0; i < InputCount; i++)
	{
		if (InputSource[i] == 0)
		{
			values[i].Base = InputValues[i].Base;
			values[i].MSR = InputValues[i].MSR;
		}
		else
		{
			values[i].Base = texture(InputTextures[i].Base, uv);
			values[i].MSR = texture(InputTextures[i].MSR, uv);
		}
	}
	
	for (int i = 0; i < CommandCount; i++)
	{
		if (Commands[i].Op.w == 0)		// Store Op1 in Sto
		{
			values[Commands[i].Op.z] = values[Commands[i].Op.x];
		}
		else if (Commands[i].Op.w == 1)	// Sto = Op1 + Op2
		{
			values[Commands[i].Op.z].Base = values[Commands[i].Op.x].Base + values[Commands[i].Op.y].Base;
			values[Commands[i].Op.z].MSR = values[Commands[i].Op.x].MSR + values[Commands[i].Op.y].MSR;			
			values[Commands[i].Op.z].Base.a = 1.0;
			values[Commands[i].Op.z].MSR.a = 1.0;
		}
		else if (Commands[i].Op.w == 2)	// Sto = Op1 - Op2
		{
			values[Commands[i].Op.z].Base = values[Commands[i].Op.x].Base - values[Commands[i].Op.y].Base;
			values[Commands[i].Op.z].MSR = values[Commands[i].Op.x].MSR - values[Commands[i].Op.y].MSR;
			values[Commands[i].Op.z].Base.a = 1.0;
			values[Commands[i].Op.z].MSR.a = 1.0;
		}
		else if (Commands[i].Op.w == 3)	// Sto(Base, MSR) = (Param[0], Param[1])
		{
			values[Commands[i].Op.z].Base = Commands[i].Params[0];
			values[Commands[i].Op.z].MSR = Commands[i].Params[1];
		}
		else if (Commands[i].Op.w == 4)	// Sto(MSR) = (Param[0])
		{
			values[Commands[i].Op.z].MSR = Commands[i].Params[0];
		}
		else if (Commands[i].Op.w == 10)	// Sto = Blend(Op1, Op2) w/ constant function (noise only)
											// Param[0].x = Noise Function (0: none, 1: Perlin2D, 2: Voronoi)
											// Param[0].y = Base Frequency
											// Param[0].z = Octaves
											// Param[0].w = Lacunarity (Octave Frequency Scale Multiple)
											// Param[1].x = Octave Blend Function (0: Average, 1: Difference, 2: Weighted Sum, 3: Threshold)
											// Param[1].y = Threshold
											// Param[1].z = Post Inflection
											// Param[1].w = Post Ridged
											// Param[2].x = Noise Stride X
											// Param[2].y = Noise Stride Y
											// Param[2].z = Noise Offset X
											// Param[2].w = Noise Offset Y
		{			
			values[Commands[i].Op.z] = NoiseOnly(
				values[Commands[i].Op.x], 
				values[Commands[i].Op.y],
				Commands[i].Params[0].x, 
				Commands[i].Params[0].y, 
				Commands[i].Params[0].z, 
				Commands[i].Params[0].w, 
				Commands[i].Params[1].x, 
				Commands[i].Params[1].y, 
				Commands[i].Params[1].z, 
				Commands[i].Params[1].w,
				vec2(Commands[i].Params[2].x, Commands[i].Params[2].y), 
				vec2(Commands[i].Params[2].z, Commands[i].Params[2].w)
			);
		}
		else if (Commands[i].Op.w == 11)	// Sto = Blend(Op1, Op2) w/ x-axis sin function
											// Param[0].x = Noise Function (0: none, 1: Perlin2D, 2: Voronoi)
											// Param[0].y = Base Frequency
											// Param[0].z = Octaves
											// Param[0].w = Lacunarity (Octave Frequency Scale Multiple)
											// Param[1].x = Octave Blend Function (0: Average, 1: Difference, 2: Weighted Sum, 3: Threshold)
											// Param[1].y = Threshold
											// Param[1].z = Post Inflection
											// Param[1].w = Post Ridged
											// Param[2].x = Noise Stride X
											// Param[2].y = Noise Stride Y
											// Param[2].z = Noise Offset X
											// Param[2].w = Noise Offset Y
		{			
			values[Commands[i].Op.z] = SinX(
				values[Commands[i].Op.x], 
				values[Commands[i].Op.y],
				Commands[i].Params[0].x, 
				Commands[i].Params[0].y, 
				Commands[i].Params[0].z, 
				Commands[i].Params[0].w, 
				Commands[i].Params[1].x, 
				Commands[i].Params[1].y, 
				Commands[i].Params[1].z, 
				Commands[i].Params[1].w,
				vec2(Commands[i].Params[2].x, Commands[i].Params[2].y), 
				vec2(Commands[i].Params[2].z, Commands[i].Params[2].w)
			);
		}
	}
	
	// Store output
	outNormal 	= EncodeNormal(normalize(viewNormal));
	outBase 	= values[0].Base;
	outMSR 		= values[0].MSR;
}