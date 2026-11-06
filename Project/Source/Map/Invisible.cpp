#include "DxLib.h"
#include "Invisible.h"
#include "../Camera/Camera.h"

InvisibleData g_InvisibleBlocks[BLOCK_MAX] = { 0 };
int g_InvisibleBlockHandle[BLOCK_TYPE_MAX] = { 0 };

void InitInvisible()
{
	InvisibleData* invisible = g_InvisibleBlocks;
	for (int i = 0; i < BLOCK_MAX; i++, invisible++)
	{
		invisible->active = false;
		invisible->pos = VGet(0.0f, 0.0f, 0.0f);
		invisible->type = MAP_CHIP_NONE;
	}
}

void LoadInvisible()
{
	g_InvisibleBlockHandle[INVISIBLE_BLOCK] = LoadGraph("Data/Map/InvisibleFloor.png");
}

void StartInvisible()
{
}

void StepInvisible()
{
}

void DrawInvisible()
{
	// スクロールのためカメラを取得
	CameraData camera = GetCamera();

	InvisibleData* invisible = g_InvisibleBlocks;
	for (int i = 0; i < BLOCK_MAX; i++, invisible++)
	{
		if (invisible->active)
		{
			DrawGraph((int)(invisible->pos.x - camera.pos.x), (int)(invisible->pos.y - camera.pos.y), invisible->handle, TRUE);
		}
	}
}

void FinInvisible()
{
	for (int i = 0; i < BLOCK_TYPE_MAX; i++)
	{
		DeleteGraph(g_InvisibleBlockHandle[i]);
	}
}

InvisibleData* CreateInvisible(MapChipType type, VECTOR pos)
{
	// 未使用のブロックを探す
	InvisibleData* invisible = g_InvisibleBlocks;
	for (int i = 0; i < BLOCK_MAX; i++, invisible++)
	{
		if (!invisible->active)
		{
			// ブロック生成
			invisible->active = true;
			invisible->handle = g_InvisibleBlockHandle[type];
			invisible->pos = pos;
			invisible->type = type;
			// サイズは固定
			invisible->width = INVISIBLE_WIDTH;
			invisible->height = INVISIBLE_HEIGHT;
			return invisible;
		}
	}

	return nullptr;
}

InvisibleData* GetInvisible()
{
	return g_InvisibleBlocks;
}