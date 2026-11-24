#pragma once
#include "DxLib.h"
#include "../BossEnemy/FirstBoss/FirstBoss.h"

enum BossType
{
	BOSS_NONE,
	BOSS_FIRST, // 第一ボス
	BOSS_MAX,
};

struct FirstBossData
{
	VECTOR pos;
	int handle;
	float width;
	float height;
	BossType type;
};