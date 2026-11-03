#pragma once
#include "MapParameter.h"

void InitSeaMine();
void LoadSeaMine();
void StartSeaMine();
void StepSeaMine();
void DrawSeaMine();
void FinSeaMine();

SeaMineData* CreateSeaMine(MapChipType type, VECTOR pos);
SeaMineData* GetSeaMine();