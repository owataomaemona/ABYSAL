#include "DxLib.h"
#include "../Camera/Camera.h"
#include "../Map/SeaMine.h"
#include "../Animation/Effect.h"

SeaMineData g_SeaMines[BLOCK_MAX] = { 0 };
int g_SeaMineHandle[BLOCK_TYPE_MAX] = { 0 };

void InitSeaMine()
{
	SeaMineData* mine = g_SeaMines;
	for (int i = 0; i < BLOCK_MAX; i++, mine++)
	{
		mine->active = false;
		mine->pos = VGet(0.0f, 0.0f, 0.0f);
		mine->type = MAP_CHIP_NONE;
	}
}

void LoadSeaMine() 
{
	g_SeaMineHandle[SEA_MINE_BLOCK] = LoadGraph("Data/Map/SeaMine.png");
}

void StartSeaMine()
{
}

void StepSeaMine()
{
}

void DrawSeaMine()
{
	// スクロールのためカメラを取得
	CameraData camera = GetCamera();

	SeaMineData* mine = g_SeaMines;
	for (int i = 0; i < BLOCK_MAX; i++, mine++)
	{
		if (mine->active)
		{
			DrawGraph((int)(mine->pos.x - camera.pos.x), (int)(mine->pos.y - camera.pos.y), mine->handle, TRUE);
		}
	}
}

void FinSeaMine()
{
	for (int i = 0; i < BLOCK_TYPE_MAX; i++)
	{
		DeleteGraph(g_SeaMineHandle[i]);
	}
}

SeaMineData* CreateSeaMine(MapChipType type, VECTOR pos)
{
	// 未使用のブロックを探す
	SeaMineData* mine = g_SeaMines;
	for (int i = 0; i < BLOCK_MAX; i++, mine++)
	{
		if (!mine->active) 
		{
			// 機雷生成
			mine->active = true;
			mine->handle = g_SeaMineHandle[type];
			mine->pos = pos;
			mine->type = type;
			mine->width = SEA_MINE_WIDTH;
			mine->height = SEA_MINE_HEIGHT;
			mine->radius = SEA_MINE_RADIUS;
			return mine;
		}
	}

	return nullptr;
}

SeaMineData* GetSeaMine()
{
	return g_SeaMines;
}

void SeaMineDead(SeaMineData* mine)
{
	if (mine != nullptr && mine->active)
	{
		mine->active = false;

		// 機雷の中心座標を計算（144 / 2 = 72）
		float centerX = mine->pos.x + (SEA_MINE_WIDTH * 0.5f);
		float centerY = mine->pos.y + (SEA_MINE_HEIGHT * 0.5f);

		// 爆発エフェクト（96x96）の半分（48）を引いて、中心をぴったり合わせる
		float effectX = centerX - (EFFECT_ANIM_PARAM[EFFECT_BOMB].width * 0.5f);
		float effectY = centerY - (EFFECT_ANIM_PARAM[EFFECT_BOMB].height * 0.5f);

		StartEffectAnimation(EFFECT_BOMB, effectX, effectY);
	}
}

void SeaMineHitPlayer(SeaMineData* mine) 
{
	SeaMineDead(mine);
}