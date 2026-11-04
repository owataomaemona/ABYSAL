#include "MapManager.h"
#include "MapParameter.h"
#include "MapChip.h"
#include "../Collision/Collision.h"
#include "Block.h"
#include "SeaMine.h"
#include "../Player/Player.h"

// キャラクターの周囲何マスまでチェックするか
#define CHECK_ROUND_NUM (2)

void InitMap()
{
}

void LoadMap()
{
	LoadMapChipData();

	// ブロックのロード
	LoadBlock();

	// 機雷のロード
	LoadSeaMine();
}

void StartMap()
{
	CreateMap();
}

void DrawMap()
{
	// ブロック描画
	DrawBlock();

	// 機雷描画
	DrawSeaMine();
}

void FinMap()
{
}

void CheckMapPlayer() 
{
	PlayerData* player = GetPlayer();
	PlayerData temp = CopyPlayer();

	// 今のとこ)プレイヤーが生きているとき
	if (player->active)
	{
		// X軸だけ移動する
		temp.pos.x += temp.move.x;

		// マップチップと当たり判定＆押し出し
		for (int y = 0; y < MAP_CHIP_Y_NUM; y++)
		{
			for (int x = 0; x < MAP_CHIP_X_NUM; x++)
			{
				// マップチップ取得
				MapChipData mapChipData = GetMapChipData(x, y);
				// マップチップが0の場合は何もない
				if (mapChipData.mapChip == 0) continue;

				if (CheckSquareSquare(temp.pos.x, temp.pos.y, temp.width, temp.height,
					mapChipData.block->pos.x, mapChipData.block->pos.y, mapChipData.block->width, mapChipData.block->height))
				{
					// 左から当たったか
					if (temp.move.x > 0.0f)
					{
						// 左に押し出す
						player->pos.x -= (temp.pos.x + temp.width) - mapChipData.block->pos.x;
					}
					// 右から当たったか
					else if (temp.move.x < 0.0f)
					{
						// 右に押し出す
						player->pos.x += (mapChipData.block->pos.x + mapChipData.block->width) - temp.pos.x;
					}

					// 移動量は0にする
					temp.move.x = 0.0f;
				}
			}
		}

		// Y軸だけ移動する
		temp.pos.y += player->move.y;

		// マップチップと当たり判定＆押し出し
		for (int y = 0; y < MAP_CHIP_Y_NUM; y++)
		{
			for (int x = 0; x < MAP_CHIP_X_NUM; x++)
			{
				// マップチップ取得
				MapChipData mapChipData = GetMapChipData(x, y);
				// マップチップが0の場合は何もない
				if (mapChipData.mapChip == 0) continue;

				if (CheckSquareSquare(temp.pos.x, temp.pos.y, player->width, player->height,
					mapChipData.block->pos.x, mapChipData.block->pos.y, mapChipData.block->width, mapChipData.block->height))
				{
					// 上から当たったか
					if (player->move.y > 0.0f)
					{
						// 上に押し出す
						player->pos.y -= (temp.pos.y + player->height) - mapChipData.block->pos.y;
						
						// 空中フラグを折る
						player->isAir = false;
					}
					// 下から当たったか
					else if (player->move.y < 0.0f)
					{
						// 下に押し出す
						player->pos.y += (mapChipData.block->pos.y + mapChipData.block->height) - temp.pos.y;
					}

					// 移動量は0にする
					player->move.y = 0.0f;
				}
			}
		}
	}
}