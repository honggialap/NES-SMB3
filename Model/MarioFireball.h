#pragma once
#ifndef __MARIO_FIREBALL_H__
#define __MARIO_FIREBALL_H__

#pragma region INCLUDE
#include "Engine/GameObject.h"
#pragma endregion

#pragma region DEFINE
/* SPRITE ID */
// BBOX
#define SPR_MARIO_FIREBALL_BBOX				999901

#define SPR_MARIO_FIREBALL1_LEFT			100101
#define SPR_MARIO_FIREBALL2_LEFT			100102
#define SPR_MARIO_FIREBALL3_LEFT			100103
#define SPR_MARIO_FIREBALL4_LEFT			100104

#define SPR_MARIO_FIREBALL1_RIGHT			110101
#define SPR_MARIO_FIREBALL2_RIGHT			110102
#define SPR_MARIO_FIREBALL3_RIGHT			110103
#define SPR_MARIO_FIREBALL4_RIGHT			110104

/* ANIMATION ID */
#define ANI_MARIO_FIREBALL_LEFT				1001
#define ANI_MARIO_FIREBALL_RIGHT			1101
/* SOUNDCLIP ID */
#pragma endregion

class CMarioFireball : public CGameObject
{
public:
	CMarioFireball(
		pGame game,
		unsigned int id, std::string name, std::string source,
		float x, float y, int gx, int gy, unsigned int layer
	) : CGameObject(game, id, name, source, x, y, gx, gy, layer) {};

	/* Body */
	bool _renderBody = false;
	float BODY_WIDTH = 0;
	float BODY_HEIGHT = 0;
	float BODY_OFFSETX = 0;
	float BODY_OFFSETY = 0;

	/* Stats */
	float DECAY_INTERVAL = 0;
	float DRAG_FORCE = 0;
	float GRAVITY = 0;
	float INITIAL_FORCE = 0;

	/* Logic */
	float _decayInterval = 0;

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

	void OnCollisionWithPlatform(pCollision e);
	void OnCollisionWithPipe(pCollision e);
	void OnCollisionWithBrick(pCollision e);
	void OnCollisionWithBlock(pCollision e);

	void OnCollisionWithGoomba(pCollision e);
	void OnCollisionWithKoopa(pCollision e);
	void OnCollisionWithPlant(pCollision e);
};
typedef CMarioFireball* pMarioFireball;

#endif // !__MARIO_FIREBALL_H__
