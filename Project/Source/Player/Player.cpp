#include "DxLib.h"
#include "Player.h"
#include "../Input/Input.h"
#include "../Camera/Camera.h"

// アニメーション用パラメーター
struct PlayerAnimationParam
{
	int interval;
	int frameNum;
	int width;
	int height;
	bool isLoop; // ここに追加することによって、パラメーターの方だけでループするか否かを設定できる!
};
const PlayerAnimationParam PLAYER_ANIM_PARAM[PLAYER_ANIM_MAX] =
{
	25,2,76,92,true,   // IDLE
	25,4,80,92,true,   // WALK
	25,4,88,88,true,   // SWIM
	3,5,92,92,false,  // DASH
};

// プレイヤー設定関連
#define PLAYER_DEFAULT_POS_X (100.0f)
#define PLAYER_DEFAULT_POS_Y (600.0f)
#define PLAYER_MOVE_SPEED (2.0f)

// プレイヤーダッシュ関連
#define PLAYER_DASH_SPEED (10.0f)
#define PLAYER_DASH_TIME (12)

// ジャンプ力
#define PLAYER_JUMP_POWER (4.5f)
// 重力
#define PLAYER_GRAVITY (0.1f)

PlayerData g_PlayerData = { 0 };
PlayerData g_PrevPlayerData = { 0 };

// このCPPでのみ使用する関数の宣言
void StartPlayerAnimation(PlayerAnimationType anim);	 // アニメーション再生
void UpdatePlayerAnimation();							 // アニメーション更新

void InitPlayer()
{
	g_PlayerData.posX = PLAYER_DEFAULT_POS_X;
	g_PlayerData.posY = PLAYER_DEFAULT_POS_Y;
	g_PlayerData.moveX = 0.0f;
	g_PlayerData.moveY = 0.0f;
	g_PlayerData.isDash = false;
	g_PlayerData.dashMove = false;
	g_PlayerData.dashTimer = 0.0f;
	g_PlayerData.active = false;
	g_PlayerData.playAnim = PLAYER_ANIM_NONE;

	for (int i = 0; i < PLAYER_ANIM_MAX; i++)
	{
		InitAnimation(&g_PlayerData.animation[i]);
	}

}

void LoadPlayer() 
{
	g_PlayerData.animation[PLAYER_ANIM_IDLE].handle = LoadGraph("Data/Player/Diver.png");
	g_PlayerData.animation[PLAYER_ANIM_WALK].handle = LoadGraph("Data/Player/DiverWalk.png");
	g_PlayerData.animation[PLAYER_ANIM_SWIM].handle = LoadGraph("Data/Player/DiverSwim.png");
	g_PlayerData.animation[PLAYER_ANIM_DASH].handle = LoadGraph("Data/Player/DiverDash.png");
}

void StartPlayer() 
{
	// 生存フラグを立てる
	g_PlayerData.active = true;

	// 待機アニメーションから開始する
	StartPlayerAnimation(PLAYER_ANIM_IDLE);
}

void StepPlayer() 
{
	// 死んでいたら何もしない
	if (!g_PlayerData.active)
	{
		return;
	}

	// 前回のデータとして記憶	
	g_PrevPlayerData = g_PlayerData;

	// X移動量は毎回リセットする
	g_PlayerData.moveX = 0.0f;

	// Y移動量に重力を反映(重力は下向きのため、プラスで足していく)
	 g_PlayerData.moveY += PLAYER_GRAVITY;

	// 左入力
	if (IsInputKey(KEY_LEFT))
	{
		g_PlayerData.moveX = -PLAYER_MOVE_SPEED;
		g_PlayerData.isTurn = true;
	}
	// 右入力
	else if (IsInputKey(KEY_RIGHT))
	{
		g_PlayerData.moveX = PLAYER_MOVE_SPEED;
		g_PlayerData.isTurn = false;
	}

	// Z入力
	if (IsTriggerKey(KEY_Z))
	{
		// Y移動量にジャンプ力を与える(上に力を与えるため、マイナスを付ける)
		g_PlayerData.moveY = -PLAYER_JUMP_POWER;
	}

	// V入力(ダッシュしてない時)
	if (IsTriggerKey(KEY_V) && !g_PlayerData.isDash) 
	{
		g_PlayerData.isDash = true;
		g_PlayerData.dashTimer = PLAYER_DASH_TIME;

		StartPlayerAnimation(PLAYER_ANIM_DASH);

		if (IsInputKey(KEY_LEFT) || IsInputKey(KEY_RIGHT)) 
		{
			g_PlayerData.dashMove = true;
		}
		else
		{
			g_PlayerData.dashMove = false;
		}
	}
}

void UpdatePlayer()
{
	// 死んでいたら処理しない
	if (!g_PlayerData.active) return;

	// 上昇しているもしくは落ちている場合は空中フラグを立てる
	if (g_PlayerData.moveY < 0.0f || g_PlayerData.moveY > PLAYER_GRAVITY)
	{
		g_PlayerData.isAir = true;
	}

	// ダッシュ中か確認
	if (g_PlayerData.isDash)
	{
		g_PlayerData.dashTimer--; // タイマーを減らす

		if (g_PlayerData.dashMove) // ダッシュで移動するか
		{
			if (g_PlayerData.isTurn) // 向きの判定
			{
				g_PlayerData.posX -= PLAYER_DASH_SPEED;
			}
			else
			{
				g_PlayerData.posX += PLAYER_DASH_SPEED;
			}
		}

		if (g_PlayerData.dashTimer <= 0) // タイマー終了(ダッシュの終了)
		{
			g_PlayerData.isDash = false;
			g_PlayerData.dashMove = false;
		}
	}

	// 移動処理
	g_PlayerData.posX += g_PlayerData.moveX;
	g_PlayerData.posY += g_PlayerData.moveY;

	UpdatePlayerAnimation();

	// カメラ取得
	CameraData camera = GetCamera();

	// プレイヤーが画面左側にはみ出さないようにする
	if (g_PlayerData.posX < camera.pos.x) // プレイヤーの横座標がカメラの横座標より小さくなったら
	{
		g_PlayerData.posX = camera.pos.x; // プレイヤーの横座標をカメラの横座標と同じにする
	}
}

void DrawPlayer()
{
	// 死んでいたら処理しない
	if (!g_PlayerData.active) return;

	// カメラ取得
	CameraData camera = GetCamera();

	// アニメーション描画でプレイヤーを描画する
	PlayerAnimationType animType = g_PlayerData.playAnim;
	AnimationData* animData = &g_PlayerData.animation[animType];
	DrawAnimation(animData, g_PlayerData.posX - camera.pos.x, g_PlayerData.posY - camera.pos.y, g_PlayerData.isTurn);

	// 当たり判定描画
	//float x, y, w, h;
	//CalcBoxCollision(g_PlayerData, x, y, w, h);
	//DrawBox((int)x, (int)y, (int)(x + w), (int)(y + h), GetColor(255, 255, 255), false);
}

void FinPlayer()
{
	for (int i = 0; i < PLAYER_ANIM_MAX; i++)
	{
		DeleteGraph(g_PlayerData.animation[i].handle);
	}
}

PlayerData GetPlayer()
{
	return g_PlayerData;
}

void StartPlayerAnimation(PlayerAnimationType anim)
{
	// 再生中のアニメーションであれば何もしない
	if (anim == g_PlayerData.playAnim) return;

	// 再生中アニメーション設定
	g_PlayerData.playAnim = anim;

	// 再生に必要なデータを取得
	AnimationData* animData = &g_PlayerData.animation[anim];
	PlayerAnimationParam animParam = PLAYER_ANIM_PARAM[anim];

	// アニメーション再生
	StartAnimation(animData, g_PlayerData.posX, g_PlayerData.posY, animParam.interval, animParam.frameNum, animParam.width, animParam.height, animParam.isLoop);
}

void UpdatePlayerAnimation()
{
	// ダッシュ中か
	if (g_PlayerData.isDash)
	{
		AnimationData* animData = &g_PlayerData.animation[PLAYER_ANIM_DASH]; // ダッシュアニメーションを取得
		UpdateAnimation(animData); // アニメーションを進める処理

		return;
	}
	// 地上にいるか
	if (!g_PlayerData.isAir)
	{
		// 横に移動していれば走っている
		if (g_PlayerData.moveX < 0.0f || g_PlayerData.moveX > 0.0f)
		{
			// 歩くアニメーション
			StartPlayerAnimation(PLAYER_ANIM_WALK);
		}
		else
		{
			// 移動していなければ待機
			StartPlayerAnimation(PLAYER_ANIM_IDLE);
		}
	}
	// 空中にいる
	else
	{
		// 上昇していればジャンプアニメーション
		if (g_PlayerData.moveY < 0.0f)
		{
			StartPlayerAnimation(PLAYER_ANIM_SWIM);
		}
		// 下降していれば落下アニメーション
		else if (g_PlayerData.moveY > 0.0f)
		{
			StartPlayerAnimation(PLAYER_ANIM_SWIM);
		}
	}

	// アニメーション更新
	PlayerAnimationType animType = g_PlayerData.playAnim;
	AnimationData* animData = &g_PlayerData.animation[animType];
	UpdateAnimation(animData);
}