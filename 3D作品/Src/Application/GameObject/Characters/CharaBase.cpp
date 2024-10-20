#include "CharaBase.h"

#include "../../Scene/SceneManager.h"

void CharaBase::Init()
{
	if (!m_spModel)
	{
		m_gravity = 0.0f;

		m_color = { 1.f,1.f,1.f,m_alpha };

		m_mTrans = Math::Matrix::Identity;
		m_mRot = Math::Matrix::Identity;
		m_mWorld = Math::Matrix::Identity;
	}
}

void CharaBase::PostUpdate()
{
	//=========================================================
	// 当たり判定(レイ判定)・・・ここから
	//=========================================================

	KdCollider::RayInfo rayInfo;
	rayInfo.m_pos = m_pos;
	rayInfo.m_dir = Math::Vector3::Down;
	rayInfo.m_pos.y += 0.01f;

	float enableStepHigh = 0.2f;
	rayInfo.m_pos.y += enableStepHigh;

	rayInfo.m_range = m_gravity + enableStepHigh;
	rayInfo.m_type = KdCollider::TypeGround;

	std::list<KdCollider::CollisionResult> retRayList;

	for (auto& obj : SceneManager::Instance().GetObjList())
	{
		obj->Intersects(rayInfo, &retRayList);
	}

	float maxOverLap = 0;
	Math::Vector3 hitPos;
	bool ishit = false;
	for (auto& ret : retRayList)
	{
		if (maxOverLap < ret.m_overlapDistance)
		{
			maxOverLap = ret.m_overlapDistance;
			hitPos = ret.m_hitPos;
			ishit = true;
		}
	}
	if (ishit)
	{
		m_pos = hitPos + Math::Vector3{ 0.f,-0.01f,0.f };
		m_gravity = 0;
	}

	//=========================================================
	// 当たり判定(レイ判定)・・・ここまで
	//=========================================================
}

void CharaBase::DrawLit()
{
	if (!m_spModel)return;
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_spModel, m_mWorld, m_color);
}

void CharaBase::GenerateDepthMapFromLight()
{
	if (!m_spModel)return;
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_spModel, m_mWorld);
}

void CharaBase::UpdateRotate(const Math::Vector3& srcMoveVec)
{
	// 何も入力がない場合は処理しない
	if (srcMoveVec.LengthSquared() == 0) return;

	// キャラの正面方向のベクトル		// 社会に出たときはForward()を使用する
	Math::Vector3 _nowDir = GetMatrix().Backward();
	_nowDir.Normalize();

	// 移動方向のベクトル
	Math::Vector3 _targetDir = srcMoveVec;
	_targetDir.Normalize();

	float _nowAng = atan2(_nowDir.x, _nowDir.z);
	_nowAng = DirectX::XMConvertToDegrees(_nowAng);

	float _targetAng = atan2(_targetDir.x, _targetDir.z);
	_targetAng = DirectX::XMConvertToDegrees(_targetAng);

	// 角度の差分を図る
	float _betweenAng = _targetAng - _nowAng;
	if (_betweenAng > 180)
	{
		_betweenAng -= 360;
	}
	else if (_betweenAng < -180)
	{
		_betweenAng += 360;
	}
	float _rotateAng = std::clamp(_betweenAng, -8.0f, 8.0f);
	m_angle.y += _rotateAng;
}
