#include "DxLib.h"
#include "../Camera/Camera.h"
#include "../Map/SeaMine.h"

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
	if (mine != nullptr) 
	{
		mine->active = false;
	}
}

void SeaMineHitPlayer(SeaMineData* mine) 
{
	SeaMineDead(mine);
}