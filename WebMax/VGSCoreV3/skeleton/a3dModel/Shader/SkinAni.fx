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
