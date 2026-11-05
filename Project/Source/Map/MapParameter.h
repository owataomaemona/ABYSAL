#pragma once
#include "DxLib.h"

#define BLOCK_MAX 1024
#define MAP_CHIP_X_NUM (154)
#define MAP_CHIP_Y_NUM (43)
#define MAP_CHIP_WIDTH (50.0f)
#define MAP_CHIP_HEIGHT (50.0f)

enum MapChipType
{
	MAP_CHIP_NONE,
	NORMAL_BLOCK,   // ここから
	NORMAL_BLOCK_2, // 
	NORMAL_BLOCK_3, //
	NORMAL_BLOCK_4, //
	NORMAL_BLOCK_5, //
	NORMAL_BLOCK_6, // ここまでブロック
	SEA_MINE_BLOCK, // 機雷
	BLOCK_TYPE_MAX,
};

struct BlockData 
{
	bool active;
	int handle;
	MapChipType type;
	VECTOR pos;
	float width;
	float height;
};

struct SeaMineData
{
	bool active;
	int handle;
	MapChipType type;
	VECTOR pos;
	float width;
	float height;
};

struct MapChipData
{
	int mapChip;
	BlockData* block;
	SeaMineData* seaMine;
};