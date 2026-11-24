#include "MapManager.h"
#include "MapParameter.h"
#include "MapChip.h"
#include "../../Collision/Collision.h"
#include "Block.h"
#include "../SeaMine/SeaMine.h"
#include "Invisible.h"
#include "SignBoard.h"
#include "../../Player/Player.h"

// キャラクターの周囲何マスまでチェックするか
#define CHECK_ROUND_NUM (2)

// 当たり判定小数誤差許容値
#define COLLISION_EPS (0.001f)

void InitMap()
{
	// 機雷の初期化
	InitSeaMine();

	// 看板の初期化
	InitSignBoard();

}

void LoadMap()
{
	LoadMapChipData();

	// ブロックのロード
	LoadBlock();

	// 機雷のロード
	LoadSeaMine();

	// 透明ブロックのロード
	LoadInvisible();

	// 看板のロード
	LoadSignBoard();
}

void StartMap()
{
	CreateMap();
}

void StepMap() 
{
	// 看板のアニメーション更新
	StepSignBoard();
}

void DrawMap()
{
	// ブロック描画
	DrawBlock();

	// 機雷描画
	DrawSeaMine();

	// 透明ブロック描画
	DrawInvisible();

	// 看板描画
	DrawSignBoard();
}

void FinMap()
{
	// 機雷終了
	FinSeaMine();

	// 看板終了
	FinSignBoard();
}

void CheckMapPlayer() 
{
	PlayerData* player = GetPlayer();
	
	// プレイヤーが生きているとき
	if (player->active)
	{	
		// マップチップと当たり判定＆押し出し
		for (int y = 0; y < MAP_CHIP_Y_NUM; y++)
		{
			for (int x = 0; x < MAP_CHIP_X_NUM; x++)
			{
				// マップチップ取得
				MapChipData mapChipData = GetMapChipData(x, y);
				// マップチップが以下と0の場合は何もない
				if (mapChipData.mapChip == SEA_MINE_BLOCK ||
					mapChipData.mapChip == INVISIBLE_BLOCK ||
					mapChipData.mapChip == SIGN_BOARD_BLOCK ||
					mapChipData.mapChip == 0) continue;

				if (CheckSquareSquare(player->pos.x, player->beforePos.y, player->width, player->height,
					mapChipData.block->pos.x, mapChipData.block->pos.y, mapChipData.block->width, mapChipData.block->height))
				{
					// 左から当たったか
					if (player->move.x > 0.0f)
					{
						// 左に押し出す
						player->pos.x -= (player->pos.x + player->width) - mapChipData.block->pos.x;
					}
					// 右から当たったか
					else if (player->move.x < 0.0f)
					{
						// 右に押し出す
						player->pos.x += (mapChipData.block->pos.x + mapChipData.block->width) - player->pos.x;
					}

					// 移動量は0にする
					player->move.x = 0.0f;
				}
			}
		}

		// マップチップと当たり判定＆押し出し
		for (int y = 0; y < MAP_CHIP_Y_NUM; y++)
		{
			for (int x = 0; x < MAP_CHIP_X_NUM; x++)
			{
				// マップチップ取得
				MapChipData mapChipData = GetMapChipData(x, y);
				// マップチップが以下と0の場合は何もない
				if (mapChipData.mapChip == SEA_MINE_BLOCK ||
					mapChipData.mapChip == INVISIBLE_BLOCK ||
					mapChipData.mapChip == SIGN_BOARD_BLOCK ||
					mapChipData.mapChip == 0) continue;

				// 矩形と矩形で当たり判定を取る
				if (CheckSquareSquare(player->pos.x, player->pos.y, player->width, player->height,
					mapChipData.block->pos.x, mapChipData.block->pos.y, mapChipData.block->width, mapChipData.block->height))
				{
					// 上から当たったか
					if (player->move.y > 0.0f)
					{
						// 上に押し出す
						player->pos.y -= (player->pos.y + player->height) - mapChipData.block->pos.y;
						
						// 空中フラグを折る
						player->isAir = false;
					}
					// 下から当たったか
					else if (player->move.y < 0.0f)
					{
						// 下に押し出す
						player->pos.y += (mapChipData.block->pos.y + mapChipData.block->height) - player->pos.y;
					}

					// 移動量は0にする
					player->move.y = 0.0f;
				}
			}
		}
	}
}

void CheckSeaMinePlayer() 
{
	PlayerData* player = GetPlayer();
	
	if (player->active || player != nullptr) 
	{
		float playerCenterX = player->pos.x + PLAYER_WIDTH / 2;
		float playerCenterY = player->pos.y + PLAYER_HEIGHT / 2;

		for (int y = 0; y < MAP_CHIP_Y_NUM; y++)
		{
			for (int x = 0; x < MAP_CHIP_X_NUM; x++)
			{
				// マップチップ取得
				MapChipData mapChipData = GetMapChipData(x, y);
				// マップチップがSEA_MINE_BLOCK以外の場合は何もしない
				if (mapChipData.mapChip != SEA_MINE_BLOCK) continue;

				// 機雷のポインタが空でないか確認
				if (mapChipData.seaMine == nullptr) continue;

				float seaMineCenterX = mapChipData.seaMine->pos.x + SEA_MINE_WIDTH / 2;
				float seaMineCenterY = mapChipData.seaMine->pos.y + SEA_MINE_HEIGHT / 2;

				// 円で当たり判定を取る(機雷本体との当たり判定)
				if (CheckCircleCircle(playerCenterX, playerCenterY, PLAYER_RADIUS,
					seaMineCenterX, seaMineCenterY, SEA_MINE_RADIUS))
				{
					mapChipData.seaMine->isNowGraze = false;

					PlayerHitSeaMine();
					SeaMineHitPlayer(mapChipData.seaMine);

					continue;
				}
				// ギリギリ判定(離脱判定)
				if (!mapChipData.seaMine->isSeaMineGraze) // まだギリギリ判定してないか
				{
					if (CheckCircleCircle(playerCenterX, playerCenterY, PLAYER_GRAZE_RADIUS,
						seaMineCenterX, seaMineCenterY, SEA_MINE_RADIUS))
					{
						// この当たり判定内にいるときにisNowGrazeがtrueになる
						mapChipData.seaMine->isNowGraze = true;
					}
					// isNowGrazeから離れたとき
					else if(mapChipData.seaMine->isNowGraze)
					{
						mapChipData.seaMine->isNowGraze = false;
						mapChipData.seaMine->isSeaMineGraze = true;
						GrazePlayerHitSeaMine(player);
					}
				}
			}
		}
	}
}

void CheckSignBoardPlayer()
{
	PlayerData* player = GetPlayer();

	if (player->active || player != nullptr)
	{
		float playerCenterX = player->pos.x + PLAYER_WIDTH / 2;
		float playerCenterY = player->pos.y + PLAYER_HEIGHT / 2;

		for (int y = 0; y < MAP_CHIP_Y_NUM; y++)
		{
			for (int x = 0; x < MAP_CHIP_X_NUM; x++)
			{
				// マップチップ取得
				MapChipData mapChipData = GetMapChipData(x, y);
				// マップチップがSIGN_BOARD_BLOCK以外の場合は何もしない
				if (mapChipData.mapChip != SIGN_BOARD_BLOCK) continue;

				// 機雷のポインタが空でないか確認
				if (mapChipData.signBoard == nullptr) continue;

				float signBoardCenterX = mapChipData.signBoard->pos.x + SIGN_BOARD_WIDTH / 2;
				float signBoardCenterY = mapChipData.signBoard->pos.y + SIGN_BOARD_HEIGHT / 2;

				// 円で当たり判定を取る(機雷本体との当たり判定)
				if (CheckCircleCircle(playerCenterX, playerCenterY, PLAYER_RADIUS,
					signBoardCenterX, signBoardCenterY, SIGN_BOARD_RADIUS))
				{
					if (!mapChipData.signBoard->isTouch)
					{
						mapChipData.signBoard->isTouch = true;

						mapChipData.signBoard->countChat++;

						SignBoardAreaPlayer(mapChipData.signBoard);
					}

					continue;
				}
			}
		}
	}
}

void CheckInvisiblePlayer()
{
	PlayerData* player = GetPlayer();

	// プレイヤーが生きているとき
	if (player->active)
	{
		// マップチップと当たり判定＆押し出し
		for (int y = 0; y < MAP_CHIP_Y_NUM; y++)
		{
			for (int x = 0; x < MAP_CHIP_X_NUM; x++)
			{
				// マップチップ取得
				MapChipData mapChipData = GetMapChipData(x, y);
				// マップチップがINVISIBLE_BLOCK以外の場合は何もしない
				if (mapChipData.mapChip != INVISIBLE_BLOCK) continue;

				if (CheckSquareSquare(player->pos.x, player->beforePos.y, player->width, player->height,
					mapChipData.invisible->pos.x, mapChipData.invisible->pos.y, mapChipData.invisible->width, mapChipData.invisible->height))
				{
					// 左から当たったか
					if (player->move.x > 0.0f)
					{
						// 左に押し出す
						player->pos.x -= (player->pos.x + player->width) - mapChipData.invisible->pos.x;
					}
					// 右から当たったか
					else if (player->move.x < 0.0f)
					{
						// 右に押し出す
						player->pos.x += (mapChipData.invisible->pos.x + mapChipData.invisible->width) - player->pos.x;
					}

					// 移動量は0にする
					player->move.x = 0.0f;
				}
			}
		}

		// マップチップと当たり判定＆押し出し
		for (int y = 0; y < MAP_CHIP_Y_NUM; y++)
		{
			for (int x = 0; x < MAP_CHIP_X_NUM; x++)
			{
				// マップチップ取得
				MapChipData mapChipData = GetMapChipData(x, y);
				// マップチップがINVISIBLE_BLOCK以外の場合は何もしない
				if (mapChipData.mapChip != INVISIBLE_BLOCK) continue;

				// 矩形と矩形で当たり判定を取る
				if (CheckSquareSquare(player->pos.x, player->pos.y, player->width, player->height,
					mapChipData.invisible->pos.x, mapChipData.invisible->pos.y, mapChipData.invisible->width, mapChipData.invisible->height))
				{
					// 上から当たったか
					if (player->move.y > 0.0f)
					{
						// 上に押し出す
						player->pos.y -= (player->pos.y + player->height) - mapChipData.invisible->pos.y;

						// 空中フラグを折る
						player->isAir = false;
					}
					// 下から当たったか
					else if (player->move.y < 0.0f)
					{
						// 下に押し出す
						player->pos.y += (mapChipData.invisible->pos.y + mapChipData.invisible->height) - player->pos.y;
					}

					// 移動量は0にする
					player->move.y = 0.0f;
				}
			}
		}
	}
}