
 // -------------------------------------------------------------------------------------------------------------------------------------
// PARAMETERS:
// -------------------------------------------------------------------------------------------------------------------------------------

//transforms
float4x4 tW: WORLD;        //the models world matrix
float4x4 tV: VIEW;         //view matrix as set via Renderer (DX9)
float4x4 tP: PROJECTION;   //projection matrix as set via Renderer (DX9)
float4x4 tWVP: WORLDVIEWPROJECTION;

texture g_txSrcColor;
float4x4 tTex <string uiname="Texture Transform";>;

sampler2D g_samSrcColor = sampler_state
{
    Texture = <g_txSrcColor>;
    AddressU = Clamp;
    AddressV = Clamp;
    MinFilter = Point;
    MagFilter = Linear;
    MipFilter = Linear;
};

float BloomScale = 1.5f;
float blur=1 ;


struct VS_OUTPUT
{
    float4 Pos  : POSITION;
    float2 TexC : TEXCOORD0;
};

VS_OUTPUT VS(
    float4 Pos  : POSITION,
    float2 TexC : TEXCOORD)
{
    //inititalize all fields of output struct with 0
    VS_OUTPUT Out = (VS_OUTPUT)0;

    //transform position
    Pos = mul(Pos, tWVP);
    //transform texturecoordinates
    TexC = mul(TexC, tTex);

    Out.Pos  = Pos;
    Out.TexC = TexC;

    return Out;
}



// ================================================================
// Pixel Shader: 	HorizontalBlur
// Desc: 		Blurs the image horizontally
// ================================================================

float4 Bloom_xPS( float2 Tex : TEXCOORD0 ) : COLOR0
{  
	static const int g_cKernelSize = 13;
	float define = blur * 100 ;
	 
	float2 TexelKernel[13] =
	{
	    { -6, 0 },
	    { -5, 0 },
	    { -4, 0 },
	    { -3, 0 },
	    { -2, 0 },
	    { -1, 0 },
	    {  0, 0 },
	    {  1, 0 },
	    {  2, 0 },
	    {  3, 0 },
	    {  4, 0 },
	    {  5, 0 },
	    {  6, 0 }
	};

	 const float BlurWeights[13] =
	{
	    0.002216,
	    0.008764,
	    0.026995,
	    0.064759,
	    0.120985,
	    0.176033,
	    0.199471,
	    0.176033,
	    0.120985,
	    0.064759,
	    0.026995,
	    0.008764,
	    0.002216
	};

  float4 Color = 0;
  for (int i = 0; i < g_cKernelSize; i++)
  {
      Color += tex2D( g_samSrcColor, Tex + TexelKernel[i].xy/define ) * BlurWeights[i];
  }
  return Color * BloomScale;
}


float4 Bloom_yPS( float2 Tex : TEXCOORD0 ) : COLOR0
{  
	static const int g_cKernelSize = 13;
	
	float define = blur * 100 ;
	float2 TexelKernel[13] =
	{
	    { -6, 0 },
	    { -5, 0 },
	    { -4, 0 },
	    { -3, 0 },
	    { -2, 0 },
	    { -1, 0 },
	    {  0, 0 },
	    {  1, 0 },
	    {  2, 0 },
	    {  3, 0 },
	    {  4, 0 },
	    {  5, 0 },
	    {  6, 0 }
	};
	
	const float BlurWeights[13] =
	{
	    0.002216,
	    0.008764,
	    0.026995,
	    0.064759,
	    0.120985,
	    0.176033,
	    0.199471,
	    0.176033,
	    0.120985,
	    0.064759,
	    0.026995,
	    0.008764,
	    0.002216
	};

  float4 Color = 0;
  for (int i = 0; i < g_cKernelSize; i++)
  {
      Color += tex2D( g_samSrcColor, Tex + TexelKernel[i].yx/define ) * BlurWeights[i];
  }
  return Color * BloomScale;
}

// ================================================================
// Technique: 	PostProcess
// Desc: 		Performs post-processing effect that converts a colored 
//		image to black and white.
// ================================================================

technique Bloom_x
{
    pass p0
    {
        VertexShader = compile vs_2_0 VS();
        PixelShader = compile ps_2_0 Bloom_xPS();
        ZEnable = false;
    }
}

technique Bloom_y
{
    pass p0
    {
        VertexShader = compile vs_2_0 VS();
        PixelShader = compile ps_2_0 Bloom_yPS();
        ZEnable = false;
    }
}
