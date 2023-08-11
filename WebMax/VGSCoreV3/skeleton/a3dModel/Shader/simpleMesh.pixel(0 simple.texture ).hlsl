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



//Finish import lib
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
float        AlphaRef = 0.0f;

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

//============================================


//============================================
//Begin Main Node'code 
float4 main( PS_INPUT input) : COLOR0
{
    float4 vDiffuse =  float4(1.0,1.0,1.0,1.0);//input.Color ;
    //Ó¦ÓÃÐÞ¸ÄÆ÷ name=simple.texture 
    float4 Node_0_Ret = COMMON_Texture(vDiffuse , input.Tex , DiffuseTexture);
    vDiffuse = Node_0_Ret; 
    //vDiffuse.w = 1.0f;
    if(vDiffuse.w <= AlphaRef)
        discard;
    return vDiffuse;//float4(vDiffuse.x , vDiffuse.y , vDiffuse.z , 1.0f);// clamp(vDiffuse.w , 0.0  , 1.0) );

}

