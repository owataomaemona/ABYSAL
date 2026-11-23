#pragma once
#include "../Map/MapParameter.h"

// 看板サイズ
#define SIGN_BOARD_WIDTH  (100.0f)
#define SIGN_BOARD_HEIGHT (100.0f)

// 離脱判定用のバカでか半径
#define SIGN_BOARD_RADIUS (100.0f)

// エフェクト用パラメーター
struct MapAnimationParam
{
	int interval;
	int frameNum;
	int width;
	int height;
	bool isLoop;
};
const MapAnimationParam SIGN_BOARD_ANIM_PARAM =
{
	10,9,100,100,true,
};

void InitSignBoard();
void LoadSignBoard();
void StepSignBoard();
void DrawSignBoard();
void FinSignBoard();

SignBoardData* CreateSignBoard(MapChipType type, VECTOR pos);
SignBoardData* GetSignBoard();

// クソでか当たり判定
void SignBoardAreaPlayer(SignBoardData* signBoard);