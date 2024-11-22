struct VS_INPUT
{
    float3 pos : POSITION;
    float3 normals : NORMAL;
    float2 uv : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float3 normals : NORMAL;
    float2 uv : TEXCOORD;
};

cbuffer TMatrix : register(b0)
{
    matrix transform;
};

cbuffer VPMatrix : register(b1)
{
    matrix view;
    matrix projection;
};


VS_OUTPUT main(VS_INPUT vsi)
{
    VS_OUTPUT vso;
    vso.pos = mul(mul(mul(float4(vsi.pos, 1.0f), transpose(transform)), transpose(view)), transpose(projection));
    vso.normals = vsi.normals;
    vso.uv = vsi.uv;
    return vso;
}