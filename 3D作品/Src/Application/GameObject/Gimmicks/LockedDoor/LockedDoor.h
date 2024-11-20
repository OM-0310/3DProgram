#pragma once

class Player;

class LockedDoor : public KdGameObject
{
public:

	LockedDoor						()				{}
	~LockedDoor						()	override	{}

	void Init						()	override;
	void Update						()	override;
	void DrawLit					()	override;
	void GenerateDepthMapFromLight	()	override;

	void Open();

	void SetPlayer(const std::shared_ptr<Player>& _spPlayer)
	{
		m_wpPlayer = _spPlayer;
	}

	const bool& GetOpeAbleFlg	() { return m_openAbleFlg; }
	const bool& GetOpenFlg		() { return m_openFlg; }
	const bool& GetLockFlg		() { return m_lockFlg; }

private:

	std::shared_ptr<KdModelWork>	m_spModel		= nullptr;					// モデル情報
	std::weak_ptr<Player>			m_wpPlayer;									// プレイヤー情報

	Math::Color						m_debugColor	= kWhiteColor;				// デバッグワイヤー用カラー

	Math::Matrix					m_mTrans		= Math::Matrix::Identity;	// 座標行列

	Math::Vector3					m_pos			= Math::Vector3::Zero;		// 座標
	Math::Vector3					m_eventPos		= Math::Vector3::Zero;		// 座標(円判定用)

	const float						m_moveMax		= 2.5f;						// Y座標のMAX値 = 2.5f
	const float						m_moveMin		= -0.9f;					// Y座標のMin値 = -0.9f
	const float						m_moveSpeed		= 0.1f;						// 移動量

	const float						m_openArea		= 1.4f;						// 扉が開閉するアクティブが起こる範囲

	bool							m_openAbleFlg	= false;					// 開錠可能フラグ
	bool							m_openFlg		= false;					// 開錠フラグ
	bool							m_lockFlg		= false;					// ロックフラグ
};
