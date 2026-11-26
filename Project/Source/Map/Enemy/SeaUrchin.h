#pragma once
#include "../Map/MapParameter.h"

struct SeaUrchinData;

#define SEA_URCHIN_WIDTH  (92.0f)
#define SEA_URCHIN_HEIGHT (66.0f)
#define SEA_URCHIN_RADIUS (56.6f)

void InitSeaUrchin();
void LoadSeaUrchin();
void StartSeaUrchin();
void StepSeaUrchin();
void DrawSeaUrchin();
void FinSeaUrchin();

SeaUrchinData* CreateSeaUrchin(MapChipType type, VECTOR pos);
SeaUrchinData* GetSeaUrchin();