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

	void Open	(const bool& _active)			{ m_bitsEachFlg.set(ActiveFlg, _active);			}	// 表示処理
	void Consent(const bool& _permissionFlg)	{ m_bitsEachFlg.set(PermissionFlg, _permissionFlg); }	// 表示許可処理
	void ClassDestruction() { m_isExpired = true; }

	void SetCardKey(const std::shared_ptr<CardKey>& _spCardKey)
	{
		m_wpCardKey = _spCardKey;
	}

	const bool GetActive		() const;
	const bool GetPermissionFlg	() const;

private:

	enum
	{
		ActiveFlg,		// アクティブフラグ
		PermissionFlg	// 表示許可フラグ
	};

private:

	std::weak_ptr<CardKey>		m_wpCardKey;

	static constexpr short		m_totalEachFlg	= 2;
	std::bitset<m_totalEachFlg> m_bitsEachFlg;

	static constexpr float		m_posMulti		= 2.5f;		// 座標掛け算値
	static constexpr float		m_adjustZPos	= 157.5f;	// Z座標調整
};
