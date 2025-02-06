#pragma once

#include "../UIBase.h"

class SecretFile;

class SecretFileLocation : public UIBase
{
public:

	SecretFileLocation	()				{}
	~SecretFileLocation	()	override	{}

	void Init			()	override;
	void Update			()	override;
	void DrawSprite		()	override;

	void Open	(const bool _active)		{ m_bitsEachFlg[ActiveFlg] = _active; }					// 表示処理
	void Consent(const bool _permissionFlg) { m_bitsEachFlg[PermissionFlg] = _permissionFlg; }	// 表示許可処理

	void SetSecretFile(const std::shared_ptr<SecretFile>& _spSecretFile)
	{
		m_wpSecretFile = _spSecretFile;
	}

	const bool GetActive		() const { return m_bitsEachFlg.test(ActiveFlg);		}
	const bool GetPermissionFlg	() const { return m_bitsEachFlg.test(PermissionFlg);	}

private:

	enum
	{
		ActiveFlg,
		PermissionFlg
	};

private:

	std::weak_ptr<SecretFile>	m_wpSecretFile;

	static constexpr short		m_totalEachFlg = 2;
	std::bitset<m_totalEachFlg>	m_bitsEachFlg;

	const float					m_posMulti		= 2.5f;		// 座標掛け算値
	const float					m_adjustZPos	= 157.5f;	// Z座標調整
};
