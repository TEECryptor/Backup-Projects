//===============================================
//Begin import lib
//Import lib name=libMatrix
float4 transNormal(float4x4 mat , float4 nr)
{
      float4 _nr = float4(nr.xyz , 0.0f);
      float4 _ret = mul(_nr  , mat );
      _ret.w = nr.w;
      return _ret;
}//Finish import lib
//============================================

struct cTransformBuffer
{
      matrix matWorld;
      matrix matView;
      matrix matProject;
      float4 cameraUp;
      float4 cameraPos;
      float4 cameraDir;
      float4 cameraArg;//[Near , Far , Fov , Aspect]
};

cTransformBuffer TransformBuffer;

struct STATICMESHVS_INPUT
{
      float4 Pos      : POSITION;
      float4 Nor      : NORMAL;
      float4 Color    : COLOR;
      float4 Tan      : TANGENT;
      float2 Tex      : TEXCOORD;  
};


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


#define VS_INPUT STATICMESHVS_INPUT

PS_INPUT main( VS_INPUT input )
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

      output.oPos = output.Pos;
      return output;

}

