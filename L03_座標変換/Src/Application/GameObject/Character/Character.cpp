#include "Character.h"
#include "../Terrain/Terrain.h"
#include "../../main.h"

void Character::Init()
{
	if (!m_spPoly)
	{
		m_spPoly = std::make_shared<KdSquarePolygon>();
		m_spPoly->SetMaterial("Asset/Data/LessonData/Character/Hamu.png");
		m_spPoly->SetPivot(KdSquarePolygon::PivotType::Center_Bottom);
	}
	if (!m_spTex)
	{
		m_spTex = std::make_shared<KdTexture>();
		m_spTex->Load("Asset/Textures/HPBar.png");
	}
}

void Character::Update()
{
	// ①マウス座標を取得する
	GetCursorPos(&m_mousePos);
	ScreenToClient(Application::Instance().GetWindowHandle(), &m_mousePos);

	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
	{
		std::shared_ptr<KdCamera> _camera = m_wpCamera.lock();
		if (_camera)
		{
			Math::Vector3 _camPos = _camera->GetCameraMatrix().Translation();
			Math::Vector3 _rayDir = Math::Vector3::Zero;
			float _rayRange = 1000.0f;
			// ②レイの発射方向を求める(_rayDir)
			_camera->GenerateRayInfoFromClientPos(m_mousePos, _camPos, _rayDir, _rayRange);

			// ③実際にレイを飛ばして衝突位置を求める
			const std::shared_ptr<KdGameObject> _terrain = m_wpTerrain.lock();
			if (_terrain)
			{
				Math::Vector3 _endRayPos = _camPos + (_rayDir * _rayRange);
				KdCollider::RayInfo _rayInfo(KdCollider::TypeGround, _camPos, _endRayPos);

				// 実際の当たり判定の処理
				std::list<KdCollider::CollisionResult> _results;
				_terrain->Intersects(_rayInfo, &_results);

				// 結果が一つでも帰って来ていたら
				if (_results.size())
				{
					for (const KdCollider::CollisionResult& result : _results)
					{
						m_TargetPos = result.m_hitPos;
					}
				}
			}
		}
	}

	// キャラクターの移動速度(真似しちゃダメですよ)
	float moveSpd = 0.05f;
	Math::Vector3 nowPos = GetPos();
	Math::Vector3 moveVec = m_TargetPos - nowPos;
	if (moveVec.Length() < moveSpd)moveSpd = moveVec.Length();

	moveVec.Normalize();
	moveVec *= moveSpd;
	nowPos += moveVec;

	// キャラクターのワールド行列を創る処理
	m_mWorld = Math::Matrix::CreateTranslation(nowPos);
}

void Character::DrawLit()
{
	if (!m_spPoly) return;

	KdShaderManager::Instance().m_StandardShader.DrawPolygon(*m_spPoly, m_mWorld);
}

void Character::DrawSprite()
{
	// 2D座標を3D座標へ変換
	Math::Vector3 charaPos;
	Math::Vector3 nowPos = GetPos();
	m_wpCamera.lock()->ConvertWorldToScreenDetail(nowPos, charaPos);

	KdShaderManager::Instance().m_spriteShader.DrawTex(m_spTex, charaPos.x, charaPos.y);
}
