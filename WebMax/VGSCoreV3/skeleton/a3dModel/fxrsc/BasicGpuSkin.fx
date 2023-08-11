/*********************************************************************NVMH3****
$Revision$

Copyright NVIDIA Corporation 2007
TO THE MAXIMUM EXTENT PERMITTED BY APPLICABLE LAW, THIS SOFTWARE IS PROVIDED
*AS IS* AND NVIDIA AND ITS SUPPLIERS DISCLAIM ALL WARRANTIES, EITHER EXPRESS
OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY
AND FITNESS FOR A PARTICULAR PURPOSE.  IN NO EVENT SHALL NVIDIA OR ITS SUPPLIERS
BE LIABLE FOR ANY SPECIAL, INCIDENTAL, INDIRECT, OR CONSEQUENTIAL DAMAGES
WHATSOEVER (INCLUDING, WITHOUT LIMITATION, DAMAGES FOR LOSS OF BUSINESS PROFITS,
BUSINESS INTERRUPTION, LOSS OF BUSINESS INFORMATION, OR ANY OTHER PECUNIARY
LOSS) ARISING OUT OF THE USE OF OR INABILITY TO USE THIS SOFTWARE, EVEN IF
NVIDIA HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.


To learn more about shading, shaders, and to bounce ideas off other shader
authors and users, visit the NVIDIA Shader Library Forums at:

http://developer.nvidia.com/forums/

******************************************************************************/

#define FLIP_TEXTURE_Y
///#include "BasiceInludce.fx"
struct cTransformBuffer
{
    float4x4 matWorld;
    float4x4 matView;
    float4x4 matProject;
    float4x4 matTexture;
    float4   cameraUp;
    float4   cameraPos;
    float4   cameraDir;
    float4   cameraArg;//[Near , Far , Fov , Aspect]
};

cTransformBuffer TransformBuffer;



//变换法线的函数
float4 transNormal(float4x4 mat , float4 nr)
{
    float4 _nr = float4(nr.xyz , 0.0f);
    float4 _ret = mul(_nr  , mat );
    _ret.w = nr.w;
    return _ret;
}


//数值DIP处理函数
float4 dip_filter(float3x3 _filter ,  sampler2D _texture , float2 _xy, float2 texSize)
{
    float2 _filter_pos_delta[3][3] = 
    {
        { float2(-1.0 , -1.0) , float2(0,-1.0), float2(1.0 , -1.0) },
        { float2( 0.0 , -1.0) , float2(0, 0.0), float2(1.0 ,  0.0) },
        { float2( 1.0 , -1.0) , float2(0, 1.0), float2(1.0 ,  1.0) },
    };



    float4 final_color = float4(0.0,0.0,0.0,0.0); 
    for(int i = 0 ; i < 3 ; i ++ )
    {
        for(int j = 0 ; j < 3 ; j ++)
        {
            float2 _xy_new = float2(_xy.x + _filter_pos_delta[i][j].x , _xy.y + _filter_pos_delta[i][j].y);
            float2 _uv_new = float2(_xy_new.x/texSize.x , _xy_new.y/texSize.y);
            final_color += tex2D( _texture , _uv_new ) * _filter[i][j];
        } 
    }
    return final_color;
}



//HDR Midle-Gray Exposure re-assign
float4  xposure(float4 _color , float gray , float ex)
{
    float   b = ( 4 * ex -  1 );
    float   a = 1 - b;
    float   f = gray * ( a * gray + b );
    return   f * _color;
}




///CommonTexture
float4 COMMON_Texture(float4 _Color , float2 texCoord , sampler2D _texture )
{
    return tex2D(_texture, texCoord.xy) * _Color ;
}



///光照计算的库
#define MAX_LIGHT 4

struct cLightState
{   
    int4       nLight;  
    float4     m_Ambient [MAX_LIGHT];
    float4     m_Diffuse [MAX_LIGHT];
    float4     m_Speculer[MAX_LIGHT];
    float4     m_Position[MAX_LIGHT];
    float4     m_dir     [MAX_LIGHT];
};

cLightState xLightState;
float4 Phong_Lighting(float3 vNormal , float3 wPosition , float3 eyePosition)
{

    float4 vTotal  = float4(0.0f,0.0f,0.0f , 0.0f);
    if( xLightState.nLight.x == 0 )
       vTotal  = float4(1.0f , 1.0f , 1.0f , 1.0f);
        
    for(int i = 0 ; i < xLightState.nLight.x ; i ++ )
    {
        //xLightDesc _desc = xLightState.m_Light[i];
        float3 LightDirection = xLightState.m_Position[i].xyz - wPosition;
        float3 fvLightDirection = normalize( LightDirection );
        float3 fvNormal         = normalize( vNormal );
        float  fNDotL           = dot( fvNormal, fvLightDirection );    

        float3 fvReflection     = normalize( ( ( 2.0f * fvNormal ) * ( fNDotL ) ) - fvLightDirection ); 
        float3 fvViewDirection  = normalize( eyePosition -  wPosition);
        float  fRDotV           = max( 0.0f, dot( fvReflection, fvViewDirection ) );

        float4 fvTotalAmbient   = xLightState.m_Ambient[i]; 
        float4 fvTotalDiffuse   = fNDotL * xLightState.m_Diffuse[i]; 
        float4 fvTotalSpecular  = float4(xLightState.m_Speculer[i].xyz , 1.0f) * pow( fRDotV, xLightState.m_Speculer[i].w );
        vTotal += ( saturate( fvTotalDiffuse + fvTotalAmbient +  fvTotalSpecular ) );
    }
    //vTotal.x = xLightState.nLight.x;///3.0f;//abs(vTotal.x );
    //vTotal.y = 0.0f;//abs(vTotal.y );
    //vTotal.z = 0.0f;//abs(vTotal.z );
    vTotal.w = 1.0f;
    return vTotal;

} //Finish import lib

float4 Simple_Lighting(float4 _Color , float4 wNormal , float4    wPosition , float4 eyePositon)
{
    return _Color * Phong_Lighting(wNormal.xyz , wPosition.xyz , eyePositon.xyz );
}
///////////////////////////////////////////////////////////////////////////////



///#include "SkinAni.fx"
struct VM_SkinRet
{
    float4 Pos;
    float4 Tan;
    float4 Nor;
    float4 wPos;
    float4 wNor;
    float4 wTan;	
};


struct cBoneFrame
{       
    float4 matBone[72 * 3];
};



cBoneFrame BoneFrame;
VM_SkinRet VM_DoSkin(float4 Pos , float4 Nor , float4 Tan , float4 Weights , int4   BoneIdxs)
{
    VM_SkinRet ret;
    //????δ??????
    float vWeight[4];
    vWeight[0] = Weights.x;
    vWeight[1] = Weights.y;
    vWeight[2] = Weights.z;
    vWeight[3] = Weights.w;


    int vBoneIdxs[4];
    vBoneIdxs[0] = BoneIdxs.x;
    vBoneIdxs[1] = BoneIdxs.y;
    vBoneIdxs[2] = BoneIdxs.z;
    vBoneIdxs[3] = BoneIdxs.w;

    float4 vPos = float4(0.0f,0.0f,0.0f,0.0f);
    float4 vNor = float4(0.0f,0.0f,0.0f,0.0f);
    float4 vTan = float4(0.0f,0.0f,0.0f,0.0f);

    for(int i = 0 ; i < 4 ; i ++ )
    {
        int boneIdx = vBoneIdxs[i];
        float4x4  matPosT;
        matPosT[0] = BoneFrame.matBone[ boneIdx * 3 + 0  ];
        matPosT[1] = BoneFrame.matBone[ boneIdx * 3 + 1  ];
        matPosT[2] = BoneFrame.matBone[ boneIdx * 3 + 2  ];
        matPosT[3] = float4(0.0 , 0.0 , 0.0 , 1.0);

        float4x4  matPos   = transpose(matPosT);


        vPos += vWeight[i] * mul(  Pos  ,  matPos  );
        vNor += vWeight[i] * transNormal( matPos  ,  Nor   );
        vTan += vWeight[i] * transNormal( matPos  ,  Tan   );
    }

    //交换
    vPos = vPos.xzyw;
    vNor = vNor.xzyw;
    vTan = vTan.xzyw;

    //??δ????????????????????????
    //vPos = Pos;
    //vNor = Nor;
    //vTan = Tan;

    vPos.w = 1.0f;

    //????????????????
    ret.wPos = vPos;
    ret.wNor = vNor;
    ret.wTan = vTan;


    //????????λ??
    vPos    = mul(vPos  , TransformBuffer.matWorld  );
    vPos    = mul(vPos  , TransformBuffer.matView   );
    ret.Pos = mul(vPos  , TransformBuffer.matProject);


    //???????????λ?ú??/??????
    float4x4 matWordNormal = TransformBuffer.matWorld;
    ret.wPos = mul( ret.wPos , TransformBuffer.matWorld );
    ret.wNor = transNormal( matWordNormal  ,   ret.wNor );
    ret.wTan = transNormal( matWordNormal  ,   ret.wTan );


    //?????????λ?ú??/??????
    float4x4 matNor = TransformBuffer.matView;
    ret.Nor = transNormal( matNor , ret.wNor ); 
    ret.Tan = transNormal( matNor , ret.wTan );




    return ret;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



///VertexShader
//===============================================
//Begin import lib
//Import lib name=libMatrix
struct PS_INPUT
{
    float4 oPos      : POSITION0;
    float4 Color     : COLOR;
    float2 Tex       : TEXCOORD0;  
    float4 Pos       : TEXCOORD1;
    float4 Nor       : TEXCOORD2;     
    float4 Tan       : TEXCOORD3;      

    float4 wPosition : TEXCOORD4;
    float4 wNormal   : TEXCOORD5;  
    float4 wTangent  : TEXCOORD6;   
};


struct VS_INPUT
{
    float4 Pos      : POSITION;
    float4 Nor      : NORMAL;
    float4 Color    : COLOR;
    float4 Tan      : TANGENT;
    float4 Weights  : BLENDWEIGHT;
    int4   BoneIdxs : BLENDINDICES;
    float2 Tex      : TEXCOORD0;  

    /*
    D3DXVECTOR3     m_Position;
	D3DXVECTOR3     m_Normal;
	D3DCOLOR        m_Diffuse;
	D3DXVECTOR3     m_Tangent;
	float           m_weight[4];
	unsigned short  m_windex[4];
    */
};

//==================================================
//============================================
//Insert Node Code Node= e:\Projects\SVN\Evol3D\xGestureEngine\bin\shader\d3d9\hlsl\SkinAni.hlsl


//============================================

//============================================
//Begin Main Node'code 
PS_INPUT vs_main( VS_INPUT input )
{
    PS_INPUT output = (PS_INPUT)0;
    output.Pos   = mul( input.Pos  , TransformBuffer.matWorld  );
    output.Pos   = mul( output.Pos , TransformBuffer.matView   );
    output.Pos   = mul( output.Pos , TransformBuffer.matProject);
    output.Tex   = input.Tex;
    output.Color = input.Color.xyzw;


    float4x4 matModelView = TransformBuffer.matWorld * TransformBuffer.matView;

    //观察坐标系中的
    output.Tan   = transNormal(matModelView , input.Tan);
    output.Nor   = transNormal(matModelView , input.Nor);


    //世界坐标系中的
    output.wPosition = mul( input.Pos  , TransformBuffer.matWorld );
    output.wNormal   = transNormal( matModelView ,   input.Nor   );
    output.wTangent  = transNormal( matModelView ,   input.Tan   );

    //应用修改器 name=SkinAni 
    VM_SkinRet Node_0_Ret = VM_DoSkin(input.Pos , input.Nor , input.Color , input.Weights , input.BoneIdxs);

    output.Pos = Node_0_Ret.Pos; 
    output.Nor = Node_0_Ret.Nor; 
    output.Tan = Node_0_Ret.Tan; 
    output.wPosition = Node_0_Ret.wPos; 
    output.wNormal = Node_0_Ret.wNor; 
    output.wTangent = Node_0_Ret.wTan; 
    output.Color = input.Color; 
    output.Tex = input.Tex; 

    output.oPos = output.Pos;
    return output;
}


////PixelShader////
//===============================================
//Begin import lib
//Import lib name=libDip
float        AlphaRef = 0.0f;

#ifndef _NO_DEFAULT_TEXTURE_
sampler2D    Texture0 : register(s0);
float4       DoTexture(float2 _texLayer[1], float4 diffuse)
{
    float4 _cl = diffuse;
	{
		 float4 _texColor = tex2D(Texture0 , _texLayer[0]);
         _cl.xyz  = _cl.xyz * _texColor.xyz;
		 _cl.w    = _cl.w * _texColor.w;
	}
	return _cl;
}
#endif

//==================================================
//============================================
//Insert Node Code Node= e:\Projects\SVN\Evol3D\xGestureEngine\bin\shader\d3d9\lib\simple.texture.hlsl

//============================================


//============================================
//Begin Main Node'code 

float4 ps_main( PS_INPUT input) : COLOR0
{
    float4 vDiffuse =  float4(1.0,1.0,1.0,1.0);//input.Color ;

    //应用修改器 name=simple.texture 
    float2 _texLayer[1];
    _texLayer[0] = input.Tex;
    float4 Node_0_Ret = DoTexture(_texLayer , vDiffuse);

    //应用修改器 name=simple.Lighting 
    float4 Node_1_Ret = Simple_Lighting(Node_0_Ret , input.wNormal , input.wPosition , TransformBuffer.cameraPos);

    vDiffuse = Node_1_Ret; 
    //vDiffuse.w = 1.0f;

    if(vDiffuse.w <= 0.2f)//AlphaRef)
        discard;
    return vDiffuse;//float4(vDiffuse.x , vDiffuse.y , vDiffuse.z , 1.0f);// clamp(vDiffuse.w , 0.0  , 1.0) );
}

///// TECHNIQUES /////////////////////////////
RasterizerState DisableCulling
{
    CullMode = NONE;
};

DepthStencilState DepthEnabling
{
    DepthEnable = TRUE;
};

BlendState DisableBlend
{
    BlendEnable[0] = FALSE;
};

technique Main <string Script = "Pass=p0;";> 
{
    pass p0 <string Script = "Draw=geometry;";  >
    {
        VertexShader = compile vs_3_0 vs_main();
        //ZEnable = true;
        //ZWriteEnable = true;
        //ZFunc = LessEqual;
        //AlphaBlendEnable = false;
        //CullMode = None;
        PixelShader = compile ps_3_0 ps_main();
    }
}

/////////////////////////////////////// eof //
