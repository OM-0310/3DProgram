#include "TPSCamera.h"

#include "../../Characters/Player/Player.h"

void TPSCamera::Init()
{
	// 親クラスの初期化呼び出し
	CameraBase::Init();

	//m_basePoint		= m_tpsRBasePoint;
	m_basePoint = {};

	m_camType		= CameraType::TpsR;
	m_pastCamType	= m_camType;

	SetCursorPos(m_FixMousePos.x, m_FixMousePos.y);
}

void TPSCamera::Update()
{
	switch (m_camType)
	{
	//case TPSCamera::CameraType::Fps:
	//	m_basePoint = m_fpsBasePoint;
	//	break;
	case TPSCamera::CameraType::TpsR:
		m_basePoint = m_tpsRBasePoint;
		break;
	case TPSCamera::CameraType::TpsL:
		m_basePoint = m_tpsLBasePoint;
		break;
	case TPSCamera::CameraType::AimR:
		m_basePoint = m_aimRBasePoint;
		break;
	case TPSCamera::CameraType::AimL:
		m_basePoint = m_aimLBasePoint;
		break;
	case TPSCamera::CameraType::SitR:
		m_basePoint = m_sitRBasePoint;
		break;
	case TPSCamera::CameraType::SitL:
		m_basePoint = m_sitLBasePoint;
		break;
	case TPSCamera::CameraType::SitAimR:
		m_basePoint = m_sitAimRPoint;
		break;
	case TPSCamera::CameraType::SitAimL:
		m_basePoint = m_sitAimLPoint;
		break;
	default:
		break;
	}

	// ターゲットの行列(有効な場合利用する)
	Math::Matrix								targetMat = Math::Matrix::Identity;
	const std::shared_ptr<const Player>	spPlayer = m_wpPlayer.lock();
	if (spPlayer)
	{
		targetMat = Math::Matrix::CreateTranslation(spPlayer->GetPos());
	}

	// カメラの回転
	UpdateRotateByMouse();
	m_mRot		= GetRotationMatrix();
	m_mLocalPos = Math::Matrix::CreateTranslation(m_basePoint);
	m_mWorld	= (m_mLocalPos * m_mRot) * targetMat;

	UpdateCollision();

	CameraBase::Update();
}

void TPSCamera::UpdateCollision()
{
	const std::shared_ptr<Player> spPlayer = m_wpPlayer.lock();

	KdCollider::RayInfo ray;
	ray.m_pos = GetPos();
	ray.m_dir = Math::Vector3::Down;
	ray.m_range = 1000.0f;
	ray.m_type = KdCollider::TypeGround;

	if (spPlayer)
	{
		Math::Vector3 targetPos = spPlayer->GetPos();
		targetPos.y += 0.1f;
		ray.m_dir = targetPos - GetPos();
		ray.m_range = ray.m_dir.Length();
		ray.m_dir.Normalize();
	}

	// ②HIT判定対象オブジェクトに総当たり
	for (std::weak_ptr<KdGameObject> wpGameObj : m_wpHitObjectList)
	{
		std::shared_ptr<KdGameObject> spGameObj = wpGameObj.lock();
		if (spGameObj)
		{
			std::list<KdCollider::CollisionResult> retResultList;
			spGameObj->Intersects(ray, & retResultList);

			// ③結果を利用して座標を補完する
			// レイに当たったリストから一番近いオブジェクトを検出
			float			maxOverLap = 0.f;
			Math::Vector3	hitPos = {};
			bool			hit = false;

			for (auto& ret : retResultList)
			{
				// レイを遮断し、オーバーした長さが
				// 一番長いものを探す
				if (maxOverLap < ret.m_overlapDistance)
				{
					maxOverLap = ret.m_overlapDistance;
					hitPos = ret.m_hitPos;
					hit = true;
				}
			}
			// 何かしらの障害物に当たっている
			if (hit)
			{
				Math::Vector3 hoseiPos = hitPos + Math::Vector3{ 0.0f,1.0f,0.1f };
				//_hoseiPos += _rayInfo.m_dir * 0.4f;
				SetPos(hoseiPos);
			}
		}
	}
}

void TPSCamera::ChangeTPSR()
{
	m_camType		= CameraType::TpsR;
	m_pastCamType	= CameraType::TpsR;
}

void TPSCamera::ChangeTPSL()
{
	m_camType		= CameraType::TpsL;
	m_pastCamType	= CameraType::TpsL;
}

void TPSCamera::ChangeAimR()
{
	m_camType		= CameraType::AimR;
	m_pastCamType	= CameraType::TpsR;
}

void TPSCamera::ChangeAimL()
{
	m_camType		= CameraType::AimL;
	m_pastCamType	= CameraType::TpsL;
}

void TPSCamera::ChangeSitR()
{
	m_camType		= CameraType::SitR;
	m_pastCamType	= CameraType::SitR;
}

void TPSCamera::ChangeSitL()
{
	m_camType		= CameraType::SitL;
	m_pastCamType	= CameraType::SitL;
}

void TPSCamera::ChangeSitAimR()
{
	m_camType		= CameraType::SitAimR;
	m_pastCamType	= CameraType::SitR;
}

void TPSCamera::ChangeSitAimL()
{
	m_camType		= CameraType::SitAimL;
	m_pastCamType	= CameraType::SitL;
}
