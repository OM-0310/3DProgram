#include "CameraBase.h"

void CameraBase::Init()
{
	if (!m_spCamera)
	{
		m_spCamera = std::make_shared<KdCamera>();
	}

	// ↓マウスを固定させる座標値
	m_FixMousePos.x = 640;
	m_FixMousePos.y = 360;
}

void CameraBase::Update()
{
	if (!m_spCamera)return;

	m_spCamera->SetCameraMatrix(m_mWorld);
}

void CameraBase::PreDraw()
{
	if (!m_spCamera)return;

	m_spCamera->SetToShader();
}

void CameraBase::UpdateRotateByMouse()
{
	// ①マウスの現在の位置を取得
	POINT _nowPos;
	GetCursorPos(&_nowPos);

	// ②画面中央からの座分を算出
	POINT _mouseMove{};
	_mouseMove.x = _nowPos.x - m_FixMousePos.x;
	_mouseMove.y = _nowPos.y - m_FixMousePos.y;

	// ③カーソルを画面中央へ戻す
	SetCursorPos(m_FixMousePos.x, m_FixMousePos.y);

	// ④差分を利用して回転速度を設定する
	m_DegAng.x += _mouseMove.y * 0.15f;
	m_DegAng.y += _mouseMove.x * 0.15f;
	
	// 回転制御
	m_DegAng.x = std::clamp(m_DegAng.x, -45.0f, 45.0f);
}
