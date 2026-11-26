#include "EnemyManager.h"
#include "SeaUrchin.h"

void InitEnemy()
{
	// ウニの初期化
	InitSeaUrchin();
}

void LoadEnemy()
{
	// ウニのロード
	LoadSeaUrchin();
}

void StartEnemy()
{
}

void StepEnemy()
{
	// ウニステップ
	StepSeaUrchin();
}

void UpdateEnemy()
{
}

void DrawEnemy()
{
	// ウニ描画
	DrawSeaUrchin();
}

void FinEnemy()
{
	// ウニ終了
	FinSeaUrchin();
}
