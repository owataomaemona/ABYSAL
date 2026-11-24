#include "BossEnemyManager.h"
#include "../BossEnemy/FirstBoss/FirstBoss.h"

int g_BossSpawnTimer = 0;

void InitBossEnemy()
{
	// ボスエネミー出現タイマーをリセット
	g_BossSpawnTimer = 0;

	// 第一ボス初期化
	InitFirstBoss();
}

void LoadBossEnemy()
{
	// 各ボスエネミーをロード

	// 第一ボスロード
	LoadFirstBoss();
}

void StartBossEnemy()
{
	// 各ボスエネミーを開始

	// 第一ボス開始
	StartFirstBoss();
}

void StepBossEnemy()
{
	// 各ボスエネミーをステップ

	// 第一ボスステップ
	StepFirstBoss();
}

void UpdateBossEnemy()
{
	// 各ボスエネミーを更新

	// 第一ボス更新
	UpdateFirstBoss();
}

void DrawBossEnemy()
{
	// 各ボスエネミーを描画

	// 第一ボス描画
	DrawFirstBoss();
}

void FinBossEnemy()
{
	// 各ボスエネミーを終了

	// 第一ボス終了
	FinFirstBoss();
}
