#include "DxLib.h"
#include "Camera.h"
#include "../Input/Input.h"
#include "../Player/Player.h"

#define DEBUG_CAMERA_SPEED (1.0f)
#define CAMERA_SCROLL_START_X (600.0f)

CameraData g_CameraData = { 0 };

void InitCamera()
{

}

void StepCamera()
{
	// プレイヤー取得
	PlayerData player = GetPlayer();


	//// スーパーマリオブラザーズ式
	//float targetX = player.posX - 300.0f; // targetXにプレイヤーの位置を格納

	//if (targetX > g_CameraData.pos.x) // プレイヤーが左に行ったら(新しく計算したカメラの位置(targetX)が現在よりも右側にあるか)
	//{
	//	g_CameraData.pos.x = targetX; // カメラの位置を最後左に行く前の位置で固定する(カメラの座標をtargetXに更新する)
	//}

	//g_CameraData.pos.y = player.posY - 600.0f;



	// 上下左右カメラ追従
	g_CameraData.pos.x = player.posX - 300.0f;
	g_CameraData.pos.y = player.posY - 600.0f;



	//// プレイヤー取得(強制スクロール)
	//PlayerData player = GetPlayer();

	/*if (player.moveX <= CAMERA_SCROLL_START_X)
	{
		g_CameraData.pos.x += DEBUG_CAMERA_SPEED;
	}
	else
	{
		g_CameraData.pos.x = 0.0f;
	}
	g_CameraData.pos.y = player.posY - 600.0;*/
}

void DrawCamera()
{
	DrawFormatString(0, 0, GetColor(255, 255, 255), "カメラの座標＝[%f,%f]", g_CameraData.pos.x, g_CameraData.pos.y);
}

CameraData GetCamera()
{
	return g_CameraData;
}