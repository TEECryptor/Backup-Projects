//write by NickLi
//for test hlsl
//2008-11-15

//����ı任����
float4x4 matWorldViewProj;
//������ͼ����ı任����
float4x4 tTex <string uiname = "Texture Transform";>;

//Դ��ͼ
texture g_txSrcColor;
//����ͼ���в���
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
//Bloom��Ч
//-------------------------------------------------------------------------
//ģ���̶�
float	BlurValue = 4.0f;
//�����̶�
float BloomValue = 0.45f;


//����Shader�ĺ���, X����Bloom
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


//����Shader�ĺ���, Y����Bloom
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

//����Shader�ĺ���, XY����Bloom
float4 PS_BloomXY(float2 Tex : TEXCOORD0) : COLOR0
{
	//����1:�������Ҹ�ƫ��������λ�������ò�ͬ��Ȩ�غ����
	//��Ҫ����ͼ��12�Σ�����Ч�ʽϵ�
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
	//����2:��ͼ������ĵ�ΪԲ�ķ�����Ϊ�µ�ͼ�񣬷Ŵ�������λ����С������λ�������ò�ͬ��Ȩ�غ����
	//��Ҫ����ͼ��6�Σ�����Ч�ʸ���, ��Ч�����Ǻܺ�
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
	
	//������ͼ����
	float2 centerUV = {0.5f, 0.5f};
	for (int i = 0; i < kernelCnt; i++)
	{
			float2 newUV = ((Tex - centerUV) * scaleWeights[i] + centerUV);
	    Color += tex2D( g_samSrcColor, newUV) * BlurWeights[i];
	 }
	return Color;
	*/
}

//��Ⱦʱ���õļ���
technique Bloom_X
{
	pass p0
	{
		PixelShader= compile ps_2_0 PS_BloomX();
	}
}


//��Ⱦʱ���õļ���
technique Bloom_Y
{
	pass p0
	{
		PixelShader= compile ps_2_0 PS_BloomY();
	}
}


//��Ⱦʱ���õļ���
technique Bloom_XY
{
	pass p0
	{
		PixelShader= compile ps_2_0 PS_BloomXY();
	}
}


//-------------------------------------------------------------------------
//�������ȺͶԱȶ�
//-------------------------------------------------------------------------
float fade = 0.9f;
float Contrast = 1.3f;
float lumaswitch = 0.0f;
float3 colour = {0,0,0};
Float luma = 1.0f;
//����Shader�ĺ���
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

//��Ⱦʱ���õļ���
technique GeneralImageProcess
{
	pass p0
	{
		PixelShader= compile ps_2_0 PS_ProcessImageColor();
	}
}


