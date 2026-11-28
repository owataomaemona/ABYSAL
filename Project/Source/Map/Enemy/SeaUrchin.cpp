#include "DxLib.h"
#include "../Enemy/SeaUrchin.h"
#include "../../Camera/Camera.h"
#include "../../Animation/Effect.h"

SeaUrchinData g_SeaUrchins[BLOCK_MAX] = { 0 };
int g_SeaUrchinhandle[BLOCK_TYPE_MAX] = { 0 };

void InitSeaUrchin()
{
	SeaUrchinData* seaUrchin = g_SeaUrchins;
	for (int i = 0; i < BLOCK_MAX; i++, seaUrchin++)
	{
		seaUrchin->active = false;
		seaUrchin->pos = VGet(0.0f, 0.0f, 0.0f);
		seaUrchin->type = MAP_CHIP_NONE;

		InitAnimation(&seaUrchin->animation);
	}
}

void LoadSeaUrchin()
{
	g_SeaUrchinhandle[ENEMY_1] = LoadGraph("Data/Enemy/Enemy1.png");
}

void StartSeaUrchin()
{
}

void StepSeaUrchin()
{
	SeaUrchinData* seaUrchin = g_SeaUrchins;
	for (int i = 0; i < BLOCK_MAX; i++, seaUrchin++)
	{
		if (seaUrchin->active)
		{
			UpdateAnimation(&seaUrchin->animation);
		}
	}
}

void DrawSeaUrchin()
{
	// スクロールのためカメラを取得
	CameraData camera = GetCamera();

	SeaUrchinData* seaUrchin = g_SeaUrchins;
	for (int i = 0; i < BLOCK_MAX; i++, seaUrchin++)
	{
		if (seaUrchin->active)
		{
			// アニメーション描画
			DrawAnimation(&seaUrchin->animation, seaUrchin->pos.x - camera.pos.x, seaUrchin->pos.y - camera.pos.y, false);
		}
	}
}

void FinSeaUrchin()
{
	for(int i = 0; i< BLOCK_TYPE_MAX; i++)
	{
		DeleteGraph(g_SeaUrchinhandle[i]);
	}
}

SeaUrchinData* CreateSeaUrchin(MapChipType type, VECTOR pos)
{
	// 未使用のブロックを探す
	SeaUrchinData* seaUrchin = g_SeaUrchins;
	for (int i = 0; i < BLOCK_MAX; i++, seaUrchin++)
	{
		if (!seaUrchin->active)
		{
			// ウニ生成
			seaUrchin->active = true;
			seaUrchin->handle = g_SeaUrchinhandle[type];
			seaUrchin->isEnemyGraze = false;
			seaUrchin->isNowGraze = false;
			seaUrchin->pos = pos;
			seaUrchin->type = type;
			seaUrchin->width = SEA_URCHIN_WIDTH;
			seaUrchin->height = SEA_URCHIN_HEIGHT;
			seaUrchin->radius = SEA_URCHIN_RADIUS;

			// アニメーション用のハンドル
			seaUrchin->animation.handle = g_SeaUrchinhandle[type];

			// アニメーションも行うため、CreateSeaUrchin内にスタートアニメーションを書く
			const SeaUrchinAnimationParam& param = SEA_URCHIN_ANIM_PARAM;
			StartAnimation(&seaUrchin->animation, pos.x, pos.y, param.interval, param.frameNum, param.width, param.height, param.isLoop);

			return seaUrchin;
		}
	}

	return nullptr;
}

SeaUrchinData* GetSeaUrchin()
{
	return g_SeaUrchins;
}

void SeaUrchinHitPlayer(SeaUrchinData* seaUrchin) 
{
	seaUrchin->isEnemyGraze = true;
	seaUrchin->isNowGraze = false;
}