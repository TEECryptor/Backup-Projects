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
float	BlurValue = 4.0f;
//高亮程度
float BloomValue = 0.45f;


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
	static const int kernelCnt = 7;
	float define = BlurValue * 100 ;
	 
	float2 TexelKernel[kernelCnt] =
	{
	    //{ -8, 0 },
	    { -6, 0 },
	    //{ -6, 0 },
	    { -4, 0 },
	    //{ -4, 0 },
	    { -2, 0 },
	    {  0, 0 },
	    {  2, 0 },
	    //{  4, 0 },
	    {  4, 0 },
	    //{  6, 0 },
	    {  6, 0 },
	    //{  8, 0 }
	};
	
	const float BlurWeights[kernelCnt] =
	{
	    //0.005216,
	    0.01764,
	    //0.026995,
	    0.064759,
	    //0.100985,
	    0.186033,
	    0.85,
	    0.186033,
	    //0.100985,
	    0.064759,
	    //0.026995,
	    0.01764,
	    //0.005216
	};

	float4 Color = 0;
	for (int i = 0; i < kernelCnt; i++)
	{
	    Color += tex2D( g_samSrcColor, Tex + TexelKernel[i].xy/define ) * BlurWeights[i];
	    Color += tex2D( g_samSrcColor, Tex + TexelKernel[i].yx/define ) * BlurWeights[i];
	}
	return Color * BloomValue;
	

	/*
	//方法2:以图像的中心点为圆心放缩成为新的图像，放大三个单位，缩小三个单位，并设置不同的权重后叠加
	//需要叠加图像共6次，运算效率更高, 但效果不是很好
	float4 Color = 0;
	static const int kernelCnt = 6;
	const float scaleWeights[kernelCnt] = 
	{
			0.95, 
			0.965, 
			0.975, 
			1.025, 
			1.035, 
			1.05
	};
	
	const float BlurWeights[kernelCnt] =
	{
	    0.018764,
	    0.094759,
	    0.376033,
	    0.376033,
	    0.094759,
	    0.018764
	};
	
	//计算贴图坐标
	float2 centerUV = {0.5f, 0.5f};
	for (int i = 0; i < kernelCnt; i++)
	{
			float2 newUV = ((Tex - centerUV) * scaleWeights[i] + centerUV);
	    Color += tex2D( g_samSrcColor, newUV) * BlurWeights[i];
	 }
	return Color;
	*/
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


