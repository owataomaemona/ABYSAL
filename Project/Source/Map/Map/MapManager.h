#pragma once
#include "MapParameter.h"
#include "../../Player/Player.h"

void InitMap();
void LoadMap();
void StartMap();
void StepMap();
void DrawMap();
void FinMap();

// ここからマップチップ関係の当たり判定
void CheckMapPlayer();

void CheckSeaMinePlayer();

void CheckSignBoardPlayer();

void CheckInvisiblePlayer();