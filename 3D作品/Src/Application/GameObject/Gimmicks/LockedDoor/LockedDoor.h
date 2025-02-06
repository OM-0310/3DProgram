#pragma once

class Player;
class SubMissionUI;

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
	void GenerateNeedCardKeyUI();

	void SetPlayer(const std::shared_ptr<Player>& _spPlayer)
	{
		m_wpPlayer = _spPlayer;
	}

	void SetSubMissionUI(const std::shared_ptr<SubMissionUI>& _spSubMissionUI)
	{
		m_wpSubMissionUI = _spSubMissionUI;
	}

	const bool GetOpeAbleFlg	() { return m_bitsEachFlg.test(OpenAbleFlg);	}
	const bool GetOpenFlg		() { return m_bitsEachFlg.test(OpenFlg);		}
	const bool GetLockFlg		() { return m_bitsEachFlg.test(LockFlg);		}

private:

	enum
	{
		LockFlg,
		OpenAbleFlg,
		OpenFlg,
		UIFlg
	};

private:

	std::shared_ptr<KdModelWork>		m_spModel		= nullptr;					// モデル情報
	std::weak_ptr<Player>				m_wpPlayer;									// プレイヤー情報
	std::weak_ptr<SubMissionUI>			m_wpSubMissionUI;							// サブミッションUI情報

	Math::Color							m_debugColor	= kWhiteColor;				// デバッグワイヤー用カラー

	Math::Matrix						m_mTrans		= Math::Matrix::Identity;	// 座標行列

	Math::Vector3						m_pos			= Math::Vector3::Zero;		// 座標
	Math::Vector3						m_eventPos		= Math::Vector3::Zero;		// 座標(円判定用)

	static constexpr float				m_moveMax		= 2.5f;						// Y座標のMAX値 = 2.5f
	static constexpr float				m_moveMin		= -0.9f;					// Y座標のMin値 = -0.9f
	static constexpr float				m_moveSpeed		= 0.1f;						// 移動量

	static constexpr float				m_openArea		= 1.4f;						// 扉が開閉するアクティブが起こる範囲

	static constexpr short				m_totalEachFlg	= 4;
	std::bitset<m_totalEachFlg>			m_bitsEachFlg;

	static constexpr float				m_openVol		= 1.0f;
	std::shared_ptr<KdSoundInstance3D>	m_spOpenSound;
};
