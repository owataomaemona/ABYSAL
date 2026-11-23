#include "DxLib.h"
#include "SignBoard.h"
#include "../../Camera/Camera.h"
#include "../../Animation/Effect.h"

SignBoardData g_SignBoards[BLOCK_MAX] = { 0 };
int g_SignBoardHandle[BLOCK_TYPE_MAX] = { 0 };

void InitSignBoard() 
{
	SignBoardData* signBoard = g_SignBoards;
	for (int i = 0; i < BLOCK_MAX; i++, signBoard++)
	{
		signBoard->active = false;
		signBoard->pos = VGet(0.0f, 0.0f, 0.0f);
		signBoard->type = MAP_CHIP_NONE;

		InitAnimation(&signBoard->animation); // &signBoardはアニメーション停止防止
	}
}

void LoadSignBoard()
{
	g_SignBoardHandle[SIGN_BOARD_BLOCK] = LoadGraph("Data/Map/SignBoard.png");
}

void StepSignBoard()
{
	SignBoardData* signBoard = g_SignBoards;
	for (int i = 0; i < BLOCK_MAX; i++, signBoard++)
	{
		if (signBoard->active)
		{
			UpdateAnimation(&signBoard->animation); // ステージ上にある看板を一つずつチェックしてアニメーションを進める
		}
	}
}

void DrawSignBoard()
{
	// スクロールのためカメラを取得
	CameraData camera = GetCamera();

	SignBoardData* signBoard = g_SignBoards;
	for (int i = 0; i < BLOCK_MAX; i++, signBoard++)
	{
		if (signBoard->active)
		{
			// アニメーション描画
			DrawAnimation(&signBoard->animation, signBoard->pos.x - camera.pos.x, signBoard->pos.y - camera.pos.y, false);
		}
	}
}

void FinSignBoard()
{
	for (int i = 0; i < BLOCK_TYPE_MAX; i++)
	{
		DeleteGraph(g_SignBoardHandle[i]);
	}
}

SignBoardData* CreateSignBoard(MapChipType type, VECTOR pos)
{
	// 未使用のブロックを探す
	SignBoardData* signBoard = g_SignBoards;
	for (int i = 0; i < BLOCK_MAX; i++, signBoard++)
	{
		if (!signBoard->active)
		{
			// 看板生成
			signBoard->active = true;
			signBoard->handle = g_SignBoardHandle[type];
			signBoard->pos = pos;
			signBoard->type = type;
			signBoard->width = SIGN_BOARD_WIDTH;
			signBoard->height = SIGN_BOARD_HEIGHT;
			
			// アニメーション用のハンドルも書くこと！
			signBoard->animation.handle = g_SignBoardHandle[type];
			
			// アニメーションも行うため、CreateSignBoard内にスタートアニメーションを書く
			const MapAnimationParam& param = SIGN_BOARD_ANIM_PARAM; // ↓の文章短縮用
			StartAnimation(&signBoard->animation, pos.x, pos.y, param.interval, param.frameNum, param.width, param.height, param.isLoop);

			return signBoard;
		}
	}

	return nullptr;
}

SignBoardData* GetSignBoard()
{
	return g_SignBoards;
}

void SignBoardAreaPlayer(SignBoardData* signBoard)
{
	if (signBoard != nullptr && signBoard->active)
	{
		StartEffectAnimation(EFFECT_TEACH_Z, signBoard->pos.x, signBoard->pos.y);
	}
}