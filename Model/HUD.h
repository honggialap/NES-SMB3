#pragma once
#ifndef __HUD_H__
#define __HUD_H__

#pragma region INCLUDE
#include "Engine/GameObject.h"
#include "Mario.h"
#pragma endregion

#pragma region DEFINE
/* SPRITE ID */
#define SPR_HUD_FRAME	100001

#define SPR_ARROW		200001
#define SPR_POWER		200002

/* ANIMATION ID */
/* SOUNDCLIP ID */
#pragma endregion

class CHUD : public CGameObject
{
public:
	CHUD(
		pGame game,
		unsigned int id, std::string name, std::string source,
		float x, float y, int gx, int gy, unsigned int layer
	) : CGameObject(game, id, name, source, x, y, gx, gy, layer) {};

	/* Stats */
	float POWER_LIMIT = 0;
	float BLINK_INTERVAL = 0;

	float FRAME_X = 0;
	float FRAME_Y = 0;

	float LEVEL1_X = 0;
	float LEVEL1_Y = 0;

	float LEVEL2_X = 0;
	float LEVEL2_Y = 0;

	float LEVEL3_X = 0;
	float LEVEL3_Y = 0;

	float LEVEL4_X = 0;
	float LEVEL4_Y = 0;

	float LEVEL5_X = 0;
	float LEVEL5_Y = 0;

	float LEVEL6_X = 0;
	float LEVEL6_Y = 0;

	float POWER_X = 0;
	float POWER_Y = 0;

	/* Logic */
	pMario _mario = nullptr;
	unsigned int _level = 0;
	bool _blink = false;
	float _blinkInterval = 0;

public:
	virtual void Load();
	virtual void Start();
	virtual void Update(float elapsedMs);
	virtual void Render();

	virtual int IsCollidable();
	virtual int IsBlocking();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};
typedef CHUD* pHUD;

#endif // !__HUD_H__
