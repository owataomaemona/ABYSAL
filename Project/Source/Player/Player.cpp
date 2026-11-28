#include "DxLib.h"
#include "Player.h"
#include "../Input/Input.h"
#include "../Collision/Collision.h"
#include "../Camera/Camera.h"
#include "../Animation/Effect.h"
#include "../Map/Map/MapChip.h"
#include "../Map/Map/MapManager.h"

// アニメーション用パラメーター
struct PlayerAnimationParam
{
	int interval;
	int frameNum;
	int width;   //ここの縦横幅は当たり判定ではなく、"画像"としての縦横幅になる
	int height;
	bool isLoop; // ここに追加することによって、パラメーターの方だけでループするか否かを設定できる!
};
const PlayerAnimationParam PLAYER_ANIM_PARAM[PLAYER_ANIM_MAX] =
{
	25,2,76,92,true,   // IDLE
	25,4,80,92,true,   // WALK
	25,4,88,88,true,   // SWIM
	3,5,92,92,false,   // DASH
	10,9,84,240,false, // DEAD
};

// プレイヤー設定関連
#define PLAYER_DEFAULT_POS_X (100.0f)
#define PLAYER_DEFAULT_POS_Y (2000.0f)
#define PLAYER_MOVE_SPEED (2.0f)
#define PLAYER_LIFE (4)

// プレイヤーダッシュ関連
#define PLAYER_DASH_SPEED (12.0f)
#define PLAYER_DASH_SPEED_Y (5.5f)
#define PLAYER_DASH_TIME (14)

// ジャンプ力
#define PLAYER_JUMP_POWER (4.0f)

// 重力
#define PLAYER_GRAVITY (0.1f)

// 敵に当たった時の無敵時間
#define PLAYER_HIT_ENEMY_INVISIBLE_TIME (80)

// ノックバックの移動量
#define PLAYER_BACK_X (5.0f)
#define PLAYER_BACK_Y (-4.0f)

PlayerData g_PlayerData = { 0 };
PlayerData g_PrevPlayerData = { 0 };

// このCPPでのみ使用する関数の宣言
void StartPlayerAnimation(PlayerAnimationType anim);	 // アニメーション再生
void UpdatePlayerAnimation();							 // アニメーション更新

void InitPlayer()
{
	g_PlayerData.pos = VGet(0.0f, 0.0f, 0.0f);
	g_PlayerData.move = VGet(0.0f, 0.0f, 0.0f);
	g_PlayerData.beforePos = VGet(0.0f, 0.0f, 0.0f);

	g_PlayerData.width = 0.0f;
	g_PlayerData.height = 0.0f;
	
	g_PlayerData.isDash = false;
	g_PlayerData.dashMove = false;
	g_PlayerData.dashTimer = 0.0f;

	g_PlayerData.isDashUp = false;
	g_PlayerData.isDashDown = false;

	g_PlayerData.active = false;
	g_PlayerData.life = 0;
	g_PlayerData.isDead = false;
	g_PlayerData.invisible = false;
	g_PlayerData.hitInvincible = 0;

	g_PlayerData.isAir = false;

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
	g_PlayerData.animation[PLAYER_ANIM_DEAD].handle = LoadGraph("Data/Player/DiverDead.png");
}

void StartPlayer() 
{
	// 生存フラグを立てる
	g_PlayerData.active = true;

	// 体力を初期化
	g_PlayerData.life = PLAYER_LIFE;

	// 死亡フラグも立てる(死亡アニメーションを組み込むため)
	g_PlayerData.isDead = false;

	// 初期位置設定
	g_PlayerData.pos.x = PLAYER_DEFAULT_POS_X;
	g_PlayerData.pos.y = PLAYER_DEFAULT_POS_Y;

	//　サイズ設定
	g_PlayerData.width = PLAYER_WIDTH;
	g_PlayerData.height = PLAYER_HEIGHT;
	
	// 待機アニメーションから開始する
	StartPlayerAnimation(PLAYER_ANIM_IDLE);
}

void StepPlayer() 
{
	// 死んでいたら何もしない
	if (!g_PlayerData.active || g_PlayerData.isDead)
	{
		return;
	}

	// 前回のデータとして記憶	
	g_PrevPlayerData = g_PlayerData;

	// X移動量は毎回リセットする
	g_PlayerData.move.x = 0.0f;

	// Y移動量に重力を反映(重力は下向きのため、プラスで足していく)
	 g_PlayerData.move.y += PLAYER_GRAVITY;

	// 左入力
	if (IsInputKey(KEY_LEFT))
	{
		g_PlayerData.move.x = -PLAYER_MOVE_SPEED;
		g_PlayerData.isTurn = true;
	}
	// 右入力
	else if (IsInputKey(KEY_RIGHT))
	{
		g_PlayerData.move.x = PLAYER_MOVE_SPEED;
		g_PlayerData.isTurn = false;
	}

	// Z入力
	if (IsTriggerKey(KEY_Z))
	{
		// Y移動量にジャンプ力を与える(上に力を与えるため、マイナスを付ける)
		g_PlayerData.move.y = -PLAYER_JUMP_POWER;
	}

	// V入力(ダッシュしてない時)
	if (IsTriggerKey(KEY_V) && !g_PlayerData.isDash) 
	{
		g_PlayerData.isDash = true;
		g_PlayerData.dashTimer = PLAYER_DASH_TIME;

		StartPlayerAnimation(PLAYER_ANIM_DASH);

		PlayerFlash();

		g_PlayerData.dashMove = false;
		g_PlayerData.isDashUp = false;
		g_PlayerData.isDashDown = false;

		if (IsInputKey(KEY_LEFT) || IsInputKey(KEY_RIGHT))
		{
			g_PlayerData.dashMove = true;
		}
		if (IsInputKey(KEY_UP)) // elseにしない
		{
			g_PlayerData.isDashUp = true;
		}
		if(IsInputKey(KEY_DOWN))
		{
			g_PlayerData.isDashDown = true;
		}
	}

	// 無敵中の処理
	if (g_PlayerData.invisible)
	{
		g_PlayerData.hitInvincible--;
	}
}

void UpdatePlayer()
{
	// 死んでいたら処理しない
	if (!g_PlayerData.active) return;

	// これがないと死亡した後でも動き続ける
	if (g_PlayerData.isDead) 
	{
		UpdatePlayerAnimation();
		return;
	}

	// 上昇しているもしくは落ちている場合は空中フラグを立てる
	if (g_PlayerData.move.y < 0.0f || g_PlayerData.move.y > PLAYER_GRAVITY)
	{
		g_PlayerData.isAir = true;
	}

	// ダッシュ中か確認
	if (g_PlayerData.isDash)
	{
		g_PlayerData.dashTimer--; // タイマーを減らす

		// 上下のダッシュ中は重力を0にする
		g_PlayerData.move.y = 0.0f;

		if (g_PlayerData.isDashUp)
		{
			g_PlayerData.move.y -= PLAYER_DASH_SPEED_Y;
		}
		if (g_PlayerData.isDashDown) // elseにしない
		{
			g_PlayerData.move.y += PLAYER_DASH_SPEED_Y; // 重力がかかってる状態だと効かない(急降下に便利)
		}

		if (g_PlayerData.dashMove) // ダッシュで移動するか
		{
			if (g_PlayerData.isTurn) // 向きの判定
			{
				g_PlayerData.pos.x -= PLAYER_DASH_SPEED;
			}
			else
			{
				g_PlayerData.pos.x += PLAYER_DASH_SPEED;
			}
		}

		if (g_PlayerData.dashTimer <= 0) // タイマー終了(ダッシュの終了)
		{
			g_PlayerData.isDash = false;
			g_PlayerData.dashMove = false;
			g_PlayerData.isDashUp = false;
			g_PlayerData.isDashDown = false;
		}
	}

	g_PlayerData.beforePos = g_PlayerData.pos;

	// 移動処理
	g_PlayerData.pos.x += g_PlayerData.move.x;
	g_PlayerData.pos.y += g_PlayerData.move.y;

	UpdatePlayerAnimation();

	// カメラ取得
	CameraData camera = GetCamera();

	// プレイヤーが画面左側にはみ出さないようにする
	if (g_PlayerData.pos.x < camera.pos.x) // プレイヤーの横座標がカメラの横座標より小さくなったら
	{
		g_PlayerData.pos.x = camera.pos.x; // プレイヤーの横座標をカメラの横座標と同じにする
	}

	// 無敵解除チェック
	if (g_PlayerData.invisible && g_PlayerData.hitInvincible <= 0) 
	{
		g_PlayerData.invisible = false;
	}

}

void DrawPlayer()
{
	// 死んでいたら処理しない
	if (!g_PlayerData.active) return;

	// カメラ取得
	CameraData camera = GetCamera();

	// 無敵時間中の点滅処理
	if (g_PlayerData.invisible)
	{
		// 4フレームごとに描画するしないを切り替える
		if ((g_PlayerData.hitInvincible / 4) % 2 == 0)
		{
			return;
		}
	}

	// アニメーション描画でプレイヤーを描画する
	PlayerAnimationType animType = g_PlayerData.playAnim;
	AnimationData* animData = &g_PlayerData.animation[animType];
	DrawAnimation(animData, g_PlayerData.pos.x - camera.pos.x, g_PlayerData.pos.y - camera.pos.y, g_PlayerData.isTurn);

}

void FinPlayer()
{
	for (int i = 0; i < PLAYER_ANIM_MAX; i++)
	{
		DeleteGraph(g_PlayerData.animation[i].handle);
	}
}

PlayerData* GetPlayer()
{
	return &g_PlayerData;
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
	StartAnimation(animData, g_PlayerData.pos.x, g_PlayerData.pos.y, animParam.interval, animParam.frameNum, animParam.width, animParam.height, animParam.isLoop);
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
	// 死亡しているか
	if (!g_PlayerData.active || g_PlayerData.isDead)
	{
		AnimationData* animData = &g_PlayerData.animation[PLAYER_ANIM_DEAD]; // 死亡アニメーション
		UpdateAnimation(animData); // アニメーションを進める処理

		return;
	}
	// 地上にいるか
	if (!g_PlayerData.isAir)
	{
		// 横に移動していれば走っている
		if (g_PlayerData.move.x < 0.0f || g_PlayerData.move.x > 0.0f)
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
		if (g_PlayerData.move.y < 0.0f)
		{
			StartPlayerAnimation(PLAYER_ANIM_SWIM);
		}
		// 下降していれば落下アニメーション
		else if (g_PlayerData.move.y > 0.0f)
		{
			StartPlayerAnimation(PLAYER_ANIM_SWIM);
		}
	}

	// アニメーション更新
	PlayerAnimationType animType = g_PlayerData.playAnim;
	AnimationData* animData = &g_PlayerData.animation[animType];
	UpdateAnimation(animData);
}

void PlayerDead() 
{
	// プレイヤーがアクティブかつ死んでない場合
	if (g_PlayerData.active && !g_PlayerData.isDead)
	{
		g_PlayerData.isDead = true;

		// 無敵状態を解除
		g_PlayerData.invisible = false;
		g_PlayerData.hitInvincible = 0;

		// ↓ダッシュ中に死亡してもアニメーションを固まらせないために追加
		g_PlayerData.isDash = false;
		g_PlayerData.dashMove = false;
		g_PlayerData.dashTimer = 0.0f;
		g_PlayerData.isDashUp = false;
		g_PlayerData.isDashDown = false;

		StartPlayerAnimation(PLAYER_ANIM_DEAD);
	}
}

void PlayerHitSeaMine() 
{
	PlayerDead();
}

void PlayerHitEnemy()
{
	if (g_PlayerData.invisible || g_PlayerData.isDead) return;

	g_PlayerData.life--;

	if (g_PlayerData.life <= 0)
	{
		PlayerDead();
	}
	else
	{
		// 無敵状態
		g_PlayerData.invisible = true;
		g_PlayerData.hitInvincible = PLAYER_HIT_ENEMY_INVISIBLE_TIME;

		// ダッシュ中の場合は強制解除
		g_PlayerData.isDash = false;
		g_PlayerData.dashMove = false;
		g_PlayerData.dashTimer = 0.0f;
		g_PlayerData.isDashUp = false;
		g_PlayerData.isDashDown = false;

		// ノックバック
		g_PlayerData.move.y = PLAYER_BACK_Y;
		if (g_PlayerData.isTurn)
		{
			g_PlayerData.move.x += PLAYER_BACK_X;
		}
		else
		{
			g_PlayerData.move.x -= PLAYER_BACK_X;
		}

	}
}

void GrazePlayerHitEnemy(PlayerData* player)
{
	// プレイヤーの中心座標を計算
	float centerX = player->pos.x + PLAYER_WIDTH / 2;
	float centerY = player->pos.y + PLAYER_HEIGHT / 2;

	// プレイヤーの左斜め上に出るようにする
	float effectX = centerX - EFFECT_ANIM_PARAM[EFFECT_NICE].width;
	float effectY = centerY - EFFECT_ANIM_PARAM[EFFECT_NICE].height;

	StartEffectAnimation(EFFECT_NICE, effectX, effectY);
}

void PlayerFlash()
{
	// プレイヤーの中心座標を計算
	float centerX = g_PlayerData.pos.x + PLAYER_WIDTH / 2;
	float centerY = g_PlayerData.pos.y + PLAYER_HEIGHT / 2;

	// フラッシュエフェクトの半分を引いて中心を合わせる
	float effectX = centerX - EFFECT_ANIM_PARAM[EFFECT_FLASH].width / 2;
	float effectY = centerY - EFFECT_ANIM_PARAM[EFFECT_FLASH].height / 2;

	StartEffectAnimation(EFFECT_FLASH, effectX, effectY);
}