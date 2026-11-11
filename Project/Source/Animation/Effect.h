#pragma once
#include "../Animation/Animation.h"

enum EffectType
{
	EFFECT_BOMB,
	EFFECT_MAX,
	EFFECT_NONE = -1
};

struct EffectData
{
	bool active;
	bool isLoop;
	int handle;
	int nowFrame;
	int frameNum;
	int frameWidth;
	int frameHeight;
	int timer;
	int interval;
	VECTOR pos;
};

// エフェクト用パラメーター
struct EffectAnimationParam
{
	int interval;
	int frameNum;
	int width;
	int height;
};
const EffectAnimationParam EFFECT_ANIM_PARAM[EFFECT_MAX] =
{
	25,8,96,96  // BOMB
};

void InitEffectAnimation();
void LoadEffectAnimation();
void StepEffectAnimation();
void UpdateEffectAnimation();
void DrawEffectAnimation();
void FinEffectAnimation();

// エフェクト開始
EffectData* StartEffectAnimation(EffectType type, float posX,float posY,int interval = -1,bool isLoop = false);

// エフェクト取得
EffectData* GetEffectAnimation();
