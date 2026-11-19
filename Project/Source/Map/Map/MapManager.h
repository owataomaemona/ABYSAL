#pragma once
#include "MapParameter.h"
#include "../../Player/Player.h"

void InitMap();
void LoadMap();
void StartMap();
void StepMap();
void DrawMap();
void FinMap();

void CheckMapPlayer();

void CheckSeaMinePlayer();

void CheckInvisiblePlayer();