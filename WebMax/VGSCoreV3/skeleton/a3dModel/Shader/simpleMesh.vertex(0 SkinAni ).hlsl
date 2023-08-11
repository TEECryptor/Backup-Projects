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


//Insert Node Declaration Node= e:\Projects\SVN\Evol3D\xGestureEngine\bin\shader\d3d9\hlsl\simpleMesh.vertex.hlsl

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
//Insert Node Declaration Node= e:\Projects\SVN\Evol3D\xGestureEngine\bin\shader\d3d9\hlsl\SkinAni.hlsl
#define _CBUFFER_ANI_
struct SKINMESHVS_INPUT
{
      float4 Pos      : POSITION;
      float4 Nor      : NORMAL;
      float4 Color    : COLOR;
      float4 Tan      : TANGENT;
      float4 Weights  : BLENDWEIGHT;
      int4   BoneIdxs : BLENDINDICES;
      float2 Tex      : TEXCOORD;  

};

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

VM_SkinRet VM_DoSkin(float4 Pos , float4 Nor , float4 Tan , float4 Weights , int4   BoneIdxs);

#undef VS_INPUT

#define VS_INPUT SKINMESHVS_INPUT

//==================================================

//============================================

//Insert Node Code Node= e:\Projects\SVN\Evol3D\xGestureEngine\bin\shader\d3d9\hlsl\SkinAni.hlsl


VM_SkinRet VM_DoSkin(float4 Pos , float4 Nor , float4 Tan , float4 Weights , int4   BoneIdxs)
{
      VM_SkinRet ret;
      //Ŀǰ��δ��뻹û��
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
     

      //��δ�����ʱ�����˹��������Ĺ���
      //vPos = Pos;
      //vNor = Nor;
      //vTan = Tan;

      vPos.w = 1.0f;

      //�ȱ�������ռ�ķ���ֵ
      ret.wPos = vPos;
      ret.wNor = vNor;
      ret.wTan = vTan;


      //����任���λ��
      vPos    = mul(vPos  , TransformBuffer.matWorld  );
      vPos    = mul(vPos  , TransformBuffer.matView   );
      ret.Pos = mul(vPos  , TransformBuffer.matProject);


      //��������ռ��λ�úͷ�/������
      float4x4 matWordNormal = TransformBuffer.matWorld;
      ret.wPos = mul( ret.wPos , TransformBuffer.matWorld );
      ret.wNor = transNormal( matWordNormal  ,   ret.wNor );
      ret.wTan = transNormal( matWordNormal  ,   ret.wTan );


      //����۲�ռ��λ�úͷ�/������
      float4x4 matNor = TransformBuffer.matView;
      ret.Nor = transNormal( matNor , ret.wNor ); 
      ret.Tan = transNormal( matNor , ret.wTan );

      return ret;
}

//============================================


//============================================
//Begin Main Node'code 

PS_INPUT main( VS_INPUT input )
{

    PS_INPUT output = (PS_INPUT)0;
    output.Pos   = mul( input.Pos  , TransformBuffer.matWorld  );
    output.Pos   = mul( output.Pos , TransformBuffer.matView   );
    output.Pos   = mul( output.Pos , TransformBuffer.matProject);
    output.Tex   = input.Tex;
    output.Color = input.Color.xyzw;


    float4x4 matModelView = TransformBuffer.matWorld * TransformBuffer.matView;

    //�۲�����ϵ�е�
    output.Tan   = transNormal(matModelView , input.Tan);
    output.Nor   = transNormal(matModelView , input.Nor);


    //��������ϵ�е�
    output.wPosition = mul( input.Pos  , TransformBuffer.matWorld );
    output.wNormal   = transNormal( matModelView ,   input.Nor   );
    output.wTangent  = transNormal( matModelView ,   input.Tan   );

    //Ӧ���޸��� name=SkinAni 
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

