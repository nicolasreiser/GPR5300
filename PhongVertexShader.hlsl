
cbuffer UpdatedBuffer : register(b1)
{
    float4x4 WorldViewProjectionMatrix;
    float4x4 WorldMatrix;
    float3 WorldSpaceCameraPos;
    float Time;
    
};

struct VertexInput
{
    float4 position : POSITION;
    float2 uv : TEXCOORD;
    float4 normal : NORMAL;
};

struct VertexOutput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float3 viewDirection : TEXCOORD1;
};

VertexOutput main(VertexInput IN)
{
    
    //OUTPUT.position = UnityObjectToClipPos(INPUT.position); kk
    //OUTPUT.normal = UnityObjectToWorldNormal(INPUT.normal); kk

    //OUT.uv = TRANSFORM_TEX(IN.uv, _MainTex); maybe
    //INPUT.position.w = 1.0; kk
    //OUTPUT.viewDirection = (_WorldSpaceCameraPos - mul(UNITY_MATRIX_M, INPUT.position)).xyz;

    
    VertexOutput OUT;
    
    IN.position.w = 1.0f;
    OUT.position = mul(IN.position, WorldViewProjectionMatrix);
    OUT.uv = IN.uv;
    
    IN.normal.w = 1.0f;
    OUT.normal = mul(IN.normal, WorldMatrix).xyz;
    OUT.viewDirection = (WorldSpaceCameraPos - mul(WorldMatrix, IN.position).xyz);
    
    return OUT;
}