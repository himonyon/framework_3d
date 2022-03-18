struct VS_IN
{
	float4 pos : POSITION0;
	float4 nor : NORMAL0;
	float4 color : COLOR0;
	float2 texture_pos : TEXTURE0;
};

struct VS_OUT
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
	float2 texture_pos : TEXTURE0;
};

cbuffer ConstantBuffer
{
	float4x4 World;				// ワールド変換行列
	float4x4 View;				// ビュー変換行列
	float4x4 Projection;			// 透視射影変換行列
	float4 CameraPos;			// カメラ座標
	float4 LightVector;		// ライト方向
	float4 LightColor;			// ライトカラー
	float4 MaterialAmbient;	// アンビエント
	float4 MaterialDiffuse;	// ディフューズ
	float4 MaterialSpecular;	// スペキュラー
	float4 color;
}

VS_OUT main(VS_IN input)
{
	VS_OUT output;

	// ローカル座標 * ワールド座標変換行列
	output.pos = mul(input.pos, World);
	// ワールド座標 * ビュー座標変換行列
	output.pos = mul(output.pos, View);
	// ビュー座標 * プロジェクション座標変換行列
	output.pos = mul(output.pos, Projection);

	// 頂点カラー
	output.color = color;

	// Texture座標指定
	output.texture_pos = input.texture_pos;

	float4 normal;
	// 移動が計算に反映させない
	input.nor.w = 0.0;
	// 頂点の法線にワールド行列を掛け合わせて
	// ワールド座標上での法線の向きに変換する
	normal = mul(input.nor, World).xyzw;
	normal = normalize(normal);
	// saturate => 引数で指定した値を0〜1間での範囲に収める
	// dot => 内積計算
	output.color *= saturate(dot(normal, LightVector));

	return output;
}
