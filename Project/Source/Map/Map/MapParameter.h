#pragma once
#include "DxLib.h"
#include "../../Animation/Animation.h"

#define BLOCK_MAX 1024
#define MAP_CHIP_X_NUM (154)
#define MAP_CHIP_Y_NUM (44)
#define MAP_CHIP_WIDTH (50.0f)
#define MAP_CHIP_HEIGHT (50.0f)

enum MapChipType
{
	MAP_CHIP_NONE,
	NORMAL_BLOCK,       // ここから
	NORMAL_BLOCK_2,     // 
	NORMAL_BLOCK_3,     //
	NORMAL_BLOCK_4,     //
	NORMAL_BLOCK_5,     //
	NORMAL_BLOCK_6,     // ここまでブロック
	SEA_MINE_BLOCK,     // 機雷
	INVISIBLE_BLOCK,    // 透明ブロック(天井や床用)
	SIGN_BOARD_BLOCK,   // 看板ブロック(アニメーション)
	SIGN_BOARD_BLOCK_2, // 看板2
	ENEMY_1,            // エネミー(ウニ)
	BLOCK_TYPE_MAX,
};
/*------------------------------------------------------------*/

struct BlockData // 床ブロック
{
	bool active;
	int handle;
	MapChipType type;
	VECTOR pos;
	float width;
	float height;
};
struct SeaMineData // 機雷
{
	bool active;
	bool isSeaMineGraze; // ギリギリ回避に当たってる
	bool isNowGraze;      // ギリギリ判定内にいるか
	int handle;
	MapChipType type;
	VECTOR pos;
	float radius;
	float width;
	float height;
};
struct InvisibleData // 透明床(天井)
{
	bool active;
	int handle;
	MapChipType type;
	VECTOR pos;
	float width;
	float height;
};
struct SignBoardData // 看板
{
	bool active;
	bool isTouch; // 範囲に触ったか
	int handle;
	MapChipType type;
	VECTOR pos;
	float width;
	float height;
	AnimationData animation;// アニメーション
};

/*------------------------------------------------------------*/


/*------------------------------------------------------------*/
// エネミー用

struct SeaUrchinData // ウニ
{
	bool active;
	VECTOR pos;
	int handle;
	float width;
	float height;
	float radius;
	MapChipType type;
};

/*------------------------------------------------------------*/

struct MapChipData
{
	int mapChip;
	BlockData* block;
	SeaMineData* seaMine;
	InvisibleData* invisible;
	SignBoardData* signBoard;
	SeaUrchinData* seaUrchin;
};