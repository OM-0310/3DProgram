#pragma once
#include "../CharaBase.h"

class TPSCamera;
class Player_Main;
class Player_Disarm;
class Player_Disarm_Pistol;
class Player_Ready_Pistol;
class Enemy_1;
class Enemy_2;
class Enemy_3;
class LockedDoor;
class CardKey;
class Goal;
class SecretFile;
class Reticle;
class RestraintUI;
class KillUI;
class InterrogationUI;
class MiniMapUI;
class MiniMapUIBack;
class MainMissionUI;
class SubMissionUI;
class CurrentLocation;
class CardKeyLocation;
class SecretFileLocation;
class PlayerStateBase;

class Player : public CharaBase
{
public:

	enum KeyFlgType
	{
		KillKey			,	// 0:殺害キー
		InterKey		,	// 1:尋問キー
		OpenMapKey		,	// 2:マップ展開キー
		ShotKey			,	// 3:弾発射キー
		ReloadKey		,	// 4:リロードキー
		CollectKey		,	// 5:アイテム回収キー
		ChangeKey		,	// 6:視点切り替えキー
		PosKey			,	// 7:姿勢切り替えキー
	};

	enum SituationType
	{
		Idle			= 1 << 0,	// 停止状態
		Run				= 1 << 1,	// 走行状態
		Walk			= 1 << 2,	// 歩行状態
		Ready			= 1 << 3,	// 構え状態
		Shot			= 1 << 4,	// 銃発射状態
		Reload			= 1 << 5,	// リロード状態
		Restraint		= 1 << 6,	// 拘束状態
		Restraint_Idle	= 1 << 7,	// 拘束停止状態
		Kill			= 1 << 8,	// 処刑状態
		Death			= 1 << 9	// 死亡状態
	};

	enum class PostureType
	{
		Stand,		// 直立
		Sit,		// しゃがみ
		Creep		// 匍匐
	};

	enum class ItemCollectType
	{
		NoCollect,			// アイテム未保持
		CardKeyCollect,		// カードキー保持
		SecretFileCollect	// 機密ファイル保持
	};

	Player						()				{}
	~Player						()	override	{ Release(); }

	void Init					()	override;
	void Update					()	override;
	void PostUpdate				()	override;

	void UpdateCollision		(); // 各当たり判定処理
	void OpneMapProc			();	// マップ展開処理
	void CollectItemProc		();	// アイテム回収処理
	void AutoReloadProc			();	// 自動リロード処理
	void ChanegeViewPointProc	();	// 視点切り替え処理
	void RestraintProc			();	// 拘束処理
	void DeathProc				();	// 死亡処理

	// アニメーション切り替え処理
	void ChangeAnimation(const std::string& _animeName, bool _isLoop = true, float _time = 0.0f);

	// クリアフラグセット
	void SetFeedOutFlg(const bool& _feedOutFlg) { m_bitsEachFlg[FeedOutFlg] = _feedOutFlg; }

	void SetCamera					// カメラ情報セット		
	(const std::shared_ptr<TPSCamera>& _spCamera) { m_wpCamera = _spCamera; }

	void SetPlayer_Main				// プレイヤー上半身情報セット
	(std::shared_ptr<Player_Main>& _spPlayer_Main) { m_wpPlayer_Main = _spPlayer_Main; }

	void SetPlayer_Disarm			// 拳銃ホルダー情報セット
	(std::shared_ptr<Player_Disarm>& _spPlayer_Disarm) { m_wpPlayer_Disarm = _spPlayer_Disarm; }
		
	void SetPlayer_Disarm_Pistol	// ホルダーにしまわれている拳銃情報セット
	(std::shared_ptr<Player_Disarm_Pistol>& _spPlayer_Disarm_Pistol) { m_wpPlayer_Disarm_Pistol = _spPlayer_Disarm_Pistol; }

	void SetPlayer_Ready_Pistol		// 構え時の銃情報セット
	(const std::shared_ptr<Player_Ready_Pistol>& _spPlayer_Ready_Pistol) { m_wpPlayer_Ready_Pistol = _spPlayer_Ready_Pistol; }

	void SetEnemy_1					// 敵1情報セット
	(const std::shared_ptr<Enemy_1>& _spEnemy_1) { m_wpEnemy_1 = _spEnemy_1; }

	void SetEnemy_2					// 敵2情報セット
	(const std::shared_ptr<Enemy_2>& _spEnemy_2) { m_wpEnemy_2 = _spEnemy_2; }

	void SetEnemy_3					// 敵2情報セット
	(const std::shared_ptr<Enemy_3>& _spEnemy_3) { m_wpEnemy_3 = _spEnemy_3; }

	void SetCardKey					// カードキー情報セット
	(const std::shared_ptr<CardKey>& _spCard) { m_wpCard = _spCard; }

	void SetDoor					// 鍵のかかったドア情報セット
	(const std::shared_ptr<LockedDoor>& _spLockDoor) { m_wpLockDoor = _spLockDoor; }

	void SetSecretFile				// 機密ファイル情報セット
	(const std::shared_ptr<SecretFile>& _spFile) { m_wpFile = _spFile; }

	void SetGoal					// ゴール情報セット
	(const std::shared_ptr<Goal>& _spGoal) { m_wpGoal = _spGoal; }

	void SetReticle					// レティクル情報セット
	(const std::shared_ptr<Reticle>& _spReticle) { m_wpReticle = _spReticle; }

	void SetRestraintUI				// 拘束UI情報セット
	(const std::shared_ptr<RestraintUI>& _spRestraintUI) { m_wpRestraintUI = _spRestraintUI; }

	void SetKillUI					// 殺害UI情報セット
	(const std::shared_ptr<KillUI>& _spKillUI) { m_wpKillUI = _spKillUI; }

	void SetInterrogationUI			// 尋問UI情報セット
	(const std::shared_ptr<InterrogationUI>& _spInterrogationUI) { m_wpInterrogationUI = _spInterrogationUI; }

	void SetMiniMapUI				// ミニマップ情報セット
	(const std::shared_ptr<MiniMapUI>& _spMiniMapUI) { m_wpMiniMapUI = _spMiniMapUI; }

	void SetMiniMapUIBack			// ミニマップ背景情報セット
	(const std::shared_ptr<MiniMapUIBack> _spMiniMapUIBack) { m_wpMiniMapUIBack = _spMiniMapUIBack; }

	void SetMainMissionUI			// メインミッションUI情報セット
	(const std::shared_ptr<MainMissionUI>& _spMainMissionUI) { m_wpMainMissionUI = _spMainMissionUI; }

	void SetSubMissionUI			// サブミッションUI情報セット
	(const std::shared_ptr<SubMissionUI>& _spSubMissionUI) { m_wpSubMissionUI = _spSubMissionUI; }

	void SetCurrentLocation			// 現在地UI情報セット
	(const std::shared_ptr<CurrentLocation>& _spCurrentLocation) { m_wpCurrentLocation = _spCurrentLocation; }

	void SetCardKeyLocation			// カードキー位置情報セット
	(const std::shared_ptr<CardKeyLocation>& _spCardKeyLocation) { m_wpCardKeyLocation = _spCardKeyLocation; }

	void SetSecretFileLocation		// 機密ファイル位置情報セット
	(const std::shared_ptr<SecretFileLocation>& _spSecretFileLocation) { m_wpSecretFileLocation = _spSecretFileLocation; }

	const Math::Matrix&		GetRotateMat		()			{ return m_mRot;						}
	const ItemCollectType&	GetItemCollType		() const	{ return m_itemCollType;				}
	const UINT&				GetSituationType	() const	{ return m_sType;						}
	const PostureType&		GetPostureType		() const	{ return m_posType;						}
	const Math::Vector3&	GetDiffVec			() const	{ return m_diffVec;						}
	const float&			GetTightArea		() const	{ return m_tightArea;					}
	const float&			GetAnimeSpeed		() const	{ return m_animeSpeed;					}
	const bool				GetFeedOutFlg		() const	{ return m_bitsEachFlg.test(FeedOutFlg);}
	const bool				GetDeathFlg			() const	{ return m_bitsEachFlg.test(DeathFlg);	}

private:

	enum
	{
		DeathFlg,
		ExeFlg,
		FeedOutFlg,
		MiniMapFlg,
		RestEnemy1Flg,
		RestEnemy2Flg,
		RestEnemy3Flg,
	};

private:

	std::weak_ptr<TPSCamera>			m_wpCamera;					// カメラ情報

	std::weak_ptr<Player_Main>			m_wpPlayer_Main;			// プレイヤー本体情報
	std::weak_ptr<Player_Disarm>		m_wpPlayer_Disarm;			// プレイヤー(武装解除)情報 
	std::weak_ptr<Player_Disarm_Pistol> m_wpPlayer_Disarm_Pistol;	// 銃(武装解除)情報 
	std::weak_ptr<Player_Ready_Pistol>	m_wpPlayer_Ready_Pistol;	// 銃(武装)情報 

	std::weak_ptr<Enemy_1>				m_wpEnemy_1;				// 敵1情報
	std::weak_ptr<Enemy_2>				m_wpEnemy_2;				// 敵2情報
	std::weak_ptr<Enemy_3>				m_wpEnemy_3;				// 敵3情報

	std::weak_ptr<Reticle>				m_wpReticle;				// レティクル情報
	std::weak_ptr<RestraintUI>			m_wpRestraintUI;			// 拘束UI情報
	std::weak_ptr<KillUI>				m_wpKillUI;					// 殺害UI情報
	std::weak_ptr<InterrogationUI>		m_wpInterrogationUI;		// 尋問UI情報
	std::weak_ptr<MiniMapUI>			m_wpMiniMapUI;				// ミニマップ情報
	std::weak_ptr<MiniMapUIBack>		m_wpMiniMapUIBack;			// ミニマップ背景情報
	std::weak_ptr<MainMissionUI>		m_wpMainMissionUI;			// メインミッションUI情報
	std::weak_ptr<SubMissionUI>			m_wpSubMissionUI;			// サブミッションUI情報
	std::weak_ptr<CurrentLocation>		m_wpCurrentLocation;		// 現在地UI情報
	std::weak_ptr<CardKeyLocation>		m_wpCardKeyLocation;		// カードキー位置UI情報
	std::weak_ptr<SecretFileLocation>	m_wpSecretFileLocation;		// 機密ファイルUI情報
	std::weak_ptr<LockedDoor>			m_wpLockDoor;				// ドア情報
	std::weak_ptr<CardKey>				m_wpCard;					// カードキー情報
	std::weak_ptr<SecretFile>			m_wpFile;					// 機密データ情報
	std::weak_ptr<Goal>					m_wpGoal;					// ゴール情報

	std::shared_ptr<KdAnimator>			m_spAnimator;				// アニメーション
	float								m_nowAnimeFrm	= 0.0f;		// 現在のアニメーションフレーム
	float								m_animFrmSpd	= 1.0f;		// アニメーションの1フレーム(blender側では24fpsでのアニメーション実装のため)
	float								m_animeSpeed	= 1.0f;		// アニメーション速度
	const float							m_normalAnimSpd = 1.0f;		// 基本のアニメーション速度 1.0f
	const float							m_runAnimSpd	= 1.2f;		// 走行時のアニメーション速度 1.2f

	const float							m_reloadFrmMax	= 60.0f;	// リロードフレーム最大値 60.0f
	const float							m_standFrmMax	= 15.0f;	// 立ち上がりフレーム最大値 15.0f
	const float							m_sitFrmMax		= 15.0f;	// しゃがみフレーム最大値 15.0f
	const float							m_shotFrmMax	= 25.0f;	// 銃発砲フレーム最大値 25.0f
	const float							m_bulletShotFrm = 2.5f;		// 弾発射フレーム 2.5f
	const float							m_restFrmMax	= 32.5f;	// 拘束フレーム最大値 32.5f
	const float							m_grabFrm		= 7.5f;		// 掴み時フレーム 17.5f
	const float							m_killFrmMax	= 45.0f;	// 処刑時フレーム最大値 45.0f
	const float							m_exeFrm		= 21.0f;	// 処刑時フレーム 22.5f
	const float							m_deathFrmMax	= 65.0f;	// 死亡時フレーム最大値 65.0f

	const float							m_zeroMoveSpd	= 0.0f;		// 停止速度 0.0f
	const float							m_walkMoveSpd	= 0.05f;	// 歩行速度 0.05f
	const float							m_runMoveSpd	= 0.12f;	// 走行速度 0.12f
	const float							m_sitWalkMoveSpd= 0.08f;	// しゃがみ歩行速度 0.08f

	const float							m_tightArea		= 1.25f;	// 拘束エリア 1.25f

	static constexpr short				m_totalKeyFlg	= 7;
	std::bitset<m_totalKeyFlg>			m_bitsKeyFlg	= false;

	static constexpr short				m_totalEachFlg	= 7;
	std::bitset<m_totalEachFlg>			m_bitsEachFlg	= false;

	Math::Vector3						m_diffVec		= Math::Vector3::Zero;			// 敵とプレイヤーの差ベクトル

	UINT								m_sType			= SituationType::Idle;			// プレイヤーの状態タイプ
	PostureType							m_posType		= PostureType::Stand;			// プレイヤーの体勢タイプ
	ItemCollectType						m_itemCollType	= ItemCollectType::NoCollect;	// アイテム所持タイプ

	const float							m_openMapVol	= 0.5f;
	std::shared_ptr<KdSoundInstance>	m_spOpenMapSound;

	const float							m_closeMapVol	= 0.5f;
	std::shared_ptr<KdSoundInstance>	m_spCloseMapSound;

	const float							m_itemCollVol	= 0.5f;
	std::shared_ptr<KdSoundInstance>	m_spItemCollectSound;

	const float							m_shotVol		= 0.5f;
	std::shared_ptr<KdSoundInstance3D>	m_spShotSound;

	const float							m_readyVol		= 0.3f;
	std::shared_ptr<KdSoundInstance3D>	m_spReadySound;

	const float							m_reloadVol		= 0.9f;
	std::shared_ptr<KdSoundInstance3D>	m_spReloadSound;

	const float							m_restraintVol	= 0.5f;
	std::shared_ptr<KdSoundInstance3D>	m_spRestraintSound;

	const float							m_runVol		= 0.2f;
	std::shared_ptr<KdSoundInstance3D>	m_spRunSound;

	const float							m_walkVol		= 0.1f;
	std::shared_ptr<KdSoundInstance3D>	m_spWalkSound;

	const float							m_exeVol		= 0.2f;
	std::shared_ptr<KdSoundInstance3D>	m_spExeSound;

	const float							m_deathVol		= 0.2f;
	std::shared_ptr<KdSoundInstance3D>	m_spDeathSound;

private:

	// ステートパターン管理系
	class ActionStateBase
	{
	public:

		virtual ~ActionStateBase()			{}

		virtual void Enter	(Player& _owner) { (void)_owner; }
		virtual void Update	(Player& _owner) { (void)_owner; }
		virtual void Exit	(Player& _owner) { (void)_owner; }
	};

	class ActionIdle : public ActionStateBase
	{
	public:

		ActionIdle	()				{}
		~ActionIdle	()	override	{}

		void Enter	(Player& _owner)	override;
		void Update	(Player& _owner)	override;
		void Exit	(Player& _owner)	override;
	};

	class ActionWalk : public ActionStateBase
	{
	public:

		ActionWalk	()				{}
		~ActionWalk	()	override	{}

		void Enter	(Player& _owner)	override;
		void Update	(Player& _owner)	override;
		void Exit	(Player& _owner)	override;
	};

	class ActionRun : public ActionStateBase
	{
	public:

		ActionRun	()				{}
		~ActionRun	()	override	{}

		void Enter	(Player& _owner)	override;
		void Update	(Player& _owner)	override;
		void Exit	(Player& _owner)	override;
	};

	class ActionReady : public ActionStateBase
	{
	public:

		ActionReady	()				{}
		~ActionReady()	override	{}

		void Enter	(Player& _owner)	override;
		void Update	(Player& _owner)	override;
		void Exit	(Player& _owner)	override;
	};

	class ActionShot : public ActionStateBase
	{
	public:

		ActionShot	()				{}
		~ActionShot	()	override	{}

		void Enter	(Player& _owner)	override;
		void Update	(Player& _owner)	override;
		void Exit	(Player& _owner)	override;
	};

	class ActionRestraint : public ActionStateBase
	{
	public:

		ActionRestraint	()				{}
		~ActionRestraint()	override	{}

		void Enter	(Player& _owner)	override;
		void Update	(Player& _owner)	override;
		void Exit	(Player& _owner)	override;
	};

	class ActionRestraint_Idle : public ActionStateBase
	{
	public:

		ActionRestraint_Idle	()				{}
		~ActionRestraint_Idle	()	override	{}

		void Enter	(Player& _owner)	override;
		void Update	(Player& _owner)	override;
		void Exit	(Player& _owner)	override;
	};

	class ActionKill : public ActionStateBase
	{
	public:

		ActionKill	()				{}
		~ActionKill	()	override	{}

		void Enter	(Player& _owner)	override;
		void Update	(Player& _owner)	override;
		void Exit	(Player& _owner)	override;
	};

	class ActionReload_Idle : public ActionStateBase
	{
	public:

		ActionReload_Idle	()				{}
		~ActionReload_Idle	()	override	{}

		void Enter	(Player& _owner)	override;
		void Update	(Player& _owner)	override;
		void Exit	(Player& _owner)	override;
	};

	class ActionReload_Walk : public ActionStateBase
	{
	public:

		ActionReload_Walk	()				{}
		~ActionReload_Walk	()	override	{}

		void Enter	(Player& _owner)	override;
		void Update	(Player& _owner)	override;
		void Exit	(Player& _owner)	override;
	};

	class ActionReload_Run : public ActionStateBase
	{
	public:

		ActionReload_Run	()				{}
		~ActionReload_Run()	override	{}

		void Enter	(Player& _owner)	override;
		void Update	(Player& _owner)	override;
		void Exit	(Player& _owner)	override;
	};

	class ActionStand : public ActionStateBase
	{
	public:

		ActionStand	()				{}
		~ActionStand()	override	{}

		void Enter	(Player& _owner)	override;
		void Update	(Player& _owner)	override;
		void Exit	(Player& _owner)	override;
	};

	class ActionSit : public ActionStateBase
	{
	public:

		ActionSit	()				{}
		~ActionSit	()	override	{}

		void Enter	(Player& _owner)	override;
		void Update	(Player& _owner)	override;
		void Exit	(Player& _owner)	override;
	};

	class ActionSit_Idle : public ActionStateBase
	{
	public:

		ActionSit_Idle	()				{}
		~ActionSit_Idle	()	override	{}

		void Enter	(Player& _owner)	override;
		void Update	(Player& _owner)	override;
		void Exit	(Player& _owner)	override;
	};

	class ActionSit_Walk : public ActionStateBase
	{
	public:

		ActionSit_Walk	()				{}
		~ActionSit_Walk	()	override	{}

		void Enter	(Player& _owner)	override;
		void Update	(Player& _owner)	override;
		void Exit	(Player& _owner)	override;
	};

	class ActionSit_Ready : public ActionStateBase
	{
	public:

		ActionSit_Ready	()				{}
		~ActionSit_Ready()	override	{}

		void Enter	(Player& _owner)	override;
		void Update	(Player& _owner)	override;
		void Exit	(Player& _owner)	override;
	};

	class ActionDeath : public ActionStateBase
	{
	public:

		ActionDeath	()				{}
		~ActionDeath()	override	{}

		void Enter	(Player& _owner)	override;
		void Update	(Player& _owner)	override;
	};

	void ChangeActionState(std::shared_ptr<ActionStateBase> _nextState);
	std::shared_ptr<ActionStateBase> m_nowState = nullptr;
};
