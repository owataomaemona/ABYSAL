#pragma once
#include "MapParameter.h"

#define INVISIBLE_WIDTH  (7680.0f)
#define INVISIBLE_HEIGHT (48.0f)

void InitInvisible();
void LoadInvisible();
void StartInvisible();
void StepInvisible();
void DrawInvisible();
void FinInvisible();

InvisibleData* CreateInvisible(MapChipType type, VECTOR pos);
InvisibleData* GetInvisible();