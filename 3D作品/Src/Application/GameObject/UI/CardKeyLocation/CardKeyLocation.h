#pragma once

#include "../UIBase.h"

class CardKey;

class CardKeyLocation : public UIBase
{
public:

	CardKeyLocation	()				{}
	~CardKeyLocation()	override	{}

	void Init		()	override;
	void Update		()	override;
	void DrawSprite	()	override;

	void Open	(const bool _active)		{ m_active = _active; }					// 表示処理
	void Consent(const bool _permissionFlg) { m_permissionFlg = _permissionFlg; }	// 表示許可処理

	void SetCardKey(const std::shared_ptr<CardKey>& _spCardKey)
	{
		m_wpCardKey = _spCardKey;
	}

	const bool& GetActive		() const { return m_active; }
	const bool& GetPermissionFlg() const { return m_permissionFlg; }

private:

	std::weak_ptr<CardKey> m_wpCardKey;

	bool		m_active		= false;	// フラグ
	bool		m_permissionFlg	= false;	// 表示許可フラグ

	const float	m_posMulti		= 2.5f;		// 座標掛け算値
	const float	m_adjustZPos	= 157.5f;	// Z座標調整
};
