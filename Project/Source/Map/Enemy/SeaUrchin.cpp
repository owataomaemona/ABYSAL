#include "DxLib.h"
#include "../Enemy/SeaUrchin.h"
#include "../../Camera/Camera.h"

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
			DrawGraph((int)(seaUrchin->pos.x - camera.pos.x), (int)(seaUrchin->pos.y - camera.pos.y), seaUrchin->handle, TRUE);
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
			seaUrchin->pos = pos;
			seaUrchin->type = type;
			seaUrchin->width = SEA_URCHIN_WIDTH;
			seaUrchin->height = SEA_URCHIN_HEIGHT;
			seaUrchin->radius = SEA_URCHIN_RADIUS;
			return seaUrchin;
		}
	}

	return nullptr;
}

SeaUrchinData* GetSeaUrchin()
{
	return g_SeaUrchins;
}