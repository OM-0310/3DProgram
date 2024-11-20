#pragma once
#include "../CharaBase.h"

class Player;
class AstarNode;
class AstarGrid;

class Enemy : public CharaBase
{
public:

	Enemy				()				{}
	~Enemy				()	override	{}

	void Init			()	override;
	void Update			()	override;
	void PostUpdate		()	override;

	void ChangeAnimation();	// アニメーション切り替え処理
	void SearchProc		();	// 索敵処理
	void ChaseProc		();	// 追跡処理
	void RouteSearchProc(); // 経路探索処理

	void InitializeGrid	();	// グリッドの初期化処理
	void MoveAlongPath	();	// 敵を経路に沿って次の位置に移動させる処理

	void SetPlayer		(std::shared_ptr<Player> _spPlayer) { m_wpPlayer = _spPlayer; }
	void SetHPDec		(const int& _decNum) { m_HP -= _decNum; }

	int GetHP			()	override { return m_HP; }

	enum class StateType
	{
		Normal,				// 通常
		Wary,				// 警戒
		Discover			// 発見
	};

	enum class EnemyType
	{
		Idle,
	};

private:

	std::shared_ptr<KdAnimator> m_spAnimator;
	std::weak_ptr<Player>		m_wpPlayer;

	Math::Vector3				m_moveDir		= Math::Vector3::Zero;
	float						m_moveSpeed		= 0.0f;

	bool						m_waryFlg		= false;
	bool						m_discoverFlg	= false;
	bool						m_chaseFlg		= false;

	int							m_gridWidth		= 0;
	int							m_gridHeight	= 0;

	static const int			MAXHP = 1;

	StateType					m_sType			= StateType::Normal;
	EnemyType					m_eType			= EnemyType::Idle;

	std::shared_ptr<AstarGrid>				m_spGrid; // グリッドのポインタをクラスメンバとして保持
	std::vector<std::shared_ptr<AstarNode>> m_spPath; // 現在のパスを保持する
};
