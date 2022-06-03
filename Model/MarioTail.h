#pragma once
#ifndef __MARIO_TAIL_H__
#define __MARIO_TAIL_H__

#pragma region INCLUDE
#include "Engine/GameObject.h"
#pragma endregion

#pragma region DEFINE
/* SPRITE ID */
// BBOX
#define SPR_MARIO_TAIL_BBOX_LEFT				999901
#define SPR_MARIO_TAIL_BBOX_RIGHT				999902

/* ANIMATION ID */
/* SOUNDCLIP ID */
#pragma endregion

class CMarioTail : public CGameObject
{
public:
	CMarioTail(
		pGame game,
		unsigned int id, std::string name, std::string source,
		float x, float y, int gx, int gy, unsigned int layer
	) : CGameObject(game, id, name, source, x, y, gx, gy, layer) {};

	/* Left Body */
	bool _renderLeftBody = false;
	float LEFT_BODY_WIDTH = 0;
	float LEFT_BODY_HEIGHT = 0;
	float LEFT_BODY_OFFSETX = 0;
	float LEFT_BODY_OFFSETY = 0;

	/* Left Body */
	bool _renderRightBody = false;
	float RIGHT_BODY_WIDTH = 0;
	float RIGHT_BODY_HEIGHT = 0;
	float RIGHT_BODY_OFFSETX = 0;
	float RIGHT_BODY_OFFSETY = 0;

	/* Logic */
	bool _left = false;
	bool _activate = false;

public:
	virtual void Load();
	virtual void Start();
	virtual void Update(float elapsedMs);
	virtual void Render();

	virtual int IsCollidable();
	virtual int IsBlocking();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void OnNoCollision(float elapsedMs);
	void OnCollisionWith(pCollision e);
	void OnCollisionWithGoomba(pCollision e);
	void OnCollisionWithKoopa(pCollision e);
	void OnCollisionWithPlant(pCollision e);
	void OnCollisionWithBrick(pCollision e);
	void OnCollisionWithBlock(pCollision e);
};
typedef CMarioTail* pMarioTail;

#endif // !__MARIO_TAIL_H__
