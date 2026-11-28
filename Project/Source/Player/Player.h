#pragma once
#include "../Animation/Animation.h"

// プレイヤーサイズ
#define PLAYER_WIDTH	(76.0f)
#define PLAYER_HEIGHT	(92.0f)
#define PLAYER_RADIUS	(38.0f)

// プレイヤーギリギリ回避判定
#define PLAYER_GRAZE_RADIUS	(46.0f)

// アニメーションタイプ
enum PlayerAnimationType
{
	PLAYER_ANIM_IDLE,
	PLAYER_ANIM_WALK,
	PLAYER_ANIM_SWIM,
	PLAYER_ANIM_DASH,
	PLAYER_ANIM_DEAD,
	PLAYER_ANIM_MAX,
	PLAYER_ANIM_NONE = -1
};

// プレイヤーデータ
struct PlayerData
{
	bool active;								// 生存フラグ
	bool isDead;                                // 死亡したか
	bool isTurn;								// 左を向いているか
	bool isAir;									// 空中にいるか
	bool isDash;                                // ダッシュ中
	bool dashMove;                              // 移動するダッシュか(止まってる状態の時のことも考慮)
	float dashTimer;                            // ダッシュ時間
	bool isDashUp;                              // 上にダッシュするか
	bool isDashDown;                            // 下にダッシュするか
	int life;                                   // 体力
	VECTOR pos;                                 // 座標
	VECTOR beforePos;                           // 前にいた座標
	VECTOR move;                                // 移動量
	float width;                                // 横幅格納用
	float height;                               // 縦幅格納用
	AnimationData animation[PLAYER_ANIM_MAX];	// アニメーション
	PlayerAnimationType playAnim;				// 再生中のアニメーション
};

// 関数のプロトタイプ宣言
void InitPlayer();
void LoadPlayer();
void StartPlayer();
void StepPlayer();
void UpdatePlayer();
void DrawPlayer();
void FinPlayer();

// 取得関数
PlayerData* GetPlayer();

// 機雷と当たった時
void PlayerHitSeaMine();
// エネミーと当たった時
void PlayerHitEnemy();
// 機雷ギリギリ回避
void GrazePlayerHitEnemy(PlayerData* player);
// フラッシュアニメーション用
void PlayerFrash();