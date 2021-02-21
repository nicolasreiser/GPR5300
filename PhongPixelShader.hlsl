Texture2D MainTexture;
SamplerState MainSampler;

struct Light
{
    float3 LightDirection;
    float LightIntensity;
    float4 AmbientColor;
    float4 DiffuseColor;
};

cbuffer MaterialConstantBuffer : register(b1)
{
    float4 TilingOffset;
    float4 SpecularPowerGlossiness;
};
cbuffer PixelShaderBuffer : register(b0)
{
    Light LightData;
};



struct PixelInput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float3 viewDirection : TEXCOORD1;
    
};

float4 main(PixelInput IN) : SV_TARGET
{
    float4 tex = MainTexture.Sample(MainSampler, IN.uv);
    float3 lightDirection = normalize(LightData.LightDirection);
    IN.normal = normalize(IN.normal);
    IN.viewDirection = normalize(IN.viewDirection);
    float diffuse = max(0, dot(lightDirection, IN.normal));

    float3 reflectVector = 2.0 * diffuse * IN.normal - lightDirection;
    float specular = pow(max(0, dot(reflectVector, IN.viewDirection)), 128) * 1;

    float4 color = LightData.AmbientColor ;
    color += tex * saturate(LightData.DiffuseColor * diffuse);
    color += specular * float4(1,1,1,1);
    return saturate(color);
    
    //return specular * float4(1, 1, 1, 1);

}