//===============================================
//Begin import lib
//Import lib name=libDip

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



//Import lib name=libLighting

struct xLightDesc
{
    float4 m_Ambient;
    float4 m_Diffuse;
    float4 m_Speculer;
    float4 m_Position;
    float4 m_dir;
};



struct cLightState
{   
    int4       nLight;  
    float4     m_Ambient [8];
    float4     m_Diffuse [8];
    float4     m_Speculer[8];
    float4     m_Position[8];
    float4     m_dir     [8];

};

cLightState xLightState;
float4 Phong_Lighting(float3 vNormal , float3 wPosition , float3 eyePosition)
{

    float4 vTotal  = float4(0.0f,0.0f,0.0f , 0.0f);
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
//============================================


//Insert Node Declaration Node= e:\Projects\SVN\Evol3D\xGestureEngine\bin\shader\d3d9\hlsl\simpleMesh.pixel.hlsl

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

sampler2D    DiffuseTexture : register(s0);
float        AlphaRef;

struct PS_INPUT
{      
    float4 Color     : COLOR;
    float2 Tex       : TEXCOORD0;  
    float4 Pos       : TEXCOORD1;
    float4 Nor       : TEXCOORD2; 
    float4 Tan       : TEXCOORD3;  

    float4 wPosition : TEXCOORD4;
    float4 wNormal   : TEXCOORD5;  
    float4 wTangent  : TEXCOORD6;  

};

//==================================================

//============================================

//Insert Node Code Node= e:\Projects\SVN\Evol3D\xGestureEngine\bin\shader\d3d9\lib\simple.texture.hlsl
float4 COMMON_Texture(float4 _Color , float2 texCoord , sampler2D _texture )
{
    return tex2D(_texture, texCoord.xy) * _Color ;
}


//Insert Node Code Node= e:\Projects\SVN\Evol3D\xGestureEngine\bin\shader\d3d9\lib\simple.Lighting.hlsl

float4 Simple_Lighting(float4 _Color , float4 wNormal , float4    wPosition , float4 eyePositon)
{
    return _Color * Phong_Lighting(wNormal.xyz , wPosition.xyz , eyePositon.xyz );
}

//============================================


//============================================
//Begin Main Node'code 
float4 main( PS_INPUT input) : COLOR0
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

