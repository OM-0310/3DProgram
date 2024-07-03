#include "TPSCamera.h"

void TPSCamera::Init()
{
	// 親クラスの初期化
	CameraBase::Init();

	// 基準点(ターゲット)からどれだけ離れているか
	m_mLocalPos = Math::Matrix::CreateTranslation(0.0f, 1.5f, -10.0f);

	// マウスカーソルを画面中央に固定する
	SetCursorPos(m_FixMousePos.x, m_FixMousePos.y);
}

void TPSCamera::Update()
{
	// ターゲットの行列(有効な場合利用する)
	Math::Matrix _targetMat = Math::Matrix::Identity;
	const std::shared_ptr<KdGameObject> _spTarget = m_wpTarget.lock();
	if (_spTarget)
	{
		//_targetMat = _spTarget->GetMatrix();
		_targetMat = Math::Matrix::CreateTranslation(_spTarget->GetPos());
	}

	// カメラの回転処理
	UpdateRotateByMouse();
	m_mRotation = GetRotationMatrix();
	m_mWorld = (m_mLocalPos * m_mRotation) * _targetMat;

	// ↓めり込み防止の為の座標補正計算↓
	// ①当たり判定(レイ判定)用の情報を作成
	KdCollider::RayInfo _rayInfo;
	// レイの発射位置を設定
	_rayInfo.m_pos = GetPos();
	// レイの発射方向
	_rayInfo.m_dir = Math::Vector3::Down;
	// レイの長さを設定
	_rayInfo.m_range = 1000.f;
	// レイの当たり判定をしたいタイプを設定
	_rayInfo.m_type = KdCollider::TypeGround;
	if (_spTarget)
	{
		Math::Vector3 _targetPos = _spTarget->GetPos();
		_targetPos.y += 0.1f;
		_rayInfo.m_dir = _targetPos - GetPos();
		_rayInfo.m_range = _rayInfo.m_dir.Length();
		_rayInfo.m_dir.Normalize();
	}

	// ②HIT判定対象オブジェクトに総当たり
	for (std::weak_ptr<KdGameObject> wpGameObj : m_wpHitObjectList)
	{
		std::shared_ptr<KdGameObject> _spGameObj = wpGameObj.lock();
		if (_spGameObj)
		{
			std::list<KdCollider::CollisionResult> _retResultList;
			_spGameObj->Intersects(_rayInfo, &_retResultList);

			// ③結果を利用して座標を補完する
			// レイに当たったリストから一番近いオブジェクトを検出
			float			_maxOverLap		= 0.f;
			Math::Vector3	_hitPos			= {};
			bool			_hit			= false;

			for (auto& ret : _retResultList)
			{
				// レイを遮断し、オーバーした長さが
				// 一番長いものを探す
				if (_maxOverLap < ret.m_overlapDistance)
				{
					_maxOverLap = ret.m_overlapDistance;
					_hitPos		= ret.m_hitPos;
					_hit		= true;
				}
			}
			// 何かしらの障害物に当たっている
			if (_hit)
			{
				Math::Vector3 _hoseiPos = _hitPos;
				_hoseiPos += _rayInfo.m_dir * 0.4f;
				SetPos(_hoseiPos);
			}
		}
	}

	//	親クラスの更新呼び出し
	CameraBase::Update();
}