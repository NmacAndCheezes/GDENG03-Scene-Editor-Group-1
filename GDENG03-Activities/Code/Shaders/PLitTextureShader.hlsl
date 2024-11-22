struct PS_INPUT
{
    float4 position : SV_POSITION;
    float3 normals : NORMAL;
    float2 uv : TEXCOORD;
};

Texture2D txtr;
SamplerState smplr;

float4 main(PS_INPUT input) : SV_TARGET
{
    return txtr.Sample(smplr, input.uv);
}