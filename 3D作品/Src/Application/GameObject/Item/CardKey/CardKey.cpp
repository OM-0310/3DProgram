﻿#include "CardKey.h"

#include "../../../Scene/SceneManager.h"

void CardKey::Init()
{
	ItemBase::SetAsset("Asset/Models/CardKey/CardKey.gltf");

	m_pos			= { -9.7f,-0.11f,18.f };

	m_color			= kWhiteColor;
	m_alphaSpeed	= 0.025f;
	m_alpha			= 1.0f;

	m_collectArea	= 1.f;

	ItemBase::Init();

	m_mTrans		= Math::Matrix::CreateTranslation(m_pos);
	m_mWorld		= m_mTrans;

	m_debugColor	= kRedColor;
	m_pDebugWire	= std::make_unique<KdDebugWireFrame>();
}

void CardKey::Update()
{
	// プレイヤーとの当たり判定処理
	ItemBase::UpdateCollision();

	// 点滅処理
	Flickering(m_alphaSpeed);
}

void CardKey::DrawLit()
{
	ItemBase::DrawLit();
}

void CardKey::DrawBright()
{
	ItemBase::DrawBright();
}

void CardKey::GenerateDepthMapFromLight()
{
	ItemBase::GenerateDepthMapFromLight();
}
