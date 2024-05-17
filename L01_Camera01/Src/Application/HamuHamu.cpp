#include"HamuHamu.h"

HamuHamu::HamuHamu()
{
}

HamuHamu::~HamuHamu()
{
}

void HamuHamu::Init()
{
	m_spPoly = std::make_shared<KdSquarePolygon>();
	m_spPoly->SetMaterial("Asset/Data/LessonData/Character/ca.png");
	m_spPoly->SetPivot(KdSquarePolygon::PivotType::Center_Bottom);
}

void HamuHamu::Update()
{
	// キャラクターの移動速度(マネしちゃだめ)
	float moveSpd = 0.05f;
	Math::Vector3 nowPos = m_mWorld.Translation();

	// ベクトル(方向ベクトル) = 必ず「長さ(力)」が1でなければならない		ベクトルとは…矢印である
	Math::Vector3 moveVec = Math::Vector3::Zero;
	if (GetAsyncKeyState('W') & 0x8000)moveVec.z += 1.0f;
	if (GetAsyncKeyState('A') & 0x8000)moveVec.x += -1.0f;
	if (GetAsyncKeyState('S') & 0x8000)moveVec.z += -1.0f;
	if (GetAsyncKeyState('D') & 0x8000)moveVec.x += 1.0f;

	moveVec.Normalize();	// Normalize()…ベクトルの長さを調整してくれる
	moveVec *= moveSpd;
	nowPos += moveVec;

	//キャラクターのワールド行列
	m_mWorld = Math::Matrix::CreateTranslation(nowPos);
}

void HamuHamu::DrawLit()
{
	KdShaderManager::Instance().m_StandardShader.
		DrawPolygon(*m_spPoly, m_mWorld);

}
