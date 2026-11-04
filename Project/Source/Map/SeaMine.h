#pragma once
#include "MapParameter.h"

void InitSeaMine();
void LoadSeaMine();
void StartSeaMine();
void StepSeaMine();
void DrawSeaMine();
void FinSeaMine();

BlockData* CreateSeaMine(MapChipType type, VECTOR pos);
BlockData* GetSeaMine();