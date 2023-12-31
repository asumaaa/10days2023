//マテリアル
cbuffer ConstBuff : register(b0)
{
	float4 color;
	float resolution;
	float2 window;
	float startDepth;
	float strength;
};

//変形行列
cbuffer ConstBuffTransform : register(b1)
{
	matrix mat;
};

//頂点シェーダの出力構造体
struct VSOutput
{
	//システム用頂点座標
	float4 svpos : SV_POSITION;
	//uv
	float2 uv : TEXCOORD;

	float4 possm : POSITON_SM;
};