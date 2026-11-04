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
	PlayerData player = GetPlayer();
	BlockData* block = GetBlocks();
	
	// 今のとこ)プレイヤーが生きているとき
	if (player.active)
	{
		// X軸だけ移動する
		player.pos.x += player.move.x;

		// マップチップと当たり判定＆押し出し
		for (int y = 0; y < MAP_CHIP_Y_NUM; y++) 
		{
			for (int x = 0; x < MAP_CHIP_X_NUM; x++) 
			{
				// マップチップ取得
				MapChipData mapChipData = GetMapChipData(x, y);
				// マップチップが0の場合は何もない
				if (mapChipData.mapChip == 0) continue;

				if (CheckSquareSquare(player.pos.x, player.pos.y, player.width, player.height,
					block->pos.x, block->pos.y, block->width, block->height))
				{
					// 左から当たったか
					if (player.move.x > 0.0f)
					{
						// 左に押し出す
						player.pos.x -= (player.pos.x + player.width) - block->pos.x;
					}
					// 右から当たったか
					else if (player.move.x > 0.0f)
					{
						// 右に押し出す
						player.pos.x += (block->pos.x + block->width) - player.pos.x;
					}

					// 移動量は0にする
					player.move.x = 0.0f;
				}
			}
		}

		// Y軸だけ移動する
		player.pos.y += player.move.y;

		// マップチップと当たり判定＆押し出し
		for (int y = 0; y < MAP_CHIP_Y_NUM; y++)
		{
			for (int x = 0; x < MAP_CHIP_X_NUM; x++)
			{
				// マップチップ取得
				MapChipData mapChipData = GetMapChipData(x, y);
				// マップチップが0の場合は何もない
				if (mapChipData.mapChip == 0) continue;

				if (CheckSquareSquare(player.pos.x, player.pos.y, player.width, player.height,
					block->pos.x, block->pos.y, block->width, block->height))
				{
					// 上から当たったか
					if (player.move.y > 0.0f)
					{
						// 上に押し出す
						player.pos.y -= (player.pos.y + player.width) - block->pos.y;
						
						// 空中フラグを折る
						player.isAir = false;
					}
					// 下から当たったか
					else if (player.move.y > 0.0f)
					{
						// 下に押し出す
						player.pos.y += (block->pos.y + block->width) - player.pos.y;
					}

					// 移動量は0にする
					player.move.y = 0.0f;
				}
			}
		}
	}
}