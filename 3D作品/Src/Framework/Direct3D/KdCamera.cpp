#include "KdCamera.h"


// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// カメラ情報(ビュー・射影行列など)をシェーダへ転送
// ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
// CPU側に用意してあるシェーダ情報格納用コンテナへデータをコピー
// コンテナをシェーダー(GPU)に送信する
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdCamera::SetToShader() const
{
	// カメラの情報をGPUへ転送
	KdShaderManager::Instance().WriteCBCamera(m_mCam, m_mProj);

	// ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
	// 被写界深度（DepthOfField）加工に使うの情報も更新
	float nearClippingDistance = -(m_mProj._43 / m_mProj._33);
	float farClippingDistance = - (m_mProj._43 / (m_mProj._33 - 1));
	float viewRange = farClippingDistance - nearClippingDistance;

	// フォーカスを合わせる焦点距離をコピー
	KdShaderManager::Instance().m_postProcessShader.SetNearClippingDistance(nearClippingDistance);
	KdShaderManager::Instance().m_postProcessShader.SetFarClippingDistance(farClippingDistance);
	KdShaderManager::Instance().m_postProcessShader.SetFocusDistance((m_focusDistance - nearClippingDistance) / viewRange);
	KdShaderManager::Instance().m_postProcessShader.SetFocusRange(m_focusForeRange / viewRange, m_focusBackRange / viewRange);
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// 射影行列の設定：各種パラメータから射影行列を生成して保持する
// ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
// 引数：（ fov：視野角 ）（ maxRange：描画する最長距離 ）（ minRange：描画する最短距離 ）（ aspectRatio：画面の縦横幅の比率 ）
// 視野角以外のパラメータはデフォルト引数が設定されているため、省略可能
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdCamera::SetProjectionMatrix(float fov, float maxRange, float minRange, float aspectRatio)
{
	float aspect = aspectRatio;

	// アスペクト比が不正だった場合
	if (aspect <= 0)
	{
		// 自動的にバックバッファからアスペクト比を求める
		if (KdDirect3D::Instance().GetBackBuffer())
		{
			aspect = KdDirect3D::Instance().GetBackBuffer()->GetAspectRatio();
		}
		// バックバッファが生成されてすらいな状況なら射影行列をセットしない
		else
		{
			return;
		}
	}

	m_mProj = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(fov), aspect, minRange, maxRange);

	SetProjectionMatrix(m_mProj);
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// 射影行列の設定：既存の射影行列をコピーする
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdCamera::SetProjectionMatrix(const DirectX::SimpleMath::Matrix& rProj)
{
	m_mProj = rProj;
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// 被写界深度の情報を設定
// ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
// カメラから見て焦点を当てる距離の設定
// ぼかさずに描画する焦点エリアを前後それぞれ別の距離に設定可能
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdCamera::SetFocus(float focusDist, float focusForeRange, float focusBackRange)
{
	m_focusDistance = focusDist;
	m_focusForeRange = focusForeRange;
	m_focusBackRange = focusBackRange;
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// クライアント座標（2D）から3Dワールド座標を求める用のレイ情報を生成
// ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
// マウスポインタの2D位置にある3Dオブジェクトを選択する時などに使用する
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdCamera::GenerateRayInfoFromClientPos(const POINT& clientPos, Math::Vector3& rayPos, Math::Vector3& rayDir, float& rayRange)
{
	// レイ判定の最遠座標
	Math::Vector3 farPos;

	// 2D座標を3Dワールド座標に変換する関数を使ってレイの発射座標と最遠座標を求める
	KdDirect3D::Instance().ClientToWorld(clientPos, 0.0f, rayPos, m_mCam, m_mProj);
	KdDirect3D::Instance().ClientToWorld(clientPos, 1.0f, farPos, m_mCam, m_mProj);

	// 目的地 - 出発地点 でレイを飛ばす方向を求める
	rayDir = farPos - rayPos;

	// 2点間の距離を求めてレイの判定距離を求める
	rayRange = rayDir.Length();

	// 正規化（長さを1.0fに）して方向ベクトルに変換する
	rayDir.Normalize();
}

// ワールド座標(3D座標)をスクリーン座標(2D座標)に変換する
void KdCamera::ConvertWorldToScreenDetail(const Math::Vector3& pos, Math::Vector3& result)
{
	// ビューポートを取得する
	Math::Viewport vp;
	KdDirect3D::Instance().CopyViewportInfo(vp);

	// ①ワールド変換行列×ビュー行列×射影行列
	Math::Matrix world = Math::Matrix::CreateTranslation(pos);
	Math::Matrix wvp = world * GetCameraViewMatrix() * GetProjMatrix();

	// ②奥行情報(w = ._44で割る必要がある)
	wvp._41 /= wvp._44;
	wvp._42 /= wvp._44;
	wvp._43 /= wvp._44;

	// 射影行列系での2D(みたいな)座標
	// ↑-1~1の範囲の座標の事
	Math::Vector3 localPos = wvp.Translation();

	// ここで幅や高さを考慮して計算する(これで正確なスクリーン座標になる)
	result.x = localPos.x * (vp.width * 0.5f);
	result.y = localPos.y * (vp.height * 0.5f);
	result.z = wvp._44;
}

void KdCamera::PerformFrustumCulling(const std::vector<Math::Vector3>& objectPositions, std::vector<Math::Vector3>& visibleObjects)
{
	visibleObjects.clear();

	for (const auto& pos : objectPositions)
	{
		bool inside = true;

		for (const auto& plane : m_frustumPlanes)
		{
			// 平面と点の関係を計算
			if (plane.DotCoordinate(pos) < 0)
			{
				inside = false;
				break;
			}
		}

		if (inside)
		{
			visibleObjects.push_back(pos);
		}
	}
}

void KdCamera::UpdateFrustum()
{
	// ビュー・プロジェクション行列を計算
	Math::Matrix viewProj = m_mView * m_mProj;

	// 視錐台の6つの平面を定義
	m_frustumPlanes[0] = Math::Plane(
		viewProj._14 + viewProj._11, viewProj._24 + viewProj._21,
		viewProj._34 + viewProj._31, viewProj._44 + viewProj._41); // 左
	m_frustumPlanes[1] = Math::Plane(
		viewProj._14 - viewProj._11, viewProj._24 - viewProj._21,
		viewProj._34 - viewProj._31, viewProj._44 - viewProj._41); // 右
	m_frustumPlanes[2] = Math::Plane(
		viewProj._14 - viewProj._12, viewProj._24 - viewProj._22,
		viewProj._34 - viewProj._32, viewProj._44 - viewProj._42); // 上
	m_frustumPlanes[3] = Math::Plane(
		viewProj._14 + viewProj._12, viewProj._24 + viewProj._22,
		viewProj._34 + viewProj._32, viewProj._44 + viewProj._42); // 下
	m_frustumPlanes[4] = Math::Plane(
		viewProj._13, viewProj._23, viewProj._33, viewProj._43);                                   // 近い
	m_frustumPlanes[5] = Math::Plane(
		viewProj._14 - viewProj._13, viewProj._24 - viewProj._23,
		viewProj._34 - viewProj._33, viewProj._44 - viewProj._43); // 遠い

	// 各平面を正規化
	for (auto& plane : m_frustumPlanes)
	{
		plane.Normalize();
	}
}
