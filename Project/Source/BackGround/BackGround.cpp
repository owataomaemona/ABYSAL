#include "DxLib.h"
#include "BackGround.h"
#include "../Camera/Camera.h"

#define BACK_GROUND_WIDTH	(1600)
#define BACK_GROUND_HEIGHT	(900)

// 背景用のデータ
struct BackGroundData
{
	int handle;
	float posX;
	float posY;
};
BackGroundData g_BackGroundData[BACK_GROUND_LAYER_MAX] = { 0 };

void InitBackGround()
{
	for (int i = 0; i < BACK_GROUND_LAYER_MAX; i++)
	{
		g_BackGroundData[i].handle = 0;
		g_BackGroundData[i].posX = 0.0f;
		g_BackGroundData[i].posY = 0.0f;
	}
}

void LoadBackGround(const char* path, BackGroundLayer layer)
{
	g_BackGroundData[layer].handle = LoadGraph(path);
}

void StepBackGround()
{
}

void UpdateBackGround()
{
	
}

void DrawBackGround()
{
	// スクロールの溜めのカメラを取得
	CameraData camera = GetCamera();

	for (int i = 0; i < BACK_GROUND_LAYER_MAX; i++)
	{
		DrawGraph((int)g_BackGroundData[i].posX - camera.pos.x, (int)g_BackGroundData[i].posY - camera.pos.y, (int)g_BackGroundData[i].handle, TRUE);
	}
}

void FinBackGround()
{
	for (int i = 0; i < BACK_GROUND_LAYER_MAX; i++)
	{
		DeleteGraph(g_BackGroundData[i].handle);
	}
}
