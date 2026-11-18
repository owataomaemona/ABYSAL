#pragma once
#include "../Map/MapParameter.h"

void InitSignBoard();
void LoadSignBoard();
void StartSignBoard();
void StepSignBoard();
void DrawSignBoard();
void FinSignBoard();

// アニメーション用パラメーター
struct MapChipAnimationParam
{
	int interval;
	int frameNum;
	int width;
	int height;
};
const MapChipAnimationParam MAP_CHIP_ANIM_PARAM[SIGN_BOARD_BLOCK] =
{
	25,9,98,92 // SIGN
};

MapChipAnimation* CreateSignBoard(MapChipType type, VECTOR pos);
MapChipAnimation* GetSignBoard();