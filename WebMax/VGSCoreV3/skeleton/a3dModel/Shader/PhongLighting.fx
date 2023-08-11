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
#include "BasiceInludce.fx"
#include "SkinAni.fx"
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
      //float4 Tan      : TANGENT;
      float2 Tex      : TEXCOORD0; 
};

//float4x4 matWorld;
//float4x4 matView;
//float4x4 matProject;

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
      output.Tan   = float4(1.0f,1.0f,1.0f,0.0f);//transNormal(matModelView , input.Tan);
      output.Nor   = transNormal(matModelView , input.Nor);

      //世界坐标系中的
      output.wPosition = mul( input.Pos  , TransformBuffer.matWorld );
      output.wNormal   = transNormal( matModelView ,   input.Nor   );
      output.wTangent  = float4(1.0f,1.0f,1.0f,0.0f);//transNormal( matModelView ,   input.Tan   );

      output.oPos = output.Pos;
      return output;

}



////PixelShader////
//===============================================
//Begin import lib
//Import lib name=libDip

sampler2D    DiffuseTexture : register(s0);
float        AlphaRef = 0.0f;

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
    float4 Node_0_Ret = COMMON_Texture(vDiffuse , input.Tex , DiffuseTexture);

    //应用修改器 name=simple.Lighting 
    float4 Node_1_Ret = Simple_Lighting(Node_0_Ret , input.wNormal , input.wPosition , TransformBuffer.cameraPos);

    vDiffuse = Node_1_Ret; 
    //vDiffuse.w = 1.0f;

    if(vDiffuse.w <= AlphaRef)
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
        ZEnable = true;
        ZWriteEnable = true;
        ZFunc = LessEqual;
        AlphaBlendEnable = false;
        CullMode = None;
        PixelShader = compile ps_3_0 ps_main();
    }
}

/////////////////////////////////////// eof //
