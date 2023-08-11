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
