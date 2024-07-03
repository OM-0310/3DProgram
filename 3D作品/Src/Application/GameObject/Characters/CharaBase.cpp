#include "CharaBase.h"

#include "../../Scene/SceneManager.h"

void CharaBase::Init()
{
	m_gravity	= 0.0f;

	m_mTrans	= Math::Matrix::Identity;
	m_mRot		= Math::Matrix::Identity;
	m_mWorld	= Math::Matrix::Identity;
}

void CharaBase::PostUpdate()
{

	//=========================================================
	// 当たり判定(レイ判定)・・・ここから
	//=========================================================

	KdCollider::RayInfo rayInfo;
	rayInfo.m_pos	= m_pos;
	rayInfo.m_dir	= Math::Vector3::Down;
	rayInfo.m_pos.y += 0.1f;

	//float enableStepHigh = 0.2f;
	//rayInfo.m_pos.y += enableStepHigh;

	rayInfo.m_range = m_gravity;
	rayInfo.m_type	= KdCollider::TypeGround;

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
		m_pos = hitPos + Math::Vector3(0, -0.1f, 0);
		m_gravity = 0;
	}
	//=========================================================
	// 当たり判定(レイ判定)・・・ここまで
	//=========================================================
}

void CharaBase::DrawLit()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_spModel, m_mWorld);
}

void CharaBase::GenerateDepthMapFromLight()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_spModel, m_mWorld);
}
