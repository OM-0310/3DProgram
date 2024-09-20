// カメラやライトのデータが使えるようになる
#include "../inc_KdCommon.hlsli"
#include "inc_KdLessonShader.hlsli"

// テクスチャの受け取り(画像データ※そのままでは使えない)
Texture2D g_baseTex : register(t0); // ベースカラーテクスチャ

// サンプラ(テクスチャから情報を抜き出す機能)
SamplerState g_ss : register(s0);	// 通常のテクスチャ描画用

// ピクセルシェーダ　1つ1つのピクセルがやってくる
float4 main(VSOutput In) : SV_TARGET
{
	// UV座標　…　テクスチャ座標に依存しない座標
	return g_baseTex.Sample(g_ss, In.UV) * g_BaseColor * In.Color;
}
