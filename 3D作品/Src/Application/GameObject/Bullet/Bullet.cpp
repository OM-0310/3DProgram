#include "Bullet.h"

void Bullet::Init()
{
	if (!m_spModel)
	{
		m_spModel	= std::make_shared<KdModelData>();
		m_spModel->Load("Asset/Models/Bullet/Bullet.gltf");
	}

		m_pos		= {};
		m_moveDir	= {};

		m_lifeSpan = 180;
}

void Bullet::Update()
{
	m_pos	+= m_moveDir * m_moveSpeed;

	m_lifeSpan--;
	if (m_lifeSpan < 0)
	{
		m_isExpired = true;
	}

	m_mTrans = Math::Matrix::CreateTranslation(m_pos);
	m_mWorld = m_mRot * m_mTrans;
}

void Bullet::DrawLit()
{
	if (!m_spModel)return;
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_spModel, m_mWorld);
}

void Bullet::Shot(const Math::Vector3 _pos, const Math::Vector3 _dir)
{
	// 弾の初期座標をコピー
	m_pos = _pos;

	// 進行ベクトルをコピー
	m_moveDir = _dir;
	m_moveDir.Normalize();

	//=====================================================================================================
	// 照準へのベクトルと弾の進行ベクトル
	//=====================================================================================================

	// ①ベクトルA　・・・　弾の現在の進行ベクトル
	Math::Vector3 _vecA = m_mWorld.Backward();
	_vecA.Normalize();

	// ②ベクトルB　・・・　照準への進行ベクトル
	Math::Vector3 _vecB = m_moveDir;
	_vecB.Normalize();

	// ①と②のベクトルの内積値から角度を算出
	float _dot = _vecA.Dot(_vecB);		// 内積値を算出
	float _angle = acos(_dot);			// 内積地から2つのベクトルの角度を算出

	// ①と②のベクトルの外積を利用して、回転する為のベクトルの角度を算出
	Math::Vector3 _rotAxis = _vecA.Cross(_vecB);

	// 内積で算出した角度文、外積で算出したベクトル(回転軸)で回転する行列を作成
	// = 弾がレティクルの方向を向く
	m_mRot = Math::Matrix::CreateFromAxisAngle(_rotAxis, _angle);
}
