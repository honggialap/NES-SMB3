#pragma once
#ifndef __TILTE_H__
#define __TILTE_H__

#pragma region INCLUDE
#include "Engine/GameObject.h"
#pragma endregion

#pragma region DEFINE
/* SPRITE ID */
#define SPR_TITLE_BACKGROUND	100001

#define SPR_LOGO_1				200001
#define SPR_LOGO_2				200002
#define SPR_LOGO_3				200003
#define SPR_LOGO_4				200004

#define SPR_LABEL				400001

/* ANIMATION ID */
#define ANI_LOGO				2000

/* SOUNDCLIP ID */
#pragma endregion

class CTitle : public CGameObject
{
public:
	CTitle(
		pGame game,
		unsigned int id, std::string name, std::string source,
		float x, float y, int gx, int gy, unsigned int layer
	) : CGameObject(game, id, name, source, x, y, gx, gy, layer) {};

public:
	float CAMERA_ORIGIN_X = 0.0;
	float CAMERA_ORIGIN_Y = 0.0;

	float _backgroundX = 0.0;
	float _backgroundY = 0.0;

	float _logoX = 0.0;
	float _logoY = 0.0;

	float _labelX = 0.0;
	float _labelY = 0.0;
	bool _blink = false;
	float _blinkInterval = 0.0;
	float BLINK_INTERVAL = 0.0;

	unsigned int NEXT_SCENE = 0;

	unsigned int SPACE = 0;

public:
	virtual void Load();
	virtual void Start();
	virtual void Update(float elapsedMs);
	virtual void Render();

	virtual int IsCollidable();
	virtual int IsBlocking();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};
typedef CTitle* pTitle;

#endif // !__TILTE_H__
