#version 410

uniform sampler2D DepthTexture;

uniform mat4 ProjectionMatrix;
uniform vec2 PixelSize;
uniform mat4 ProjectionInverse;

uniform vec4 DiffuseColor;
uniform vec4 SpecularColor;
uniform float SpecularHardness;
uniform vec4 EmanativeColor;

uniform vec4 DiffuseColor2;
uniform vec4 SpecularColor2;
uniform float SpecularHardness2;
uniform vec4 EmanativeColor2;

uniform int NoiseType;
uniform float NoiseScale;
uniform vec2 NoiseOffset;
uniform float Mat1Cieling;
uniform float Mat2Floor;

layout(location = 0) in vec3 viewVertex;
layout(location = 1) in vec3 viewNormal;
layout(location = 2) in vec2 uv;

layout(location = 0) out vec4 outDiffuse;

//
//  Value Noise 2D
//  Return value range of 0.0->1.0
//
float Value2D( vec2 P )
{
    //  https://github.com/BrianSharpe/Wombat/blob/master/Value2D.glsl

    //	establish our grid cell and unit position
    vec2 Pi = floor(P);
    vec2 Pf = P - Pi;

    //	calculate the hash.
    vec4 Pt = vec4( Pi.xy, Pi.xy + 1.0 );
    Pt = Pt - floor(Pt * ( 1.0 / 71.0 )) * 71.0;
    Pt += vec2( 26.0, 161.0 ).xyxy;
    Pt *= Pt;
    Pt = Pt.xzxz * Pt.yyww;
    vec4 hash = fract( Pt * ( 1.0 / 951.135664 ) );

    //	blend the results and return
    vec2 blend = Pf * Pf * Pf * (Pf * (Pf * 6.0 - 15.0) + 10.0);
    vec4 blend2 = vec4( blend, vec2( 1.0 - blend ) );
    return dot( hash, blend2.zxzx * blend2.wwyy );
}

//
//  Perlin Noise 2D
//  Return value range of -1.0->1.0
//
float Perlin2D( vec2 P )
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
//  Simplex Perlin Noise 2D
//  Return value range of -1.0->1.0
//
float SimplexPerlin2D( vec2 P )
{
    //  https://github.com/BrianSharpe/Wombat/blob/master/SimplexPerlin2D.glsl

    //  simplex math constants
    const float SKEWFACTOR = 0.36602540378443864676372317075294;            // 0.5*(sqrt(3.0)-1.0)
    const float UNSKEWFACTOR = 0.21132486540518711774542560974902;          // (3.0-sqrt(3.0))/6.0
    const float SIMPLEX_TRI_HEIGHT = 0.70710678118654752440084436210485;    // sqrt( 0.5 )	height of simplex triangle
    const vec3 SIMPLEX_POINTS = vec3( 1.0-UNSKEWFACTOR, -UNSKEWFACTOR, 1.0-2.0*UNSKEWFACTOR );  //  simplex triangle geo

    //  establish our grid cell.
    P *= SIMPLEX_TRI_HEIGHT;    // scale space so we can have an approx feature size of 1.0
    vec2 Pi = floor( P + dot( P, vec2( SKEWFACTOR ) ) );

    // calculate the hash
    vec4 Pt = vec4( Pi.xy, Pi.xy + 1.0 );
    Pt = Pt - floor(Pt * ( 1.0 / 71.0 )) * 71.0;
    Pt += vec2( 26.0, 161.0 ).xyxy;
    Pt *= Pt;
    Pt = Pt.xzxz * Pt.yyww;
    vec4 hash_x = fract( Pt * ( 1.0 / 951.135664 ) );
    vec4 hash_y = fract( Pt * ( 1.0 / 642.949883 ) );

    //  establish vectors to the 3 corners of our simplex triangle
    vec2 v0 = Pi - dot( Pi, vec2( UNSKEWFACTOR ) ) - P;
    vec4 v1pos_v1hash = (v0.x < v0.y) ? vec4(SIMPLEX_POINTS.xy, hash_x.y, hash_y.y) : vec4(SIMPLEX_POINTS.yx, hash_x.z, hash_y.z);
    vec4 v12 = vec4( v1pos_v1hash.xy, SIMPLEX_POINTS.zz ) + v0.xyxy;

    //  calculate the dotproduct of our 3 corner vectors with 3 random normalized vectors
    vec3 grad_x = vec3( hash_x.x, v1pos_v1hash.z, hash_x.w ) - 0.49999;
    vec3 grad_y = vec3( hash_y.x, v1pos_v1hash.w, hash_y.w ) - 0.49999;
    vec3 grad_results = inversesqrt( grad_x * grad_x + grad_y * grad_y ) * ( grad_x * vec3( v0.x, v12.xz ) + grad_y * vec3( v0.y, v12.yw ) );

    //	Normalization factor to scale the final result to a strict 1.0->-1.0 range
    //	http://briansharpe.wordpress.com/2012/01/13/simplex-noise/#comment-36
    const float FINAL_NORMALIZATION = 99.204334582718712976990005025589;

    //	evaluate and return
    vec3 m = vec3( v0.x, v12.xz ) * vec3( v0.x, v12.xz ) + vec3( v0.y, v12.yw ) * vec3( v0.y, v12.yw );
    m = max(0.5 - m, 0.0);
    m = m*m;
    return dot(m*m, grad_results) * FINAL_NORMALIZATION;
}

//
//  Cellular Noise 2D
//  produces a range of 0.0->1.0
//
float Cellular2D( vec2 P )
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

//
//  TurbulentSine Noise 2D
//  produces a range of 0.0->1.0
//
float TurbulentSineX( vec2 P )
{
	float xN = (Perlin2D( vec2(P.x, 0) * 4.0 ) + 1.0) * 0.5;
	float turb = (Perlin2D( P * 8.0 ) + 1.0) * 0.5;
	float xyValue = P.x * xN * 0.5 + P.x * xN * P.y * 0.05 + turb * 0.2;
    float sineValue = abs(sin(xyValue * 3.14159));
	return sineValue;
}

void main(void)
{
	vec3 viewVec = viewVertex;
	
	// Extract buffered pixel position
	vec3 pos 		= vec3((gl_FragCoord.x * PixelSize.x), 
						   (gl_FragCoord.y * PixelSize.y), 0.0);
	pos.z           = texture(DepthTexture, pos.xy).r;
	vec4 clip       = ProjectionInverse * vec4(pos * 2.0 - 1.0, 1.0);
	pos             = clip.xyz / clip.w;
	
	// Discard fragment if behind other geometry
	if(length(viewVec) > length(pos))
	{
		discard;
	}
	
	float t = 0;
	if (NoiseType == 0)
	{
	}
	else if (NoiseType == 1)
	{
		t = Value2D(uv * NoiseScale + NoiseOffset);
	}
	else if (NoiseType == 2)
	{
		t = (Perlin2D(uv * NoiseScale + NoiseOffset) + 1.0) * 0.5;
	}
	else if (NoiseType == 3)
	{
		t = (SimplexPerlin2D(uv * NoiseScale + NoiseOffset) + 1.0) * 0.5;
	}
	else if (NoiseType == 4)
	{
		t = Cellular2D(uv * NoiseScale + NoiseOffset);
	}
	else if (NoiseType == 5)
	{
		t = TurbulentSineX(uv * NoiseScale + NoiseOffset);
	}
	
	if (t < Mat1Cieling)
		t = 0.0;
	else if (t > Mat2Floor)
		t = 1.0;	
		
	outDiffuse = mix(DiffuseColor, DiffuseColor2, t);
}