#pragma once
#ifndef __HIT_EFFECT_H__
#define __HIT_EFFECT_H__

#pragma region INCLUDE
#include "Engine/GameObject.h"
#pragma endregion

#pragma region DEFINE
/* SPRITE ID */
#define SPR_BBOX					999901
#define SPR_MICRO_GOOMBA_LEFT		110101
#define SPR_MICRO_GOOMBA_RIGHT		120101
#pragma endregion

class CHitEffect : public CGameObject
{
public:
	CHitEffect(
		pGame game,
		unsigned int id, std::string name, std::string source,
		float x, float y, int gx, int gy, unsigned int layer
	) : CGameObject(game, id, name, source, x, y, gx, gy, layer) {};

public:
	virtual void Load();
	virtual void Start();
	virtual void Update(float elapsedMs);
	virtual void Render();

	virtual int IsCollidable();
	virtual int IsBlocking();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};
typedef CHitEffect* pHitEffect;

#endif // !__HIT_EFFECT_H__
