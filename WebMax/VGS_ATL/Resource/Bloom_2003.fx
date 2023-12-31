//write by NickLi
//for test hlsl
//2008-11-15

//顶点的变换矩阵
float4x4 matWorldViewProj;
//用于贴图坐标的变换矩阵
float4x4 tTex <string uiname = "Texture Transform";>;

//源贴图
texture g_txSrcColor;
//对贴图进行采样
sampler2D g_samSrcColor = sampler_state
{
    Texture = <g_txSrcColor>;
    AddressU = Clamp;
    AddressV = Clamp;
    MinFilter = Point;
    MagFilter = Linear;
    MipFilter = Linear;
};



//-------------------------------------------------------------------------
//Bloom特效
//-------------------------------------------------------------------------
//模糊程度
float	BlurValue = 30.0f;
//高亮程度
float BloomValue = 0.2f;


//像素Shader的函数, X方向Bloom
float4 PS_BloomX(float2 Tex : TEXCOORD0) : COLOR0
{
	static const int g_cKernelSize = 7;
	float define = BlurValue * 100 ;
	 
	float2 TexelKernel[g_cKernelSize] =
	{
	    { -6, 0 },
	    //{ -5, 0 },
	    { -4, 0 },
	    //{ -3, 0 },
	    { -2, 0 },
	    //{ -1, 0 },
	    {  0, 0 },
	    //{  1, 0 },
	    {  2, 0 },
	    //{  3, 0 },
	    {  4, 0 },
	    //{  5, 0 },
	    {  6, 0 }
	};

	const float BlurWeights[g_cKernelSize] =
	{
	    0.005216,
	    //0.008764,
	    0.026995,
	    //0.064759,
	    0.120985,
	    //0.176033,
	    0.199471,
	    //0.176033,
	    0.120985,
	    //0.064759,
	    0.026995,
	    //0.008764,
	    0.005216
	};

  float4 Color = 0;
  for (int i = 0; i < g_cKernelSize; i++)
  {
      Color += tex2D( g_samSrcColor, Tex + TexelKernel[i].xy/define ) * BlurWeights[i] * 2;
  }
  return Color * BloomValue;
}


//像素Shader的函数, Y方向Bloom
float4 PS_BloomY(float2 Tex : TEXCOORD0) : COLOR0
{
	static const int g_cKernelSize = 7;
	float define = BlurValue * 100 ;
	 
	float2 TexelKernel[g_cKernelSize] =
	{
	    { -6, 0 },
	    //{ -5, 0 },
	    { -4, 0 },
	    //{ -3, 0 },
	    { -2, 0 },
	    //{ -1, 0 },
	    {  0, 0 },
	    //{  1, 0 },
	    {  2, 0 },
	    //{  3, 0 },
	    {  4, 0 },
	    //{  5, 0 },
	    {  6, 0 }
	};

	const float BlurWeights[g_cKernelSize] =
	{
	    0.005216,
	    //0.008764,
	    0.026995,
	    //0.064759,
	    0.120985,
	    //0.176033,
	    0.199471,
	    //0.176033,
	    0.120985,
	    //0.064759,
	    0.026995,
	    //0.008764,
	    0.005216
	};

  float4 Color = 0;
  for (int i = 0; i < g_cKernelSize; i++)
  {
      Color += tex2D( g_samSrcColor, Tex + TexelKernel[i].yx/define ) * BlurWeights[i] * 2;
  }
  return Color * BloomValue;
}

//像素Shader的函数, XY方向Bloom
float4 PS_BloomXY(float2 Tex : TEXCOORD0) : COLOR0
{
	//方法1:上下左右各偏移三个单位，并设置不同的权重后叠加
	//需要叠加图像共12次，运算效率较低
	static const int kernelCnt = 13;
	float define = 10000.0f / BlurValue;
	 
	float2 TexelKernel[kernelCnt] =
	{
	    { -8, 0 },
	    { -7, 0 },
	    { -6, 0 },
	    { -5, 0 },
	    { -3, 0 },
	    { -1, 0 },
	    {  0, 0 },
	    {  1, 0 },
	    {  3, 0 },
	    {  5, 0 },
	    {  6, 0 },
	    {  7, 0 },
	    {  8, 0 }
	};
	
	const float BlurWeights[kernelCnt] =
	{
	    0.02,
	    0.05	,
	    0.08,
	    0.15,
	    0.25,
	    0.5,
	    0.8,
	    0.5,
	    0.25,
	    0.15,
	    0.08,
	    0.05,
	    0.02
	};

	float4 Color = 0;
	for (int i = 0; i < kernelCnt; i++)
	{
	    Color += tex2D( g_samSrcColor, Tex + TexelKernel[i].xy/define ) * BlurWeights[i];
	    Color += tex2D( g_samSrcColor, Tex + TexelKernel[i].yx/define ) * BlurWeights[i];
	}
	return Color * BloomValue;	
}

//渲染时调用的技术
technique Bloom_X
{
	pass p0
	{
		PixelShader= compile ps_2_0 PS_BloomX();
	}
}


//渲染时调用的技术
technique Bloom_Y
{
	pass p0
	{
		PixelShader= compile ps_2_0 PS_BloomY();
	}
}


//渲染时调用的技术
technique Bloom_XY
{
	pass p0
	{
		PixelShader= compile ps_2_0 PS_BloomXY();
	}
}


//-------------------------------------------------------------------------
//调节亮度和对比度
//-------------------------------------------------------------------------
float fade = 0.9f;
float Contrast = 1.3f;
float lumaswitch = 0.0f;
float3 colour = {0,0,0};
Float luma = 1.0f;
//像素Shader的函数
float4 PS_ProcessImageColor(float2 TexC: TEXCOORD0): COLOR
{
    float4 col = tex2D(g_samSrcColor, TexC) ;
    col.a=1;
     
     // Luma
 		if (col.r + col.g + col.b < luma * 3) 
 				col.a = lumaswitch; 
 		else 
 				col.a = 1-lumaswitch; //col.a-lumaswitch ;    
 		
 		// RGB +/-		
    col.rgb = colour.rgb + col.rgb ;       
		
		// contrast
  	col.rgb =  ((col.rgb -0.5)*Contrast)+0.5  ;  
  	
  	//level
  	col = col * fade  ;   
  	
    return col;
}

//渲染时调用的技术
technique GeneralImageProcess
{
	pass p0
	{
		PixelShader= compile ps_2_0 PS_ProcessImageColor();
	}
}


