struct VSInput
{
	float3 Position : POSITION; //���W
	float3 Normal   : NORMAL;   //�@��
	float4 Color    : COLOR;	//���_�J���[
	float2 TexCoord	: TEXCOORD; //�e�N�X�`�����W
	float3 Tangent  : TANGENT;   //�ڐ��x�N�g��
    float3 AddColor	: ADD_COLOR;	//�ǉ��J���[
};

struct VSOutput
{
	float4 Position : SV_POSITION;
	float4 Color : COLOR;
	float2 TexCoord : TEXCOORD;
    float3 AddColor : ADD_COLOR;
};

cbuffer Transform : register(b0)
{
	float4x4 World : packoffset(c0); //���[���h�s��
	float4x4 View  : packoffset(c4); //�r���[�s��
	float4x4 Proj  : packoffset(c8); //�ˉe�s��
}

VSOutput main(VSInput input) 
{
	VSOutput output = (VSOutput)0;

	float4 localPos = float4(input.Position, 1.0f );
	float4 worldPos = mul(World, localPos);
	float4 viewPos = mul(View, worldPos);
	float4 projPos = mul(Proj, viewPos);

	output.Position = projPos;
	output.TexCoord = input.TexCoord;
	output.Color = input.Color;
    output.AddColor = input.AddColor;

	return output;
}