#pragma once
#include "MapParameter.h"

#define SEA_MINE_WIDTH  (144.0f)
#define SEA_MINE_HEIGHT (144.0f)
#define SEA_MINE_RADIUS (65)

void InitSeaMine();
void LoadSeaMine();
void StartSeaMine();
void StepSeaMine();
void DrawSeaMine();
void FinSeaMine();

SeaMineData* CreateSeaMine(MapChipType type, VECTOR pos);
SeaMineData* GetSeaMine();

// 当たり判定
void SeaMineHitPlayer(SeaMineData* mine);