#pragma once
#include "../Map/MapParameter.h"

struct SeaUrchinData;

#define SEA_URCHIN_WIDTH  (92.0f)
#define SEA_URCHIN_HEIGHT (66.0f)
#define SEA_URCHIN_TOP_SPACE (34.0f) // 元画像の空白17px * 2倍(当たり判定で空白部分にも当たり判定が付かないように)
#define SEA_URCHIN_RADIUS (56.6f)

void InitSeaUrchin();
void LoadSeaUrchin();
void StartSeaUrchin();
void StepSeaUrchin();
void DrawSeaUrchin();
void FinSeaUrchin();

SeaUrchinData* CreateSeaUrchin(MapChipType type, VECTOR pos);
SeaUrchinData* GetSeaUrchin();

// 当たり判定
void SeaUrchinHitPlayer(SeaUrchinData* seaUrchin);