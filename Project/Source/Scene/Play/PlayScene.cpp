#include "DxLib.h"
#include "../../BackGround/BackGround.h"
//#include "../../Scene/SceneManager.h"
#include "../../Map/MapManager.h"
#include "../../Player/Player.h"
//#include "../../Collision/Collision.h"
#include "../../Camera/Camera.h"


// 背景のスピード
#define BACK_GROUND_SPEED (-1.5f)

void InitPlayScene()
{
	// 背景初期化
	InitBackGround();

	// プレイヤー初期化
	InitPlayer();

	// マップ初期化
	InitMap();

	// カメラ初期化
	InitCamera();
}

void LoadPlayScene()
{
	// 背景ロード
	LoadBackGround("Data/Play/AbyssLoad2.png", BACK_GROUND_LAYER_1);

	// プレイヤーロード
	LoadPlayer();

	// マップをロード
	LoadMap();
}

void StartPlayScene()
{
	// プレイヤー開始
	StartPlayer();

	// マップを開始
	StartMap();
}

void StepPlayScene()
{
	// プレイヤーステップ
	StepPlayer();

	// カメラステップ
	StepCamera();
}

void UpdatePlayScene()
{
	//// マップ更新
	// プレイヤー更新
	UpdatePlayer();

	//// 当たり判定
	//CheckCollision();
}

void DrawPlayScene()
{
	// 背景
	DrawBackGround();

	// マップ描画
	DrawMap();

	// プレイヤー描画
	DrawPlayer();

	// カメラ描画
	DrawCamera();
}

void FinPlayScene()
{
	// 背景
	FinBackGround();

	// マップ終了
	FinMap();

	// プレイヤー終了
	FinPlayer();
}
