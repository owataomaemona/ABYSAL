#include "DxLib.h"
#include "Effect.h"
#include "../Camera/Camera.h"

#define EFFECT_ANIMATION_MAX 255

EffectData g_EffectData[EFFECT_ANIMATION_MAX] = { 0 };

int g_EffectHandle[EFFECT_MAX] = { 0 };

void InitEffectAnimation() 
{
	EffectData* effect = g_EffectData;
	for (int i = 0; i < EFFECT_ANIMATION_MAX; i++, effect++) 
	{
		effect->active = false;
		effect->handle = 0;
		effect->frameNum = 0;
		effect->frameWidth = 0;
		effect->frameHeight = 0;
		effect->timer = 0;
		effect->pos.x = 0.0f;
		effect->pos.y = 0.0f;
		effect->isLoop = false;
	}
}

void LoadEffectAnimation()
{
	g_EffectHandle[EFFECT_BOMB] = LoadGraph("Data/Effect/Bomb.png");
}

void StepEffectAnimation()
{
	EffectData* effect = g_EffectData;
	for (int i = 0; i < EFFECT_ANIMATION_MAX; i++,effect++)
	{
		if (!effect->active) continue;

		effect->timer--;

		// 次のコマへ進めるか
		if (effect->timer <= 0)
		{
			// 次のコマへ
			effect->nowFrame++;
			// タイマーリセット
			effect->timer = effect->interval;

			// 全コマ表示し終わったら非アクティブ
			if (effect->nowFrame >= effect->frameNum)
			{
				if (effect->isLoop == true)// ループさせたいエフェクトだったらループする
				{
					effect->nowFrame = 0;
				}
				else
				{
					effect->active = false;
					// これ以上処理する必要なし
					continue;
				}
			}
		}
	}
}

void UpdateEffectAnimation()
{
}

void DrawEffectAnimation()
{
	// スクロールのためカメラを取得
	CameraData camera = GetCamera();

	EffectData* effect = g_EffectData;
	for (int i = 0; i < EFFECT_ANIMATION_MAX; i++, effect++)
	{
		if (!effect->active) continue;

		// コマの切り取り位置計算（横１行の画像の想定）
		int frameX = effect->frameWidth * effect->nowFrame;

		// カメラ座標を引いて画面上の描画位置を計算
		int drawX = (int)(effect->pos.x - camera.pos.x);
		int drawY = (int)(effect->pos.y - camera.pos.y);

		// 画像から表示するコマの部分だけ描画
		DrawRectGraph(drawX, drawY, frameX, 0, effect->frameWidth, effect->frameHeight, effect->handle, TRUE);
	}
}


void FinEffectAnimation() 
{
	for (int i = 0; i < EFFECT_MAX; i++) 
	{
		DeleteGraph(g_EffectHandle[i]);
	}
}

EffectData* StartEffectAnimation(EffectType type, float posX, float posY, bool loop)
{
	// マスターパラム取得
	EffectAnimationParam param = EFFECT_ANIM_PARAM[type];

	EffectData* effect = g_EffectData;
	for (int i = 0; i < EFFECT_ANIMATION_MAX; i++, effect++)
	{
		// 未使用のエフェクトを探す
		if (!effect->active)
		{
			// アクティブ
			effect->active = true;

			// 座標設定
			effect->pos.x = posX;
			effect->pos.y = posY;

			// インターバル設定
			effect->interval = param.interval;

			// タイマー設定
			effect->timer = effect->interval;

			// 画像ハンドル
			effect->handle = g_EffectHandle[type];

			// 0コマ目にする
			effect->nowFrame = 0;

			// コマ数
			effect->frameNum = param.frameNum;

			// 1コマのサイズ
			effect->frameWidth = param.width;
			effect->frameHeight = param.height;

			// ループ
			effect->isLoop = loop;

			return effect;
		}

	}

	return nullptr;
}

EffectData* GetEffectAnimation()
{
	return g_EffectData;
}